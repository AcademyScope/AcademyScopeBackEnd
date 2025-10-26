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

const QMap<ProgramTableColumn, ProgramTableColumnInfo> ProgramTableColumns::table = {
    { ProgramTableColumn::ProgramKodu, {"ProgramKodu", "Program Kodu"}},
    { ProgramTableColumn::UniversiteAdi, {"UniversiteAdi", "Üniversite"}},
    { ProgramTableColumn::FakulteYuksekOkulAdi, {"Kampus", "Fakülte/YO"}},
    { ProgramTableColumn::ProgramAdi, {"ProgramAdi", "Program"}},
    { ProgramTableColumn::PuanTuru, {"PuanTuru", "Puan Türü"}},
    { ProgramTableColumn::GenelKontenjan, {"GenelKontenjan", "Genel Kontenjan"}},
    { ProgramTableColumn::GenelYerlesen, {"GenelYerlesen", "Genel Yerleşen"}},
    { ProgramTableColumn::GenelBasariSirasi, {"GenelBasariSirasi", "Başarı Sırası"}},
    { ProgramTableColumn::GenelEnKucukPuan, {"GenelEnKucukPuan", "En Küçük Puan"}},
    { ProgramTableColumn::OkulBirincisiKontenjan, {"OkulBirincisiKontenjan", "Okul Birincisi Kontenjan"}},
    { ProgramTableColumn::OkulBirincisiYerlesen, {"OkulBirincisiYerlesen", "Okul Birincisi Yerleşen"}},
    { ProgramTableColumn::OkulBirincisiBasariSirasi, {"OkulBirincisiBasariSirasi", "Okul Birincisi Kontenjan"}},
    { ProgramTableColumn::OkulBirincisiEnKucukPuan, {"OkulBirincisiEnKucukPuan", "Okul Birincisi En Küçük Puan"}},
    { ProgramTableColumn::SehitGaziYakiniKontenjan, {"SehitGaziYakiniKontenjan", "Şehit/Gazi Yakını "}},
    { ProgramTableColumn::SehitGaziYakiniYerlesen, {"SehitGaziYakiniYerlesen", "Şehit/Gazi Yakını Yerleşen"}},
    { ProgramTableColumn::SehitGaziYakiniBasariSirasi, {"SehitGaziYakiniBasariSirasi", "Şehit/Gazi Yakını Başarı Sırası"}},
    { ProgramTableColumn::SehitGaziYakiniEnKucukPuan, {"SehitGaziYakiniEnKucukPuan", "Şehit/Gazi Yakını En Küçük Puan"}},
    { ProgramTableColumn::DepremzedeKontenjan, {"DepremzedeKontenjan", "Depremzede Kontenjan"}},
    { ProgramTableColumn::DepremzedeYerlesen, {"DepremzedeYerlesen", "Depremzede Yerleşen"}},
    { ProgramTableColumn::DepremzedeBasariSirasi, {"DepremzedeBasariSirasi", "Depremzede "}},
    { ProgramTableColumn::DepremzedeEnKucukPuan, {"DepremzedeEnKucukPuan", "Depremzede En Küçük Puan"}},
    { ProgramTableColumn::Kadin34PlusKontenjan, {"Kadin34Kontenjan", "34+ Kadın Kontenjan"}},
    { ProgramTableColumn::Kadin34PlusYerlesen, {"Kadin34Yerlesen", "34+ Kadın Yerleşen"}},
    { ProgramTableColumn::Kadin34PlusBasariSirasi, {"Kadin34BasariSirasi", "34+ Kadın Başarı Sırası"}},
    { ProgramTableColumn::Kadin34PlusEnKucukPuan, {"Kadin34EnKucukPuan", "34+ Kadın En Küçük Puan"}}
};

ProgramTableColumnInfo ProgramTableColumns::operator[](ProgramTableColumn programTableColumn) {
    return table[programTableColumn];
}

QList<ProgramTableColumnInfo> ProgramTableColumns::getColumns()
{
    return table.values();
}
