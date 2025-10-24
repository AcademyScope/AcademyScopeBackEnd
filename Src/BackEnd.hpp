/*
MainWindow class declarations of AcademyScope
Copyright (C) 2025 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/
#pragma once

#include <QLocale>
#include <QSqlDatabase>
#include "DataTypeDefinitions.hpp"
#include <QStandardItemModel>
#include "AcademyScopeModel.hpp"

class ProgramTableInterface {
public:
    virtual void clearContents() = 0;
    virtual void setRowCount(int rowCount) = 0;
    virtual void setUpdatesEnabled(bool isEnabled) = 0;
    virtual void setSortingEnabled(bool isEnabled) = 0;
    virtual void insertRow(int rowIndex) = 0;
    virtual bool isColumnHidden(ProgramTableColumn column) = 0;
    virtual bool isColumnHidden(int columnIndex) = 0;
    virtual void setItem(int row, ProgramTableColumn column, QString text) = 0;
    virtual void showColumn(ProgramTableColumn column) = 0;
    virtual void hideColumn(ProgramTableColumn column) = 0;
    virtual void setColumnWidth(ProgramTableColumn column, int width) = 0;
protected:
    QList<Qt::AlignmentFlag> columnAlignments;
    virtual void setColumnAlignment(ProgramTableColumn column, Qt::AlignmentFlag alignment) = 0;
    virtual void init() = 0;
};

class AcademyScopeBackEnd {
public:
    AcademyScopeBackEnd();
    QList<University> getUniversities()const;
    QList<QString> getDepartments() const;
    void populateProgramTable(const AcademyScopeParameters &academyScopeParameters);
    AcademyScopeModel * getDataModel();

private:
    void initDB();
    void setProgramTableColumnWidths();
    void populateUniversitiesComboBox();
    void populateDepartmentsComboBox();
    void hideUnnecessaryColumnsOnTheProgramTable(const AcademyScopeParameters &academyScopeParameters) const;
    void hideUnusedColumnsOnTheProgramTable();
    void initializeYKSTableColumnNames();
    void setLogoDarkMode(bool isDarkMode);
    QString buildFilteredSql(const AcademyScopeParameters &academyScopeParameters);
    QString getDbColumnNameFromProgramTableColumnIndex(ProgramTableColumn columnIndex);
    AcademyScopeModel dataModel;
    DataWindow window;

    QLocale turkishLocale;
    QStringList yksTableColumnNames;
    QSqlDatabase db;
};
