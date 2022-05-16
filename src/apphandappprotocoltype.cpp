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
    // appHandAppProtocolType
    //

    template<> appHandAppProtocolType MessageBuilder::emit<appHandAppProtocolType>(ceps::ast::Struct & msg){
        appHandAppProtocolType r{};
        for_all_children(msg, [&](node_t e){
            auto match_res = match_struct(e,"ProtocolNamespace");
            if (match_res) {
                auto field_value = get_string_field(as_struct_ref(e));
                if (!field_value) return;
                strncpy(r.ProtocolNamespace.characters,field_value->c_str(), sizeof(r.ProtocolNamespace.characters - 1)); 
                r.ProtocolNamespace.charactersLen = field_value->length();
                return;
            }
            match_res = match_struct(e,"VersionNumberMajor");
            if (match_res) {
                auto field_value = get_numerical_field<uint32_t>(as_struct_ref(e));
                if (!field_value) return;
                r.VersionNumberMajor = *field_value;
                return;
            }
            match_res = match_struct(e,"VersionNumberMinor");
            if (match_res) {
                auto field_value = get_numerical_field<uint32_t>(as_struct_ref(e));
                if (!field_value) return;
                r.VersionNumberMinor = *field_value;
                return;
            }
            match_res = match_struct(e,"SchemaID");
            if (match_res) {
                auto field_value = get_numerical_field<uint8_t>(as_struct_ref(e));
                if (!field_value) return;
                r.SchemaID = *field_value;
                return;
            }
            match_res = match_struct(e,"Priority");
            if (match_res) {
                auto field_value = get_numerical_field<uint8_t>(as_struct_ref(e));
                if (!field_value) return;
                r.Priority = *field_value;
                return;
            }
        });        
        return r;
    }
}