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
#include "EnumDefinitions.hpp"
#include <QStandardItemModel>
#include <optional>
#include "AcademyScopeModel.hpp"

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
    Undefined,
    Science,    //Sayısal
    Humanities, //Sözel
    EqualWeight,//Eşit Ağırlık
    Language,   //Dil
    TYT,         //TYT
};

enum class Country {
    AllCountries,
    Turkiye,
    Cyprus,
    ForeignCountries
};

enum class DegreeType {
    All,
    Bachelor, //Lisans
    Associate //Ön lisans
};

struct Interval {
    std::optional<double> minimum = 100;
    std::optional<double> maximum = 560;
};

struct SelectedQuotaTypes {
    bool regularQuota = true;
    bool martyrsAndVeteransQuota = false;
    bool earthquakeVictimsQuota = false;
    bool highSchoolValedictoriansQuota = false;
    bool women34PlusQuota = false;
    bool trncNationalsQuota = false;
    bool mtokQuota = false;
};

struct SelectedTuitionFeeTypes {
    bool free = true;
    bool discounted = true;
    bool paid = true;
};

enum class PlacementType {
    Regular,       // Normal yerleştirme
    Additional     // Ek yerleştirme
};

struct OrderParameters {
    ProgramTableColumn column;
    Qt::SortOrder direction = Qt::SortOrder::AscendingOrder;
    bool toBeOrdered = false;
};

struct AcademyScopeParameters {
    PlacementType placementType = PlacementType::Regular;
    QString universityName = "";
    QString departmentName = "";
    UniversityType universityType = UniversityType::Undefined;
    TrackType trackType = TrackType::Undefined;
    Interval scoreInterval;
    Country country;
    DegreeType degreeType = DegreeType::All;
    SelectedQuotaTypes selectedQuotaTypes;
    SelectedTuitionFeeTypes selectedTuitionFeeTypes;
    OrderParameters order;
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
    QList<University> getUniversities();
    QList<QString> getDepartments();
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

    QLocale turkishLocale;
    QStringList yksTableColumnNames;
    QSqlDatabase db;
};
