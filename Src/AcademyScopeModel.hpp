/*
MainWindow class declarations of AcademyScope
Copyright (C) 2025 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/
#pragma once
#include <QAbstractTableModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVector>
#include <QVariant>
#include "EnumDefinitions.hpp"

struct LazyLoadingInfo {
    int totalRowCount = 0;
    int limit = 300;
    int offset = 0;
    int columnCount = 0;
};

class AcademyScopeModel : public QAbstractTableModel {
    Q_OBJECT
signals:
    void columnVisibilityChanged(int index, bool visible) const;
public:
    explicit AcademyScopeModel(QObject *parent = nullptr);

    void setDatabase(const QSqlDatabase &db);
    void setBaseQuery(const QString &queryBase);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void showColumn(ProgramTableColumn column) const;
    void hideColumn(ProgramTableColumn column) const;

    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;

    void clear();

private:
    void fetchMoreData();

    QSqlDatabase db;
    QString baseQuery;     // Filtrelenmiş WHERE + ORDER BY
    QVector<QVector<QVariant>> modelData;
    LazyLoadingInfo lazyLoadingInfo;
};
