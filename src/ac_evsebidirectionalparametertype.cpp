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
    // iso2AC_EVSEBidirectionalParameterType
    //
    template<> node_t MessageBuilder::strct(iso2AC_EVSEBidirectionalParameterType v){
        return rec("AC_EVSEBidirectionalParameter",
            rec("EVSEMaximumChargeCurrent",v.EVSEMaximumChargeCurrent),
            rec("EVSENominalVoltage",v.EVSENominalVoltage),
            rec("EVSENominalFrequency",v.EVSENominalFrequency),
            rec("EVSEMaximumDischargeCurrent",v.EVSEMaximumDischargeCurrent)
        );    
    }

    template<> iso2AC_EVSEBidirectionalParameterType MessageBuilder::emit<iso2AC_EVSEBidirectionalParameterType>(ceps::ast::Struct & msg){
        iso2AC_EVSEBidirectionalParameterType r{};
        for_all_children(msg, [&](node_t e){            
            auto match_res = match_struct(e,"EVSEMaximumChargeCurrent");
            if (match_res) {
                r.EVSEMaximumChargeCurrent = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVSENominalVoltage");
            if (match_res) {
                r.EVSENominalVoltage = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVSENominalFrequency");
            if (match_res) {
                r.EVSENominalFrequency = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVSEMaximumDischargeCurrent");
            if (match_res) {
                r.EVSEMaximumDischargeCurrent = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
        });
        return r;
    }
}