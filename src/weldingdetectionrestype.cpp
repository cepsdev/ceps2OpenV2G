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
    // iso2WeldingDetectionResType
    //

    template<> iso2WeldingDetectionResType MessageBuilder::emit<iso2WeldingDetectionResType>(ceps::ast::Struct & msg){
        iso2WeldingDetectionResType r{};
        evse_prolog(r,msg);
        for_all_children(msg, [&](node_t e){
            auto match_res = match_struct(e,"EVSEPresentVoltage");
            if (match_res) {
                r.EVSEPresentVoltage = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
        });        
        return r;
    }
    template<> node_t MessageBuilder::strct(iso2WeldingDetectionResType v){
        return rec("WeldingDetectionResType",
            rec("EVSEPresentVoltage",v.EVSEPresentVoltage)
        );    
    }
}