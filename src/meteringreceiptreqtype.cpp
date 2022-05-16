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
    // MeteringReceiptReq
    //

    template<> iso2MeteringReceiptReqType MessageBuilder::emit<iso2MeteringReceiptReqType>(ceps::ast::Struct & msg){
        iso2MeteringReceiptReqType r{};
        for_all_children(msg, [&](node_t e){
            auto match_res = match_struct(e,"Id");
            if (match_res) {
                auto field_value = get_string_field(as_struct_ref(e));
                if (!field_value) return;
                strncpy(r.Id.characters,field_value->c_str(), sizeof(r.Id.characters - 1)); 
                r.Id.charactersLen = field_value->length();
                return;
            }
            match_res = match_struct(e,"SessionID");
            if (match_res) {
                r.SessionID.bytesLen = write_bytes(as_struct_ptr(e),r.SessionID.bytes, 
                                                        r.SessionID.bytes + sizeof(r.SessionID.bytes));
                return;
            }
            match_res = match_struct(e,"SAScheduleTupleID");
            if (match_res) {
                auto field_value = get_numerical_field<uint8_t>(as_struct_ref(e));
                if (!field_value) return;
                r.SAScheduleTupleID = *field_value;
                r.SAScheduleTupleID_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"MeterInfo");
            if (match_res) {
                r.MeterInfo = emit<iso2MeterInfoType>(as_struct_ref(e));
                return;
            }
        });
        return r;
    }
}