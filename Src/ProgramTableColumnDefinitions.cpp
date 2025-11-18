/*
Program table column definitions of AcademyScope
Copyright (C) 2025 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "ProgramTableColumnDefinitions.hpp"

ProgramTableColumnInfo::ProgramTableColumnInfo(const QString& db, const QString& display)
    : dbName(db), displayName(display) {}

const QMap<ProgramTableColumn, ProgramTableColumnInfo> ProgramTableColumns::columnMap = {
    //Base Columns
    { ProgramTableColumn::ProgramKodu, ProgramTableColumnInfo("ProgramKodu", "Program Kodu")},
    { ProgramTableColumn::UniversiteAdi, ProgramTableColumnInfo("UniversiteAdi", "Üniversite")},
    { ProgramTableColumn::FakulteYuksekOkulAdi, ProgramTableColumnInfo("Kampus", "Fakülte/YO")},
    { ProgramTableColumn::ProgramAdi, ProgramTableColumnInfo("ProgramAdi", "Program")},
    { ProgramTableColumn::PuanTuru, ProgramTableColumnInfo("PuanTuru", "Puan Türü")},
    //Optional Columns
    { ProgramTableColumn::GenelKontenjan, ProgramTableColumnInfo("GenelKontenjan", "Genel Kontenjan")},
    { ProgramTableColumn::GenelYerlesen, ProgramTableColumnInfo("GenelYerlesen", "Genel Yerleşen")},
    { ProgramTableColumn::GenelBasariSirasi, ProgramTableColumnInfo("GenelBasariSirasi", "Başarı Sırası")},
    { ProgramTableColumn::GenelEnKucukPuan, ProgramTableColumnInfo("GenelEnKucukPuan", "En Küçük Puan")},
    { ProgramTableColumn::OkulBirincisiKontenjan, ProgramTableColumnInfo("OkulBirincisiKontenjan", "Okul Birincisi Kontenjan")},
    { ProgramTableColumn::OkulBirincisiYerlesen, ProgramTableColumnInfo("OkulBirincisiYerlesen", "Okul Birincisi Yerleşen")},
    { ProgramTableColumn::OkulBirincisiBasariSirasi, ProgramTableColumnInfo("OkulBirincisiBasariSirasi", "Okul Birincisi Kontenjan")},
    { ProgramTableColumn::OkulBirincisiEnKucukPuan, ProgramTableColumnInfo("OkulBirincisiEnKucukPuan", "Okul Birincisi En Küçük Puan")},
    { ProgramTableColumn::SehitGaziYakiniKontenjan, ProgramTableColumnInfo("SehitGaziYakiniKontenjan", "Şehit/Gazi Yakını ")},
    { ProgramTableColumn::SehitGaziYakiniYerlesen, ProgramTableColumnInfo("SehitGaziYakiniYerlesen", "Şehit/Gazi Yakını Yerleşen")},
    { ProgramTableColumn::SehitGaziYakiniBasariSirasi, ProgramTableColumnInfo("SehitGaziYakiniBasariSirasi", "Şehit/Gazi Yakını Başarı Sırası")},
    { ProgramTableColumn::SehitGaziYakiniEnKucukPuan, ProgramTableColumnInfo("SehitGaziYakiniEnKucukPuan", "Şehit/Gazi Yakını En Küçük Puan")},
    { ProgramTableColumn::DepremzedeKontenjan, ProgramTableColumnInfo("DepremzedeKontenjan", "Depremzede Kontenjan")},
    { ProgramTableColumn::DepremzedeYerlesen, ProgramTableColumnInfo("DepremzedeYerlesen", "Depremzede Yerleşen")},
    { ProgramTableColumn::DepremzedeBasariSirasi, ProgramTableColumnInfo("DepremzedeBasariSirasi", "Depremzede ")},
    { ProgramTableColumn::DepremzedeEnKucukPuan, ProgramTableColumnInfo("DepremzedeEnKucukPuan", "Depremzede En Küçük Puan")},
    { ProgramTableColumn::Kadin34PlusKontenjan, ProgramTableColumnInfo("Kadin34Kontenjan", "34+ Kadın Kontenjan")},
    { ProgramTableColumn::Kadin34PlusYerlesen, ProgramTableColumnInfo("Kadin34Yerlesen", "34+ Kadın Yerleşen")},
    { ProgramTableColumn::Kadin34PlusBasariSirasi, ProgramTableColumnInfo("Kadin34BasariSirasi", "34+ Kadın Başarı Sırası")},
    { ProgramTableColumn::Kadin34PlusEnKucukPuan, ProgramTableColumnInfo("Kadin34EnKucukPuan", "34+ Kadın En Küçük Puan")}
};

const QMap<ProgramTableColumn, ProgramTableColumnInfo> ProgramTableColumns::baseColumnMap = {
    //Base Columns
    { ProgramTableColumn::ProgramKodu, ProgramTableColumnInfo("ProgramKodu", "Program Kodu")},
    { ProgramTableColumn::UniversiteAdi, ProgramTableColumnInfo("UniversiteAdi", "Üniversite")},
    { ProgramTableColumn::FakulteYuksekOkulAdi, ProgramTableColumnInfo("Kampus", "Fakülte/YO")},
    { ProgramTableColumn::ProgramAdi, ProgramTableColumnInfo("ProgramAdi", "Program")},
    { ProgramTableColumn::PuanTuru, ProgramTableColumnInfo("PuanTuru", "Puan Türü")},
};

const QMap<ProgramTableColumn, ProgramTableColumnInfo> ProgramTableColumns::optionalColumnMap = {
    //Optional Columns
    { ProgramTableColumn::GenelKontenjan, ProgramTableColumnInfo("GenelKontenjan", "Genel Kontenjan")},
    { ProgramTableColumn::GenelYerlesen, ProgramTableColumnInfo("GenelYerlesen", "Genel Yerleşen")},
    { ProgramTableColumn::GenelBasariSirasi, ProgramTableColumnInfo("GenelBasariSirasi", "Başarı Sırası")},
    { ProgramTableColumn::GenelEnKucukPuan, ProgramTableColumnInfo("GenelEnKucukPuan", "En Küçük Puan")},
    { ProgramTableColumn::OkulBirincisiKontenjan, ProgramTableColumnInfo("OkulBirincisiKontenjan", "Okul Birincisi Kontenjan")},
    { ProgramTableColumn::OkulBirincisiYerlesen, ProgramTableColumnInfo("OkulBirincisiYerlesen", "Okul Birincisi Yerleşen")},
    { ProgramTableColumn::OkulBirincisiBasariSirasi, ProgramTableColumnInfo("OkulBirincisiBasariSirasi", "Okul Birincisi Kontenjan")},
    { ProgramTableColumn::OkulBirincisiEnKucukPuan, ProgramTableColumnInfo("OkulBirincisiEnKucukPuan", "Okul Birincisi En Küçük Puan")},
    { ProgramTableColumn::SehitGaziYakiniKontenjan, ProgramTableColumnInfo("SehitGaziYakiniKontenjan", "Şehit/Gazi Yakını ")},
    { ProgramTableColumn::SehitGaziYakiniYerlesen, ProgramTableColumnInfo("SehitGaziYakiniYerlesen", "Şehit/Gazi Yakını Yerleşen")},
    { ProgramTableColumn::SehitGaziYakiniBasariSirasi, ProgramTableColumnInfo("SehitGaziYakiniBasariSirasi", "Şehit/Gazi Yakını Başarı Sırası")},
    { ProgramTableColumn::SehitGaziYakiniEnKucukPuan, ProgramTableColumnInfo("SehitGaziYakiniEnKucukPuan", "Şehit/Gazi Yakını En Küçük Puan")},
    { ProgramTableColumn::DepremzedeKontenjan, ProgramTableColumnInfo("DepremzedeKontenjan", "Depremzede Kontenjan")},
    { ProgramTableColumn::DepremzedeYerlesen, ProgramTableColumnInfo("DepremzedeYerlesen", "Depremzede Yerleşen")},
    { ProgramTableColumn::DepremzedeBasariSirasi, ProgramTableColumnInfo("DepremzedeBasariSirasi", "Depremzede ")},
    { ProgramTableColumn::DepremzedeEnKucukPuan, ProgramTableColumnInfo("DepremzedeEnKucukPuan", "Depremzede En Küçük Puan")},
    { ProgramTableColumn::Kadin34PlusKontenjan, ProgramTableColumnInfo("Kadin34Kontenjan", "34+ Kadın Kontenjan")},
    { ProgramTableColumn::Kadin34PlusYerlesen, ProgramTableColumnInfo("Kadin34Yerlesen", "34+ Kadın Yerleşen")},
    { ProgramTableColumn::Kadin34PlusBasariSirasi, ProgramTableColumnInfo("Kadin34BasariSirasi", "34+ Kadın Başarı Sırası")},
    { ProgramTableColumn::Kadin34PlusEnKucukPuan, ProgramTableColumnInfo("Kadin34EnKucukPuan", "34+ Kadın En Küçük Puan")}
};

const ProgramTableColumnInfo & ProgramTableColumns::operator[](ProgramTableColumn programTableColumn) {
    return columnMap[programTableColumn];
}

QList<ProgramTableColumnInfo> ProgramTableColumns::getColumns()
{
    return columnMap.values();
}

QMap<ProgramTableColumn, ProgramTableColumnInfo> ProgramTableColumns::getColumnMap()
{
    return columnMap;
}
