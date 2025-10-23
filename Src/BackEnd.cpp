/*
AcademyScopeBackEnd class definitions of AcademyScope
Copyright (C) 2025 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/
#include "BackEnd.hpp"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QStandardItemModel>
#include <QCollator>
#include <QCollator>
#include <QFile>
#include <QStandardPaths>
#include <QDir>
#include <QtGlobal>
#include "Utils/SQLiteUtil.hpp"
#include "Utils/StringUtil.hpp"

AcademyScopeBackEnd::AcademyScopeBackEnd(ProgramTableInterface * programTable) {
    this->programTable = programTable;
    initDB();
}

QList<University> AcademyScopeBackEnd::getUniversities() {
    QSqlQuery query;
    QList<University> universities;

    if (query.exec("SELECT UniversiteID, UniversiteAdi FROM Universiteler")) {
        while (query.next()) {
            University university;
            university.id = query.value(0).toInt();
            university.name = query.value(1).toString();
            universities.append(university);
        }
    }

    // Sorts with Turkish collator
    QCollator collator(QLocale(QLocale::Turkish, QLocale::Turkey));
    std::sort(universities.begin(), universities.end(),
              [&](const University &a, const University &b) {
                  return collator.compare(a.name, b.name) < 0;
              });

    return universities;
}

void AcademyScopeBackEnd::initDB() {
    db = QSqlDatabase::addDatabase("QSQLITE");

    QString dbPath = SQLiteUtil::resolveDatabasePath();

    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qDebug() << "Veritabanı açılamadı:" << db.lastError().text();
        return;
    }
}

QList<QString> AcademyScopeBackEnd::getDepartments() {
    /*
    QStandardItem* firstItem = model->item(0);
    firstItem->setFlags(firstItem->flags() & ~Qt::ItemIsEnabled);
    firstItem->setForeground(QBrush(Qt::gray));
    */
    QSqlQuery query;
    QList<QString> departments;

    QString selectionQuery = "SELECT DISTINCT\n\
        TRIM(\n\
            CASE\n\
                WHEN instr(ProgramAdi, '(') > 0\n\
            THEN substr(ProgramAdi, 1, instr(ProgramAdi, '(') - 1)\n\
            ELSE ProgramAdi\n\
                END\n\
            ) AS AnaProgramAdi\n\
            FROM YKS;\
           ";

    if (query.exec(selectionQuery)) {
        while (query.next()) {
            QString department = query.value(0).toString();
            departments.append(department);
        }
    }

    QCollator collator(QLocale(QLocale::Turkish, QLocale::Turkey));
    std::sort(departments.begin(), departments.end(),
              [&](const QString &a, const QString &b) {
                  return collator.compare(a, b) < 0;
              });

    return departments;
}

