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
    // iso2ParameterSetType
    //
    template<> iso2ParameterSetType MessageBuilder::emit<iso2ParameterSetType>(ceps::ast::Struct & msg){
        iso2ParameterSetType r{};
        r.Parameter.arrayLen = read_array<iso2ParameterType>(msg, r.Parameter, string{"Parameter"});
        for_all_children(msg, [&](node_t e){
            auto match_res = match_struct(e,"ParameterSetID");
            if (match_res) {
                auto field_value = get_numerical_field<int16_t>(as_struct_ref(e));
                if (!field_value) return;
                r.ParameterSetID = *field_value;
                return;
            }            
        });
        return r;
    }
    template<> node_t MessageBuilder::strct(iso2ParameterSetType v){
        return rec("ParameterSetType",
            rec("Parameter",v.Parameter),
            rec("ParameterSetID",v.ParameterSetID)
        );
    }       
}