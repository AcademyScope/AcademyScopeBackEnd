/*
MainWindow class declarations of AcademyScope
Copyright (C) 2025 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/
#pragma once

#include <QMainWindow>
#include <QLocale>
#include <QSqlDatabase>
#include "EnumDefinitions.hpp"
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QStandardItemModel>
#include <optional>

struct University {
    int id;
    QString name;
};

struct Department {
    int id;
    QString name;
};

enum class UniversityType {
    Undefined = -1,
    Government = 0,
    Private = 1,
};

enum class TrackType {
    Science,    //Sayısal
    Humanities, //Sözel
    EqualWeight,//Eşit Ağırlık
    Language,   //Dil
    TYT         //TYT
};

enum class Country {
    AllCountries,
    Turkiye,
    Cyprus,
    ForeignCountries
};

enum class DegreeType {
    Bachelor, //Lisans
    Associate //Ön lisans
};

struct Interval {
    std::optional<double> minimum;
    std::optional<double> maximum;
};

struct SelectedQuotaTypes {
    bool regularQuota;
    bool highSchoolValedictoriansQuota;
    bool women34PlusQuota;
    bool earthquakeVictimsQuota;
    bool martyrsAndVeteransQuota;
    bool trncNationalsQuota;
    bool mtokQuota;
};

struct SelectedTuitionFeeTypes {
    bool Free;
    bool Discounted;
    bool Paid;
};

enum class PlacementType {
    Regular,       // Normal yerleştirme
    Additional     // Ek yerleştirme
};

struct AcademyScopeParameters {
    PlacementType placementType;
    QString universityName;
    QString departmentName;
    UniversityType universityType;
    TrackType trackType;
    Interval scoreInterval;
    Country country;
    DegreeType degreeType;
    SelectedQuotaTypes selectedQuotaTypes;
    SelectedTuitionFeeTypes selectedTuitionFeeTypes;
    Interval regularQuotaInterval,
          highSchoolValedictoriansQuotaInterval,
          women34PlusQuotaInterval,
          earthquakeVictimsQuotaInterval,
          martyrsAndVeteransQuotaInterval,
          trncNationalsQuotaInterval;
};

struct Program {

};

class ProgramTableInterface {
public:
    virtual void clearContents() = 0;
    virtual void setRowCount(int rowCount) = 0;
    virtual void setUpdatesEnabled(bool isEnabled) = 0;
    virtual void setSortingEnabled(bool isEnabled) = 0;
    virtual void createItem(QString text, Qt::AlignmentFlag) = 0;
    virtual void insertRow(int rowIndex) = 0;
    virtual bool isColumnHidden(int columnIndex) = 0;
    virtual void setItem(int row, ProgramTableColumn column, QString text) = 0;
    virtual void showColumn(ProgramTableColumn column) = 0;
    virtual void hideColumn(ProgramTableColumn column) = 0;
protected:
    QList<Qt::AlignmentFlag> columnAlignments;
    virtual void setColumnAlignment(int columnIndex, Qt::AlignmentFlag alignment) = 0;
    virtual void init() = 0;
};

class AcademyScopeBackEnd {
public:
    AcademyScopeBackEnd(ProgramTableInterface * programTable);
    QList<University> getUniversities();
    QList<QString> getDepartments();
    void populateProgramTable(const AcademyScopeParameters &academyScopeParameters);

private:
    void initDB();
    void setProgramTableColumnWidths();
    void populateUniversitiesComboBox();
    void populateDepartmentsComboBox();
    void hideUnnecessaryColumnsOnTheProgramTable(const AcademyScopeParameters &academyScopeParameters);
    void hideUnusedColumnsOnTheProgramTable();
    void initializeYKSTableColumnNames();
    void setLogoDarkMode(bool isDarkMode);
    void setSortBy(int columnIndex);
    QString getDbColumnNameFromProgramTableColumnIndex(int columnIndex);

    ProgramTableInterface * programTable = nullptr;

    QLocale turkishLocale;
    int lastSortCol = -1;
    Qt::SortOrder lastSortOrder = Qt::AscendingOrder;
    QHeaderView * programTableHorizontalHeader = nullptr;
    QStringList yksTableColumnNames;
    QSqlDatabase db;
    TercihTuru tercihTuru = TercihTuru::NormalTercih;
};