void AcademyScopeBackEnd::populateProgramTable(const AcademyScopeParameters &academyScopeParameters) {
    programTable->clearContents();
    programTable->setRowCount(0);

    if (!db.isOpen())
        return;

    hideUnnecessaryColumnsOnTheProgramTable(academyScopeParameters);

    programTable->setUpdatesEnabled(false);
    programTable->setSortingEnabled(false);

    QString universityName = academyScopeParameters.universityName;
    QString department = academyScopeParameters.departmentName;
    universityName = turkishLocale.toUpper(universityName);
    QSqlQuery query;

    QStringList whereQueries;
    QStringList kontenjanQueries;
    QStringList tuitionQueries;
    QStringList gradeIntervalQueries;
    QString kontenjanSubQuery = "";
    QString tuitionSubQuery = "";
    QString gradeIntervalSubQuery = "";

    QString sqlQuery = "Select * FROM ";
    if(academyScopeParameters.placementType == PlacementType::Regular)
        sqlQuery += "YKS";
    else if(academyScopeParameters.placementType == PlacementType::Additional)
        sqlQuery += "EkTercihDetayli";

    if(universityName.trimmed() != "") {
        whereQueries.append("UniversiteAdi LIKE \"%" + StringUtil::toTurkishUpperCase(universityName) + "%\"");
    }

    if(department.trimmed() != "") {
        whereQueries.append("ProgramAdi LIKE \"%" + StringUtil::toTurkishTitleCase(department) + "%\"");
    }

    if(academyScopeParameters.country == Country::Turkiye) {
        whereQueries.append("UlkeKodu = 90");
    }
    else if(academyScopeParameters.country == Country::Cyprus) {
        whereQueries.append("UlkeKodu = 357");
    }
    else if(academyScopeParameters.country == Country::ForeignCountries) {
        whereQueries.append("UlkeKodu <> 90");
        whereQueries.append("UlkeKodu <> 357");
    }

    if(academyScopeParameters.degreeType == DegreeType::Bachelor) {
        whereQueries.append("Lisans = 1");
    }
    else if(academyScopeParameters.degreeType == DegreeType::Associate) {
        whereQueries.append("Lisans = 0");
    }

    if(academyScopeParameters.universityType == UniversityType::Government) {
        whereQueries.append("DevletUniversitesi = 1");
    }
    else if(academyScopeParameters.universityType == UniversityType::Private) {
        whereQueries.append("DevletUniversitesi = 0");
    }

    switch(academyScopeParameters.trackType) {
    case TrackType::Science:
        whereQueries.append("PuanTuru = \"SAY\"");
        break;
    case TrackType::EqualWeight:
        whereQueries.append("PuanTuru = \"EA\"");
        break;
    case TrackType::Humanities:
        whereQueries.append("PuanTuru = \"SÖZ\"");
        break;
    case TrackType::TYT:
        whereQueries.append("PuanTuru = \"TYT\"");
        break;
    case TrackType::Language:
        whereQueries.append("PuanTuru = \"DİL\"");
        break;
    }

    double enKucukPuan, enBuyukPuan;

    if(academyScopeParameters.scoreInterval.minimum.has_value())
        enKucukPuan = academyScopeParameters.scoreInterval.minimum.value();
    else
        enKucukPuan = 0;

    if(academyScopeParameters.scoreInterval.maximum.has_value())
        enBuyukPuan = academyScopeParameters.scoreInterval.maximum.value();
    else
        enBuyukPuan = 0;

    QString genelPuanAraligiQuery = "";
    if(academyScopeParameters.selectedQuotaTypes.regularQuota || academyScopeParameters.selectedQuotaTypes.trncNationalsQuota || academyScopeParameters.selectedQuotaTypes.mtokQuota) {
        if(enKucukPuan > 100) {
            genelPuanAraligiQuery = "GenelEnKucukPuan > " + QString::number(enKucukPuan);
        }
        if(enBuyukPuan < 560) {
            if(!genelPuanAraligiQuery.isEmpty())
                genelPuanAraligiQuery += " AND ";
            genelPuanAraligiQuery += "GenelEnBuyukPuan < " + QString::number(enBuyukPuan);
        }
        if(!genelPuanAraligiQuery.isEmpty())
            gradeIntervalQueries.append(genelPuanAraligiQuery);
    }

    QString okulBirincisiPuanAraligiQuery = "";
    if(academyScopeParameters.selectedQuotaTypes.highSchoolValedictoriansQuota) {
        if(enKucukPuan > 100) {
            okulBirincisiPuanAraligiQuery = "OkulBirincisiEnKucukPuan > " + QString::number(enKucukPuan);
        }
        if(enBuyukPuan < 560) {
            if(!okulBirincisiPuanAraligiQuery.isEmpty())
                okulBirincisiPuanAraligiQuery += " AND ";
            okulBirincisiPuanAraligiQuery += "OkulBirincisiEnKucukPuan < " + QString::number(enBuyukPuan);
        }
        if(!okulBirincisiPuanAraligiQuery.isEmpty())
            gradeIntervalQueries.append(okulBirincisiPuanAraligiQuery);
    }

    QString sehitGaziPuanAraligiQuery = "";
    if(academyScopeParameters.selectedQuotaTypes.martyrsAndVeteransQuota) {
        if(enKucukPuan > 100) {
            sehitGaziPuanAraligiQuery = "SehitGaziEnKucukPuan > " + QString::number(enKucukPuan);
        }
        if(enBuyukPuan < 560) {
            if(!sehitGaziPuanAraligiQuery.isEmpty())
                sehitGaziPuanAraligiQuery += " AND ";
            sehitGaziPuanAraligiQuery += "SehitGaziEnKucukPuan < " + QString::number(enBuyukPuan);
        }
        if(!sehitGaziPuanAraligiQuery.isEmpty())
            gradeIntervalQueries.append(sehitGaziPuanAraligiQuery);
    }

    QString depremzedePuanAraligiQuery = "";
    if(academyScopeParameters.selectedQuotaTypes.earthquakeVictimsQuota) {
        if(enKucukPuan > 100) {
            depremzedePuanAraligiQuery = "DepremzedeEnKucukPuan > " + QString::number(enKucukPuan);
        }
        if(enBuyukPuan < 560) {
            if(!depremzedePuanAraligiQuery.isEmpty())
                depremzedePuanAraligiQuery += " AND ";
            depremzedePuanAraligiQuery += "DepremzedeEnKucukPuan < " + QString::number(enBuyukPuan);
        }
        if(!depremzedePuanAraligiQuery.isEmpty())
            gradeIntervalQueries.append(depremzedePuanAraligiQuery);
    }

    QString kadin34PuanAraligiQuery = "";
    if(academyScopeParameters.selectedQuotaTypes.women34PlusQuota) {
        if(enKucukPuan > 100) {
            kadin34PuanAraligiQuery = "Kadin34EnKucukPuan > " + QString::number(enKucukPuan);
        }
        if(enBuyukPuan < 560) {
            if(!kadin34PuanAraligiQuery.isEmpty())
                kadin34PuanAraligiQuery += " AND ";
            kadin34PuanAraligiQuery += "Kadin34EnKucukPuan < " + QString::number(enBuyukPuan);
        }
        if(!kadin34PuanAraligiQuery.isEmpty())
            gradeIntervalQueries.append(kadin34PuanAraligiQuery);
    }

    if(!gradeIntervalQueries.isEmpty()) {
        gradeIntervalSubQuery += "(" + gradeIntervalQueries[0];
        for(int i = 1; i < gradeIntervalQueries.size(); i++) {
            gradeIntervalSubQuery += " OR " + gradeIntervalQueries[i];
        }
        gradeIntervalSubQuery += ")";
    }

    if(!gradeIntervalSubQuery.isEmpty()) {
        whereQueries.append(gradeIntervalSubQuery);
    }

    if (academyScopeParameters.selectedQuotaTypes.regularQuota) {
        kontenjanQueries.append("GenelKontenjan IS NOT NULL");
    }

    if (academyScopeParameters.selectedQuotaTypes.highSchoolValedictoriansQuota) {
        kontenjanQueries.append("OkulBirincisiKontenjan IS NOT NULL");
    }

    if (academyScopeParameters.selectedQuotaTypes.martyrsAndVeteransQuota) {
        kontenjanQueries.append("SehitGaziKontenjan IS NOT NULL");
    }

    if (academyScopeParameters.selectedQuotaTypes.earthquakeVictimsQuota) {
        kontenjanQueries.append("DepremzedeKontenjan IS NOT NULL");
    }

    if (academyScopeParameters.selectedQuotaTypes.women34PlusQuota) {
        kontenjanQueries.append("Kadin34Kontenjan IS NOT NULL");
    }

    if (academyScopeParameters.selectedTuitionFeeTypes.free) {
        tuitionQueries.append("UcretDurumu = 0");
    }

    if (academyScopeParameters.selectedTuitionFeeTypes.discounted) {
        tuitionQueries.append("UcretDurumu = 50");
    }

    if (academyScopeParameters.selectedTuitionFeeTypes.paid) {
        tuitionQueries.append("UcretDurumu = 100");
    }

    if (academyScopeParameters.selectedQuotaTypes.trncNationalsQuota) {
        kontenjanQueries.append("KKTCUyruklu = TRUE"); //In order to add OR Query, it is appended to kontenjanQueries
    }
    else {
        whereQueries.append("KKTCUyruklu = FALSE"); //In order to add AND Query, it is appended to whereQueries
    }

    if (academyScopeParameters.selectedQuotaTypes.mtokQuota) {
        kontenjanQueries.append("MTOK = TRUE"); //In order to add OR Query, it is appended to kontenjanQueries
    }
    else {
        whereQueries.append("MTOK = FALSE"); //In order to add AND Query, it is appended to whereQueries
    }

    if(!kontenjanQueries.isEmpty()) {
        kontenjanSubQuery += "(" + kontenjanQueries[0];
        for(int i = 1; i < kontenjanQueries.size(); i++) {
            kontenjanSubQuery += " OR " + kontenjanQueries[i];
        }
        kontenjanSubQuery += ")";
    }

    if(!kontenjanSubQuery.isEmpty()) {
        whereQueries.append(kontenjanSubQuery);
    }

    if(!tuitionQueries.isEmpty()) {
        tuitionSubQuery += "(" + tuitionQueries[0];
        for(int i = 1; i < tuitionQueries.size(); i++) {
            tuitionSubQuery += " OR " + tuitionQueries[i];
        }
        tuitionSubQuery += ")";
    }

    if(!tuitionSubQuery.isEmpty())
        whereQueries.append(tuitionSubQuery);

    if(kontenjanSubQuery.isEmpty() || tuitionSubQuery.isEmpty()) {
        programTable->setRowCount(0);
        programTable->setUpdatesEnabled(true);
        programTable->setSortingEnabled(true);
        return;
    }

    // LASTLY (FINALLY) process "WHERE" queries
    if(!whereQueries.isEmpty()) {
        sqlQuery += " WHERE ";
        sqlQuery += whereQueries[0];
        for(int i = 1; i < whereQueries.size(); i++) {
            sqlQuery += " AND " + whereQueries[i];
        }
    }

    if(academyScopeParameters.order.toBeOrdered == false) {
        const QString key = SQLiteUtil::trOrderExprFor("ProgramKodu");
        sqlQuery += " ORDER BY ProgramKodu ASC";
    }
    else if(!programTable->isColumnHidden(academyScopeParameters.order.column)) {
        QString col = getDbColumnNameFromProgramTableColumnIndex(academyScopeParameters.order.column);
        const QString key = SQLiteUtil::trOrderExprFor(col);
        sqlQuery += " ORDER BY " + key;
        if(academyScopeParameters.order.direction == Qt::AscendingOrder)
            sqlQuery += " ASC";
        else
            sqlQuery += " DESC";
    }

    if (query.exec(sqlQuery)) {
        int row = 0;
        while (query.next()) {
            programTable->insertRow(row);
            int id = query.value("ProgramKodu").toInt();
            QString universityType = query.value("UniversiteTuru").toString();
            QString universityName = query.value("UniversiteAdi").toString();
            QString facultyName = query.value("FakulteYuksekokulAdi").toString();
            QString programAdi = query.value("ProgramAdi").toString();
            QString puanTuru = query.value("PuanTuru").toString();
            QString genelKontenjan = query.value("GenelKontenjan").toString();
            QString generalMinimumPoints = query.value("GenelEnKucukPuan").toString();

            QString sehitGaziKontenjan = query.value("SehitGaziKontenjan").toString();
            QString sehitGaziEnKucukPuan = query.value("SehitGaziEnKucukPuan").toString();

            QString depremzedeKontenjan = query.value("DepremzedeKontenjan").toString();
            QString depremzedeEnKucukPuan = query.value("DepremzedeEnKucukPuan").toString();

            QString kadin34Kontenjan = query.value("Kadin34Kontenjan").toString();
            QString kadin34EnKucukPuan = query.value("Kadin34EnKucukPuan").toString();

            //Ek kontenjanda yok
            if(academyScopeParameters.placementType == PlacementType::Regular) {
                //Ek kontenjanda yok
                QString okulBirincisiKontenjan = query.value("OkulBirincisiKontenjan").toString();
                QString okulBirincisiEnKucukPuan = query.value("OkulBirincisiEnKucukPuan").toString();

                QString okulBirincisiYerlesen = query.value("OkulBirincisiYerlesen").toString();
                QString sehitGaziYerlesen = query.value("SehitGaziYerlesen").toString();
                QString depremzedeYerlesen = query.value("DepremzedeYerlesen").toString();
                QString kadin34Yerlesen = query.value("Kadin34Yerlesen").toString();

                QString genelYerlesen = query.value("GenelYerlesen").toString();
                QString generalMaximumPoints = query.value("GenelEnBuyukPuan").toString();

                programTable->setItem(row, ProgramTableColumn::GenelYerlesen, genelYerlesen);
                programTable->setItem(row, ProgramTableColumn::OkulBirincisiYerlesen, okulBirincisiYerlesen);
                programTable->setItem(row, ProgramTableColumn::SehitGaziYakiniYerlesen, sehitGaziYerlesen);
                programTable->setItem(row, ProgramTableColumn::DepremzedeYerlesen, depremzedeYerlesen);
                programTable->setItem(row, ProgramTableColumn::Kadin34PlusYerlesen, kadin34Yerlesen);

                programTable->setItem(row, ProgramTableColumn::OkulBirincisiKontenjan, okulBirincisiKontenjan);
                programTable->setItem(row, ProgramTableColumn::OkulBirincisiEnKucukPuan, okulBirincisiEnKucukPuan);
            }

            programTable->setItem(row, ProgramTableColumn::ProgramKodu, QString::number(id));
            programTable->setItem(row, ProgramTableColumn::Universite, universityName);
            programTable->setItem(row, ProgramTableColumn::Kampus, facultyName);
            programTable->setItem(row, ProgramTableColumn::Program, programAdi);
            programTable->setItem(row, ProgramTableColumn::PuanTuru, puanTuru);

            programTable->setItem(row, ProgramTableColumn::GenelKontenjan, genelKontenjan);
            programTable->setItem(row, ProgramTableColumn::GenelEnKucukPuan, generalMinimumPoints);

            programTable->setItem(row, ProgramTableColumn::SehitGaziYakiniKontenjan, sehitGaziKontenjan);
            programTable->setItem(row, ProgramTableColumn::SehitGaziYakiniEnKucukPuan, sehitGaziEnKucukPuan);

            programTable->setItem(row, ProgramTableColumn::DepremzedeKontenjan, depremzedeKontenjan);
            programTable->setItem(row, ProgramTableColumn::DepremzedeEnKucukPuan, depremzedeEnKucukPuan);

            programTable->setItem(row, ProgramTableColumn::Kadin34PlusKontenjan, kadin34Kontenjan);
            programTable->setItem(row, ProgramTableColumn::Kadin34PlusEnKucukPuan, kadin34EnKucukPuan);

            row++;
        }
    }

    programTable->setUpdatesEnabled(true);
    programTable->setSortingEnabled(true);

    return;
}

