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
    // iso2WPT_EVSEChargeParameterType
    //

    template<> iso2WPT_EVSEChargeParameterType MessageBuilder::emit<iso2WPT_EVSEChargeParameterType>(ceps::ast::Struct & msg){
        iso2WPT_EVSEChargeParameterType r{};
        for_all_children(msg, [&](node_t e){            
            auto match_res = match_struct(e,"EVSEMaximumPower");
            if (match_res) {
                r.EVSEMaximumPower = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVSEMinimumPower");
            if (match_res) {
                r.EVSEMinimumPower = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
        });    
        return r;
    }
}