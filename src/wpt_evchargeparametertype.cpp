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
    // iso2WPT_EVChargeParameterType
    //

    template<> iso2WPT_EVChargeParameterType MessageBuilder::emit<iso2WPT_EVChargeParameterType>(ceps::ast::Struct & msg){
        iso2WPT_EVChargeParameterType r{};
        for_all_children(msg, [&](node_t e){            
            auto match_res = match_struct(e,"DepartureTime");
            if (match_res) {
                auto field_value = get_numerical_field<uint32_t>(as_struct_ref(e));
                if (!field_value) return;
                r.DepartureTime = *field_value;
                return;
            }
            match_res = match_struct(e,"EVMaximumPower");
            if (match_res) {
                r.EVMaximumPower = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVMinimumPower");
            if (match_res) {
                r.EVMinimumPower = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVTargetEnergyRequest");
            if (match_res) {
                r.EVTargetEnergyRequest = emit<iso2PhysicalValueType>(as_struct_ref(e));
                r.EVTargetEnergyRequest_isUsed=1;
                return;
            }
            match_res = match_struct(e,"EVMaximumEnergyRequest");
            if (match_res) {
                r.EVMaximumEnergyRequest = emit<iso2PhysicalValueType>(as_struct_ref(e));
                r.EVMaximumEnergyRequest_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"EVMinimumEnergyRequest");
            if (match_res) {
                r.EVMinimumEnergyRequest = emit<iso2PhysicalValueType>(as_struct_ref(e));
                r.EVMinimumEnergyRequest_isUsed = 1;
                return;
            }
        });    
        return r;
    }
    template<> node_t MessageBuilder::strct(iso2WPT_EVChargeParameterType v){
        return rec("WPT_EVChargeParamterType",
            rec("DepartureTime",v.DepartureTime),
            rec("EVMaximumPower",v.EVMaximumPower),
            rec("EVMinimumPower",v.EVMinimumPower),
            v.EVTargetEnergyRequest_isUsed == 1? rec("EVTargetEnergyRequest",v.EVTargetEnergyRequest):unused,
            v.EVMaximumEnergyRequest_isUsed == 1 ? rec("EVMaximumEnergyRequest",v.EVMaximumEnergyRequest):unused,
            v.EVMinimumEnergyRequest_isUsed == 1 ? rec("EVMinimumEnergyRequest",v.EVMinimumEnergyRequest):unused
        );    
    }
}