void AcademyScopeBackEnd::hideUnnecessaryColumnsOnTheProgramTable(const AcademyScopeParameters &academyScopeParameters) {
    if(academyScopeParameters.selectedQuotaTypes.regularQuota || academyScopeParameters.selectedQuotaTypes.trncNationalsQuota || academyScopeParameters.selectedQuotaTypes.mtokQuota) {
        programTable->showColumn(ProgramTableColumn::GenelKontenjan);
        programTable->showColumn(ProgramTableColumn::GenelYerlesen);
        programTable->showColumn(ProgramTableColumn::GenelEnKucukPuan);
    }
    else {
        programTable->hideColumn(ProgramTableColumn::GenelKontenjan);
        programTable->hideColumn(ProgramTableColumn::GenelYerlesen);
        programTable->hideColumn(ProgramTableColumn::GenelEnKucukPuan);
    }

    if(academyScopeParameters.selectedQuotaTypes.highSchoolValedictoriansQuota) {
        programTable->showColumn(ProgramTableColumn::OkulBirincisiKontenjan);
        programTable->showColumn(ProgramTableColumn::OkulBirincisiYerlesen);
        programTable->showColumn(ProgramTableColumn::OkulBirincisiEnKucukPuan);
    }
    else {
        programTable->hideColumn(ProgramTableColumn::OkulBirincisiKontenjan);
        programTable->hideColumn(ProgramTableColumn::OkulBirincisiYerlesen);
        programTable->hideColumn(ProgramTableColumn::OkulBirincisiEnKucukPuan);
    }

    if(academyScopeParameters.selectedQuotaTypes.martyrsAndVeteransQuota) {
        programTable->showColumn(ProgramTableColumn::SehitGaziYakiniKontenjan);
        programTable->showColumn(ProgramTableColumn::SehitGaziYakiniYerlesen);
        programTable->showColumn(ProgramTableColumn::SehitGaziYakiniEnKucukPuan);
    }
    else {
        programTable->hideColumn(ProgramTableColumn::SehitGaziYakiniKontenjan);
        programTable->hideColumn(ProgramTableColumn::SehitGaziYakiniYerlesen);
        programTable->hideColumn(ProgramTableColumn::SehitGaziYakiniEnKucukPuan);
    }


    if(academyScopeParameters.selectedQuotaTypes.earthquakeVictimsQuota) {
        programTable->showColumn(ProgramTableColumn::DepremzedeKontenjan);
        programTable->showColumn(ProgramTableColumn::DepremzedeYerlesen);
        programTable->showColumn(ProgramTableColumn::DepremzedeEnKucukPuan);
    }
    else {
        programTable->hideColumn(ProgramTableColumn::DepremzedeKontenjan);
        programTable->hideColumn(ProgramTableColumn::DepremzedeYerlesen);
        programTable->hideColumn(ProgramTableColumn::DepremzedeEnKucukPuan);
    }


    if(academyScopeParameters.selectedQuotaTypes.women34PlusQuota) {
        programTable->showColumn(ProgramTableColumn::Kadin34PlusKontenjan);
        programTable->showColumn(ProgramTableColumn::Kadin34PlusYerlesen);
        programTable->showColumn(ProgramTableColumn::Kadin34PlusEnKucukPuan);
    }
    else {
        programTable->hideColumn(ProgramTableColumn::Kadin34PlusKontenjan);
        programTable->hideColumn(ProgramTableColumn::Kadin34PlusYerlesen);
        programTable->hideColumn(ProgramTableColumn::Kadin34PlusEnKucukPuan);
    }

    //Ek kontenjanda yok
    if(academyScopeParameters.placementType == PlacementType::Regular) {
        if(academyScopeParameters.selectedQuotaTypes.regularQuota)
            programTable->showColumn(ProgramTableColumn::GenelYerlesen);
        if(academyScopeParameters.selectedQuotaTypes.highSchoolValedictoriansQuota)
            programTable->showColumn(ProgramTableColumn::OkulBirincisiYerlesen);
        if(academyScopeParameters.selectedQuotaTypes.highSchoolValedictoriansQuota)
            programTable->showColumn(ProgramTableColumn::SehitGaziYakiniYerlesen);
        if(academyScopeParameters.selectedQuotaTypes.earthquakeVictimsQuota)
            programTable->showColumn(ProgramTableColumn::DepremzedeYerlesen);
        if(academyScopeParameters.selectedQuotaTypes.women34PlusQuota)
            programTable->showColumn(ProgramTableColumn::Kadin34PlusYerlesen);
    }
    else {
        programTable->hideColumn(ProgramTableColumn::GenelYerlesen);
        programTable->hideColumn(ProgramTableColumn::OkulBirincisiYerlesen);
        programTable->hideColumn(ProgramTableColumn::SehitGaziYakiniYerlesen);
        programTable->hideColumn(ProgramTableColumn::DepremzedeYerlesen);
        programTable->hideColumn(ProgramTableColumn::Kadin34PlusYerlesen);
    }
}

