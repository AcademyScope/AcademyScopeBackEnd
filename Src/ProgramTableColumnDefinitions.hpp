/*
Program table column declarations of AcademyScope
Copyright (C) 2025 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/
#pragma once
#include <QObject>
#include <QString>
#include <QMap>

enum class ColumnTypes : int {
    Base,
    Regular,
    MartyrsAndVeterans,
    EarthquakeVictims,
    HighSchoolValedictorians,
    Women34Plus
};

enum class ProgramTableColumn : int {
    ProgramKodu = 0,
    UniversiteAdi,
    FakulteYuksekOkulAdi,
    ProgramAdi,
    PuanTuru,
    GenelKontenjan,
    GenelYerlesen,
    GenelBasariSirasi,
    GenelEnKucukPuan,
    OkulBirincisiKontenjan,
    OkulBirincisiYerlesen,
    OkulBirincisiBasariSirasi,
    OkulBirincisiEnKucukPuan,
    SehitGaziYakiniKontenjan,
    SehitGaziYakiniYerlesen,
    SehitGaziYakiniBasariSirasi,
    SehitGaziYakiniEnKucukPuan,
    DepremzedeKontenjan,
    DepremzedeYerlesen,
    DepremzedeBasariSirasi,
    DepremzedeEnKucukPuan,
    Kadin34PlusKontenjan,
    Kadin34PlusYerlesen,
    Kadin34PlusBasariSirasi,
    Kadin34PlusEnKucukPuan
};

struct ProgramTableColumnInfo {
    QString dbName;
    QString displayName;
    ProgramTableColumnInfo() = default;
    ProgramTableColumnInfo(const QString& db, const QString& display);
};

class ProgramTableColumns {
    Q_GADGET
public:
    enum Column {
        ProgramName,
        University,
        Quota
    };
    Q_ENUM(Column)

    static const QMap<ProgramTableColumn, ProgramTableColumnInfo> table;

    static ProgramTableColumnInfo operator[] (ProgramTableColumn programTableColumn);

    static QList<ProgramTableColumnInfo> getColumns();
};
