/*
Data type definitions of AcademyScope
Copyright (C) 2025 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/
#pragma once
#include <QObject>
#include <QString>
#include <QMap>

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

enum class ProgramTableColumn : int;

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

struct DataWindow {
    int windowSize = 300;
    int beginningIndex = 0;
    int endingIndex = 0;
    int tableRowCount = 0;
    int columnCount = 0;
};