void AcademyScopeBackEnd::hideUnusedColumnsOnTheProgramTable() {
    programTable->hideColumn(ProgramTableColumn::GenelBasariSirasi);
    programTable->hideColumn(ProgramTableColumn::OkulBirincisiBasariSirasi);
    programTable->hideColumn(ProgramTableColumn::SehitGaziYakiniBasariSirasi);
    programTable->hideColumn(ProgramTableColumn::DepremzedeBasariSirasi);
    programTable->hideColumn(ProgramTableColumn::Kadin34PlusBasariSirasi);
}

void AcademyScopeBackEnd::initializeYKSTableColumnNames()
{
    yksTableColumnNames = {
        "ProgramKodu",
        "UniversiteTuru",
        "UniversiteAdi",
        "FakulteYuksekokulAdi",
        "ProgramAdi",
        "PuanTuru",
        "GenelKontenjan",
        "GenelYerlesen",
        "GenelEnKucukPuan",
        "GenelEnBuyukPuan",
        "OkulBirincisiKontenjan",
        "OkulBirincisiYerlesen",
        "OkulBirincisiEnKucukPuan",
        "OkulBirincisiEnBuyukPuan",
        "DepremzedeKontenjan",
        "DepremzedeYerlesen",
        "DepremzedeEnKucukPuan",
        "DepremzedeEnBuyukPuan",
        "Kadin34Kontenjan",
        "Kadin34Yerlesen",
        "Kadin34EnKucukPuan",
        "Kadin34EnBuyukPuan",
        "SehitGaziKontenjan",
        "SehitGaziYerlesen",
        "SehitGaziEnKucukPuan",
        "SehitGaziEnBuyukPuan",
        "Lisans",
        "UlkeKodu"
    };
}

