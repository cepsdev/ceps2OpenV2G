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
    // iso2PreChargeReqType
    //

    template<> iso2PreChargeReqType MessageBuilder::emit<iso2PreChargeReqType>(ceps::ast::Struct & msg){
        iso2PreChargeReqType r{};
        for_all_children(msg, [&](node_t e){            
            auto match_res = match_struct(e,"EVTargetVoltage");
            if (match_res) {
                r.EVTargetVoltage = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVTargetCurrent");
            if (match_res) {
                r.EVTargetCurrent = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
        });
        return r;
    }
    template<> node_t MessageBuilder::strct(iso2PreChargeReqType v){
        return rec("PreChargeReqType",
            rec("EVTargetVoltage",v.EVTargetVoltage),
            rec("EVTargetCurrent",v.EVTargetCurrent)
        );    
    }
}