/*
AcademyScopeBackEnd class definitions of AcademyScope
Copyright (C) 2025 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/
#include "AcademyScopeModel.hpp"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include "EnumDefinitions.hpp"

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
    baseQuery = queryBase;
    lazyLoadingInfo = LazyLoadingInfo();

    // Get total row count
    QString countQuery = QString("SELECT COUNT(*) FROM (%1)").arg(baseQuery);
    QSqlQuery count(db);
    if (count.exec(countQuery) && count.next())
        lazyLoadingInfo.totalRowCount = count.value(0).toInt();
    else {
        qWarning() << "COUNT query failed:" << count.lastError().text();
        lazyLoadingInfo.totalRowCount = 0;
    }

    // Detect column count
    QString firstRowQuery = QString("%1 LIMIT 1").arg(baseQuery);
    QSqlQuery colQuery(db);
    if (colQuery.exec(firstRowQuery))
        lazyLoadingInfo.columnCount = colQuery.record().count();
    else
        lazyLoadingInfo.columnCount = 0;

    endResetModel();
    fetchMoreData();
}

int AcademyScopeModel::rowCount(const QModelIndex &) const
{
    return modelData.size();
}

int AcademyScopeModel::columnCount(const QModelIndex &) const
{
    return lazyLoadingInfo.columnCount;
}

QVariant AcademyScopeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return {};

    const auto &row = modelData[index.row()];
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
        case ProgramTableColumn::Universite:                  return "Üniversite";
        case ProgramTableColumn::Kampus:                      return "Kampüs";
        case ProgramTableColumn::Program:                     return "Program";
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
    qDebug()<<"Show column" << (int)column;
    emit columnVisibilityChanged(int(column), true);
}

void AcademyScopeModel::hideColumn(ProgramTableColumn column) const
{
    qDebug()<<"Hide column" << (int)column;
    emit columnVisibilityChanged(int(column), false);
}

bool AcademyScopeModel::canFetchMore(const QModelIndex &) const
{
    return modelData.size() < lazyLoadingInfo.totalRowCount;
}

void AcademyScopeModel::fetchMore(const QModelIndex &)
{
    fetchMoreData();
}

void AcademyScopeModel::fetchMoreData()
{
    if (!db.isOpen() || baseQuery.isEmpty())
        return;

    const int startRow = modelData.size();

    QString limitedQuery = QString("%1 LIMIT %2 OFFSET %3")
                               .arg(baseQuery)
                               .arg(lazyLoadingInfo.limit)
                               .arg(lazyLoadingInfo.offset);

    QSqlQuery query(db);
    if (!query.exec(limitedQuery)) {
        qWarning() << "[AcademyScopeModel] Query failed:" << query.lastError().text();
        return;
    }

    beginInsertRows(QModelIndex(), startRow, startRow + lazyLoadingInfo.limit - 1);

    while (query.next()) {
        QVector<QVariant> row;
        row.reserve(lazyLoadingInfo.columnCount);
        for (int i = 0; i < lazyLoadingInfo.columnCount; ++i) {
            QVariant value = query.value(i);
            if (value.isNull())
                value = QVariant("—");
            row.append(value);
        }
        modelData.append(row);
    }

    endInsertRows();
    lazyLoadingInfo.offset += lazyLoadingInfo.limit;
}

void AcademyScopeModel::clear()
{
    beginResetModel();
    modelData.clear();
    lazyLoadingInfo = LazyLoadingInfo();
    baseQuery.clear();
    endResetModel();
}

