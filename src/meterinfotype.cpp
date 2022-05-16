/*
    cepS <-> openV2G.
    Copyright (C) 2022  Tomas Prerovsky <cepsdev@hotmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "ceps2openv2g.h"

namespace ceps2openv2g{
    //
    // iso2MeterInfoType
    //
    template<> iso2MeterInfoType MessageBuilder::emit<iso2MeterInfoType>(ceps::ast::Struct & msg){
        iso2MeterInfoType r{};
        for_all_children(msg, [&](node_t e){
            auto match_res = match_struct(e,"MeterID");
            if (match_res) {
                auto field_value = get_string_field(as_struct_ref(e));
                if (!field_value) return;
                strncpy(r.MeterID.characters,field_value->c_str(), sizeof(r.MeterID.characters - 1)); 
                r.MeterID.charactersLen = field_value->length();
                return;
            }
            match_res = match_struct(e,"MeterReadingCharged");
            if (match_res) {
                auto field_value = get_numerical_field<uint64_t>(as_struct_ref(e));
                if (!field_value) return;
                r.MeterReadingCharged = *field_value;
                r.MeterReadingCharged_isUsed = 1;
                return;
            }            
            match_res = match_struct(e,"MeterReadingDischarged");
            if (match_res) {
                auto field_value = get_numerical_field<uint64_t>(as_struct_ref(e));
                if (!field_value) return;
                r.MeterReadingDischarged = *field_value;
                r.MeterReadingDischarged_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"SigMeterReading");
            if (match_res) {
                r.SigMeterReading.bytesLen = write_bytes(as_struct_ptr(e),r.SigMeterReading.bytes, 
                                                        r.SigMeterReading.bytes + sizeof(r.SigMeterReading.bytes));
                r.SigMeterReading_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"MeterStatus");
            if (match_res) {
                auto field_value = get_numerical_field<int16_t>(as_struct_ref(e));
                if (!field_value) return;
                r.MeterStatus = *field_value;
                r.MeterStatus_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"TMeter");
            if (match_res) {
                auto field_value = get_numerical_field<int64_t>(as_struct_ref(e));
                if (!field_value) return;
                r.TMeter = *field_value;
                r.TMeter_isUsed = 1;
                return;
            }
        });
        return r;
    }            
}