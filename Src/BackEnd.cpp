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

AcademyScopeBackEnd::AcademyScopeBackEnd() {
    initDB();
}

QList<University> AcademyScopeBackEnd::getUniversities() const {
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
    dataModel.setDatabase(db);
}

QList<QString> AcademyScopeBackEnd::getDepartments() const {
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
    QString baseQuery = buildFilteredSql(academyScopeParameters);
    dataModel.setBaseQuery(baseQuery);
    hideUnnecessaryColumnsOnTheProgramTable(academyScopeParameters);
}

AcademyScopeModel *AcademyScopeBackEnd::getDataModel()
{
    return &dataModel;
}

void AcademyScopeBackEnd::hideUnnecessaryColumnsOnTheProgramTable(const AcademyScopeParameters &academyScopeParameters) const {
    if(academyScopeParameters.selectedQuotaTypes.regularQuota || academyScopeParameters.selectedQuotaTypes.trncNationalsQuota || academyScopeParameters.selectedQuotaTypes.mtokQuota) {
        dataModel.showColumn(ProgramTableColumn::GenelKontenjan);
        dataModel.showColumn(ProgramTableColumn::GenelYerlesen);
        dataModel.showColumn(ProgramTableColumn::GenelEnKucukPuan);
    }
    else {
        dataModel.hideColumn(ProgramTableColumn::GenelKontenjan);
        dataModel.hideColumn(ProgramTableColumn::GenelYerlesen);
        dataModel.hideColumn(ProgramTableColumn::GenelEnKucukPuan);
    }

    if(academyScopeParameters.selectedQuotaTypes.highSchoolValedictoriansQuota) {
        dataModel.showColumn(ProgramTableColumn::OkulBirincisiKontenjan);
        dataModel.showColumn(ProgramTableColumn::OkulBirincisiYerlesen);
        dataModel.showColumn(ProgramTableColumn::OkulBirincisiEnKucukPuan);
    }
    else {
        dataModel.hideColumn(ProgramTableColumn::OkulBirincisiKontenjan);
        dataModel.hideColumn(ProgramTableColumn::OkulBirincisiYerlesen);
        dataModel.hideColumn(ProgramTableColumn::OkulBirincisiEnKucukPuan);
    }

    if(academyScopeParameters.selectedQuotaTypes.martyrsAndVeteransQuota) {
        dataModel.showColumn(ProgramTableColumn::SehitGaziYakiniKontenjan);
        dataModel.showColumn(ProgramTableColumn::SehitGaziYakiniYerlesen);
        dataModel.showColumn(ProgramTableColumn::SehitGaziYakiniEnKucukPuan);
    }
    else {
        dataModel.hideColumn(ProgramTableColumn::SehitGaziYakiniKontenjan);
        dataModel.hideColumn(ProgramTableColumn::SehitGaziYakiniYerlesen);
        dataModel.hideColumn(ProgramTableColumn::SehitGaziYakiniEnKucukPuan);
    }


    if(academyScopeParameters.selectedQuotaTypes.earthquakeVictimsQuota) {
        dataModel.showColumn(ProgramTableColumn::DepremzedeKontenjan);
        dataModel.showColumn(ProgramTableColumn::DepremzedeYerlesen);
        dataModel.showColumn(ProgramTableColumn::DepremzedeEnKucukPuan);
    }
    else {
        dataModel.hideColumn(ProgramTableColumn::DepremzedeKontenjan);
        dataModel.hideColumn(ProgramTableColumn::DepremzedeYerlesen);
        dataModel.hideColumn(ProgramTableColumn::DepremzedeEnKucukPuan);
    }


    if(academyScopeParameters.selectedQuotaTypes.women34PlusQuota) {
        dataModel.showColumn(ProgramTableColumn::Kadin34PlusKontenjan);
        dataModel.showColumn(ProgramTableColumn::Kadin34PlusYerlesen);
        dataModel.showColumn(ProgramTableColumn::Kadin34PlusEnKucukPuan);
    }
    else {
        dataModel.hideColumn(ProgramTableColumn::Kadin34PlusKontenjan);
        dataModel.hideColumn(ProgramTableColumn::Kadin34PlusYerlesen);
        dataModel.hideColumn(ProgramTableColumn::Kadin34PlusEnKucukPuan);
    }

    //Ek kontenjanda yok
    if(academyScopeParameters.placementType == PlacementType::Regular) {
        if(academyScopeParameters.selectedQuotaTypes.regularQuota)
            dataModel.showColumn(ProgramTableColumn::GenelYerlesen);
        if(academyScopeParameters.selectedQuotaTypes.highSchoolValedictoriansQuota)
            dataModel.showColumn(ProgramTableColumn::OkulBirincisiYerlesen);
        if(academyScopeParameters.selectedQuotaTypes.highSchoolValedictoriansQuota)
            dataModel.showColumn(ProgramTableColumn::SehitGaziYakiniYerlesen);
        if(academyScopeParameters.selectedQuotaTypes.earthquakeVictimsQuota)
            dataModel.showColumn(ProgramTableColumn::DepremzedeYerlesen);
        if(academyScopeParameters.selectedQuotaTypes.women34PlusQuota)
            dataModel.showColumn(ProgramTableColumn::Kadin34PlusYerlesen);
    }
    else {
        dataModel.hideColumn(ProgramTableColumn::GenelYerlesen);
        dataModel.hideColumn(ProgramTableColumn::OkulBirincisiYerlesen);
        dataModel.hideColumn(ProgramTableColumn::SehitGaziYakiniYerlesen);
        dataModel.hideColumn(ProgramTableColumn::DepremzedeYerlesen);
        dataModel.hideColumn(ProgramTableColumn::Kadin34PlusYerlesen);
    }
}

