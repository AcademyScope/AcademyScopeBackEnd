/*
AcademyScopeBackEnd class definitions of AcademyScope
Copyright (C) 2025 Volkan Orhan

This program is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your option)
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include "AcademyScopeModel.hpp"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include "DataTypeDefinitions.hpp"
#include "ProgramTableColumnDefinitions.hpp"

AcademyScopeModel::AcademyScopeModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

void AcademyScopeModel::setDatabase(const QSqlDatabase &database)
{
    db = database;
}

void AcademyScopeModel::setBaseQuery(const QString &queryBase)
{
    if (!db.isOpen()) {
        qWarning() << "[AcademyScopeModel] Database is not open!";
        return;
    }

    beginResetModel();

    modelData.clear();
    baseQuery = "SELECT * " + queryBase;
    dataWindow = DataWindow(); // reset window state

    // --- Get total row count ---
    countQuery = "SELECT COUNT(*) " + queryBase;
    QSqlQuery count(db);
    if (count.exec(countQuery) && count.next())
        dataWindow.tableRowCount = count.value(0).toInt();
    else {
        qWarning() << "[AcademyScopeModel] COUNT query failed:" << count.lastError().text();
        dataWindow.tableRowCount = 0;
    }

    // --- Detect column count ---
    QString firstRowQuery = QString("%1 LIMIT 1").arg(baseQuery);
    QSqlQuery colQuery(db);
    if (colQuery.exec(firstRowQuery))
        dataWindow.columnCount = colQuery.record().count();
    else
        dataWindow.columnCount = 0;

    // --- Allocate placeholder slots ---
    modelData.resize(dataWindow.tableRowCount);

    endResetModel();

    // Load initial viewport
    dataWindow.beginningIndex = 0;
    dataWindow.endingIndex = std::min(dataWindow.windowSize - 1, dataWindow.tableRowCount - 1);
    loadCurrentWindow();
}

int AcademyScopeModel::rowCount(const QModelIndex &) const
{
    return dataWindow.tableRowCount;
}

int AcademyScopeModel::columnCount(const QModelIndex &) const
{
    return dataWindow.columnCount;
}

QVariant AcademyScopeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return {};

    const int r = index.row();
    if (r < 0 || r >= modelData.size())
        return {};

    const auto &row = modelData[r];
    if (row.isEmpty()) return {}; // not loaded yet

    if (index.column() >= row.size())
        return {};

    const QVariant &val = row[index.column()];
    return val.isNull() ? QVariant("—") : val;
}

QVariant AcademyScopeModel::headerData(int section,
                                       Qt::Orientation orientation,
                                       int role) const
{
    if (role != Qt::DisplayRole)
        return {};

    if (orientation == Qt::Horizontal) {
        switch (static_cast<ProgramTableColumn>(section)) {
        case ProgramTableColumn::ProgramKodu:                 return "Program Kodu";
        case ProgramTableColumn::UniversiteAdi:               return "Üniversite";
        case ProgramTableColumn::FakulteYuksekOkulAdi:        return "Kampüs";
        case ProgramTableColumn::ProgramAdi:                  return "Program";
        case ProgramTableColumn::PuanTuru:                    return "Puan Türü";
        case ProgramTableColumn::GenelKontenjan:              return "Kontenjan";
        case ProgramTableColumn::GenelYerlesen:               return "Yerleşen";
        case ProgramTableColumn::GenelBasariSirasi:           return "Başarı Sırası";
        case ProgramTableColumn::GenelEnKucukPuan:            return "En Küçük Puan";
        case ProgramTableColumn::OkulBirincisiKontenjan:      return "Okul Birincisi Kontenjan";
        case ProgramTableColumn::OkulBirincisiYerlesen:       return "Okul Birincisi Yerleşen";
        case ProgramTableColumn::OkulBirincisiBasariSirasi:   return "Okul Birincisi Başarı Sırası";
        case ProgramTableColumn::OkulBirincisiEnKucukPuan:    return "Okul Birincisi En Küçük Puan";
        case ProgramTableColumn::SehitGaziYakiniKontenjan:    return "Şehit / Gazi Yakını Kontenjan";
        case ProgramTableColumn::SehitGaziYakiniYerlesen:     return "Şehit / Gazi Yakını Yerleşen";
        case ProgramTableColumn::SehitGaziYakiniBasariSirasi: return "Şehit / Gazi Yakını Başarı Sırası";
        case ProgramTableColumn::SehitGaziYakiniEnKucukPuan:  return "Şehit / Gazi Yakını En Küçük Puan";
        case ProgramTableColumn::DepremzedeKontenjan:         return "Depremzede Kontenjan";
        case ProgramTableColumn::DepremzedeYerlesen:          return "Depremzede Yerleşen";
        case ProgramTableColumn::DepremzedeBasariSirasi:      return "Depremzede Başarı Sırası";
        case ProgramTableColumn::DepremzedeEnKucukPuan:       return "Depremzede En Küçük Puan";
        case ProgramTableColumn::Kadin34PlusKontenjan:        return "34+ Kadın Kontenjan";
        case ProgramTableColumn::Kadin34PlusYerlesen:         return "34+ Kadın Yerleşen";
        case ProgramTableColumn::Kadin34PlusBasariSirasi:     return "34+ Kadın Başarı Sırası";
        case ProgramTableColumn::Kadin34PlusEnKucukPuan:      return "34+ Kadın En Küçük Puan";
        }
    }

    if (orientation == Qt::Vertical)
        return QString::number(section + 1);

    return {};
}

void AcademyScopeModel::showColumn(ProgramTableColumn column) const
{
    emit columnVisibilityChanged(int(column), true);
}

void AcademyScopeModel::hideColumn(ProgramTableColumn column) const
{
    emit columnVisibilityChanged(int(column), false);
}

bool AcademyScopeModel::canFetchMore(const QModelIndex &parent) const
{

}

void AcademyScopeModel::fetchMore(const QModelIndex &parent)
{

}

void AcademyScopeModel::fetchMoreData()
{

}

DataWindow *AcademyScopeModel::getDataWindow()
{
    return &dataWindow;
}

/*--------------------------------------
 * LazyLoad++ core functions
 *-------------------------------------*/

