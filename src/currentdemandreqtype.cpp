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
    // iso2CurrentDemandReqType
    //

    template<> node_t MessageBuilder::strct(iso2CurrentDemandReqType v){
        return rec("CurrentDemandReq",
            rec("EVTargetEnergyRequest",v.EVTargetEnergyRequest),
            v.EVMaximumEnergyRequest_isUsed == 1 ? rec("EVMaximumEnergyRequest",v.EVMaximumEnergyRequest):unused,
            v.EVMinimumEnergyRequest_isUsed == 1 ? rec("EVMinimumEnergyRequest",v.EVMinimumEnergyRequest):unused,
            v.DisplayParameters_isUsed == 1 ? rec("DisplayParameters",v.DisplayParameters):unused,
            rec("EVTargetCurrent",v.EVTargetCurrent),
            rec("EVTargetVoltage",v.EVTargetVoltage),
            v.EVMaximumCurrent_isUsed == 1 ? rec("EVMaximumCurrent",v.EVMaximumCurrent):unused,
            v.EVMaximumPower_isUsed == 1 ? rec("EVMaximumPower",v.EVMaximumPower):unused,
            v.EVMaximumVoltage_isUsed == 1 ? rec("EVMaximumVoltage",v.EVMaximumVoltage):unused           
        );    
    }    

    template<> iso2CurrentDemandReqType MessageBuilder::emit<iso2CurrentDemandReqType>(ceps::ast::Struct & msg){
        iso2CurrentDemandReqType r{};
        for_all_children(msg, [&](node_t e){            
            auto match_res = match_struct(e,"EVTargetEnergyRequest");
            if (match_res) {
                r.EVTargetEnergyRequest = emit<iso2PhysicalValueType>(as_struct_ref(e));
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
            match_res = match_struct(e,"DisplayParameters");
            if (match_res) {
                r.DisplayParameters = emit<iso2DisplayParametersType>(as_struct_ref(e));
                r.DisplayParameters_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"EVTargetCurrent");
            if (match_res) {
                r.EVTargetCurrent = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVTargetVoltage");
            if (match_res) {
                r.EVTargetVoltage = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVMaximumCurrent");
            if (match_res) {
                r.EVMaximumCurrent = emit<iso2PhysicalValueType>(as_struct_ref(e));
                r.EVMaximumCurrent_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"EVMaximumPower");
            if (match_res) {
                r.EVMaximumPower = emit<iso2PhysicalValueType>(as_struct_ref(e));
                r.EVMaximumPower_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"EVMaximumVoltage");
            if (match_res) {
                r.EVMaximumVoltage = emit<iso2PhysicalValueType>(as_struct_ref(e));
                r.EVMaximumVoltage_isUsed = 1;
                return;
            }
        });
        return r;
    }
}