QString AcademyScopeBackEnd::getDbColumnNameFromProgramTableColumnIndex(ProgramTableColumn column) {
    switch (column) {
    case ProgramTableColumn::ProgramKodu:              return "ProgramKodu";
    case ProgramTableColumn::Universite:               return "UniversiteAdi";
    case ProgramTableColumn::Kampus:                   return "FakulteYuksekokulAdi";
    case ProgramTableColumn::Program:                  return "ProgramAdi";
    case ProgramTableColumn::PuanTuru:                 return "PuanTuru";
    case ProgramTableColumn::GenelKontenjan:           return "GenelKontenjan";
    case ProgramTableColumn::GenelYerlesen:            return "GenelYerlesen";
    case ProgramTableColumn::GenelEnKucukPuan:         return "GenelEnKucukPuan";
    case ProgramTableColumn::OkulBirincisiKontenjan:   return "OkulBirincisiKontenjan";
    case ProgramTableColumn::OkulBirincisiYerlesen:    return "OkulBirincisiYerlesen";
    case ProgramTableColumn::OkulBirincisiEnKucukPuan: return "OkulBirincisiEnKucukPuan";
    case ProgramTableColumn::SehitGaziYakiniKontenjan: return "SehitGaziKontenjan";
    case ProgramTableColumn::SehitGaziYakiniYerlesen:  return "SehitGaziYerlesen";
    case ProgramTableColumn::SehitGaziYakiniEnKucukPuan:return "SehitGaziEnKucukPuan";
    case ProgramTableColumn::DepremzedeKontenjan:      return "DepremzedeKontenjan";
    case ProgramTableColumn::DepremzedeYerlesen:       return "DepremzedeYerlesen";
    case ProgramTableColumn::DepremzedeEnKucukPuan:    return "DepremzedeEnKucukPuan";
    case ProgramTableColumn::Kadin34PlusKontenjan:     return "Kadin34Kontenjan";
    case ProgramTableColumn::Kadin34PlusYerlesen:      return "Kadin34Yerlesen";
    case ProgramTableColumn::Kadin34PlusEnKucukPuan:   return "Kadin34EnKucukPuan";
    default: return QString();
    }
}