void AcademyScopeModel::loadCurrentWindow()
{
    loadRows(dataWindow.beginningIndex, dataWindow.endingIndex);
}

void AcademyScopeModel::loadRows(int startRow, int endRow)
{
    if (!db.isOpen() || baseQuery.isEmpty())
        return;

    startRow = std::max(0, startRow);
    endRow   = std::min(dataWindow.tableRowCount - 1, endRow);

    const int fetchCount = endRow - startRow + 1;
    QString queryStr = QString("%1 LIMIT %2 OFFSET %3")
                           .arg(baseQuery)
                           .arg(fetchCount)
                           .arg(startRow);

    QSqlQuery query(db);
    if (!query.exec(queryStr)) {
        qWarning() << "[AcademyScopeModel] Query failed:" << query.lastError().text();
        return;
    }

    beginResetModel();

    if (modelData.isEmpty())
        modelData.resize(dataWindow.tableRowCount);

    int rowIndex = startRow;
    while (query.next() && rowIndex <= endRow) {
        QVector<QVariant> row;
        row.reserve(dataWindow.columnCount);
        for (int i = 0; i < dataWindow.columnCount; ++i)
            row.append(query.value(i).isNull() ? QVariant("—") : query.value(i));
        modelData[rowIndex] = row;
        ++rowIndex;
    }

    // Geri kalan satırları boşalt
    for (int i = 0; i < startRow; ++i)
        modelData[i].clear();
    for (int i = endRow + 1; i < modelData.size(); ++i)
        modelData[i].clear();

    endResetModel();

    qDebug() << "[AcademyScopeModel] Loaded rows:" << startRow << "-" << endRow;
}


void AcademyScopeModel::clear()
{
    beginResetModel();
    modelData.clear();
    dataWindow = DataWindow();
    baseQuery.clear();
    endResetModel();
}
