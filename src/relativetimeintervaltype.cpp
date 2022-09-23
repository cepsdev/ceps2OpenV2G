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
    // iso2RelativeTimeIntervalType
    //

    template<> iso2RelativeTimeIntervalType MessageBuilder::emit<iso2RelativeTimeIntervalType>(ceps::ast::Struct & msg){
        iso2RelativeTimeIntervalType r{};
        for_all_children(msg, [&](node_t e){            
            auto match_res = match_struct(e,"start");
            if (match_res) {
                auto field_value = get_numerical_field<uint32_t>(as_struct_ref(e));
                if (!field_value) return;
                r.start = *field_value;
                return;
            } 
            match_res = match_struct(e,"duration");
            if (match_res) {
                auto field_value = get_numerical_field<uint32_t>(as_struct_ref(e));
                if (!field_value) return;
                r.duration = *field_value;
                r.duration_isUsed = 1;
                return;
            }
        });
        return r;
    }
    template<> node_t MessageBuilder::strct(iso2RelativeTimeIntervalType v){
        return rec("RelativeTimeIntervalType",
            rec("Start",v.start),
            v.duration_isUsed == 1? rec("duration",v.duration):unused
        );    
    }
}