void AcademyScopeBackEnd::hideUnusedColumnsOnTheProgramTable() {
    dataModel.hideColumn(ProgramTableColumn::GenelBasariSirasi);
    dataModel.hideColumn(ProgramTableColumn::OkulBirincisiBasariSirasi);
    dataModel.hideColumn(ProgramTableColumn::SehitGaziYakiniBasariSirasi);
    dataModel.hideColumn(ProgramTableColumn::DepremzedeBasariSirasi);
    dataModel.hideColumn(ProgramTableColumn::Kadin34PlusBasariSirasi);
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

QString AcademyScopeBackEnd::buildFilteredSql(const AcademyScopeParameters &p)
{
    QStringList where;
    QString sql = "FROM ";

    // Base table
    sql += (p.placementType == PlacementType::Additional)
               ? "EkTercihDetayli" : "YKS";

    // University name
    if (!p.universityName.trimmed().isEmpty())
        where << QString("UniversiteAdi LIKE '%%1%'")
                     .arg(StringUtil::toTurkishUpperCase(p.universityName));

    // Department
    if (!p.departmentName.trimmed().isEmpty())
        where << QString("ProgramAdi LIKE '%%1%'")
                     .arg(StringUtil::toTurkishTitleCase(p.departmentName));

    // Country filter
    switch (p.country) {
    case Country::Turkiye:          where << "UlkeKodu = 90"; break;
    case Country::Cyprus:           where << "UlkeKodu = 357"; break;
    case Country::ForeignCountries: where << "UlkeKodu NOT IN (90, 357)"; break;
    case Country::AllCountries:
        break;
    }

    // Degree type
    if (p.degreeType == DegreeType::Bachelor)      where << "Lisans = 1";
    else if (p.degreeType == DegreeType::Associate) where << "Lisans = 0";

    // University type
    if (p.universityType == UniversityType::Government)      where << "DevletUniversitesi = 1";
    else if (p.universityType == UniversityType::Private)    where << "DevletUniversitesi = 0";

    // Track type
    switch (p.trackType) {
    case TrackType::Science:      where << "PuanTuru = 'SAY'"; break;
    case TrackType::EqualWeight:  where << "PuanTuru = 'EA'";  break;
    case TrackType::Humanities:   where << "PuanTuru = 'SÖZ'"; break;
    case TrackType::TYT:          where << "PuanTuru = 'TYT'"; break;
    case TrackType::Language:     where << "PuanTuru = 'DİL'"; break;
    case TrackType::Undefined:    break;
    }

    // Score range
    double minScore = p.scoreInterval.minimum.value_or(0);
    double maxScore = p.scoreInterval.maximum.value_or(0);
    QString scoreRange;

    if (minScore > 100)
        scoreRange += QString("GenelEnKucukPuan > %1").arg(minScore);
    if (maxScore < 560) {
        if (!scoreRange.isEmpty()) scoreRange += " AND ";
        scoreRange += QString("GenelEnBuyukPuan < %1").arg(maxScore);
    }
    if (!scoreRange.isEmpty())
        where << QString("(%1)").arg(scoreRange);

    // Quota types
    QStringList kontenjan;
    if (p.selectedQuotaTypes.regularQuota)           kontenjan << "GenelKontenjan IS NOT NULL";
    if (p.selectedQuotaTypes.highSchoolValedictoriansQuota) kontenjan << "OkulBirincisiKontenjan IS NOT NULL";
    if (p.selectedQuotaTypes.martyrsAndVeteransQuota) kontenjan << "SehitGaziKontenjan IS NOT NULL";
    if (p.selectedQuotaTypes.earthquakeVictimsQuota)  kontenjan << "DepremzedeKontenjan IS NOT NULL";
    if (p.selectedQuotaTypes.women34PlusQuota)        kontenjan << "Kadin34Kontenjan IS NOT NULL";
    if (p.selectedQuotaTypes.trncNationalsQuota)      kontenjan << "KKTCUyruklu = TRUE";
    else                                              where << "KKTCUyruklu = FALSE";
    if (p.selectedQuotaTypes.mtokQuota)               kontenjan << "MTOK = TRUE";
    else                                              where << "MTOK = FALSE";

    if (!kontenjan.isEmpty())
        where << "(" + kontenjan.join(" OR ") + ")";

    // Tuition filters
    QStringList tuition;
    if (p.selectedTuitionFeeTypes.free)       tuition << "UcretDurumu = 0";
    if (p.selectedTuitionFeeTypes.discounted) tuition << "UcretDurumu = 50";
    if (p.selectedTuitionFeeTypes.paid)       tuition << "UcretDurumu = 100";
    if (!tuition.isEmpty())                   where << "(" + tuition.join(" OR ") + ")";

    // Combine WHERE clauses
    if (!where.isEmpty())
        sql += " WHERE " + where.join(" AND ");

    // Order
    if (p.order.toBeOrdered) {
        QString col = getDbColumnNameFromProgramTableColumnIndex(p.order.column);
        sql += QString(" ORDER BY %1 %2")
                   .arg(SQLiteUtil::trOrderExprFor(col))
                   .arg(p.order.direction == Qt::AscendingOrder ? "ASC" : "DESC");
    } else {
        sql += " ORDER BY ProgramKodu ASC";
    }

    return sql;
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
