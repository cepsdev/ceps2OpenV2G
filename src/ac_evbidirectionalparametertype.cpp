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
    // iso2AC_EVBidirectionalParameterType
    //

    template<> iso2AC_EVBidirectionalParameterType MessageBuilder::emit<iso2AC_EVBidirectionalParameterType>(ceps::ast::Struct & msg){
        iso2AC_EVBidirectionalParameterType r{};
        for_all_children(msg, [&](node_t e){            
            auto match_res = match_struct(e,"DepartureTime");
            if (match_res) {
                auto field_value = get_numerical_field<uint32_t>(as_struct_ref(e));
                if (!field_value) return;
                r.DepartureTime = *field_value;
                return;
            }
            match_res = match_struct(e,"EVMaximumChargePower");
            if (match_res) {
                r.EVMaximumChargePower = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVMaximumChargeCurrent");
            if (match_res) {
                r.EVMaximumChargeCurrent = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVMinimumChargeCurrent");
            if (match_res) {
                r.EVMinimumChargeCurrent = emit<iso2PhysicalValueType>(as_struct_ref(e));
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
            match_res = match_struct(e,"EVMaximumVoltage");
            if (match_res) {
                r.EVMaximumVoltage = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }

            match_res = match_struct(e,"EVMaximumDischargePower");
            if (match_res) {
                r.EVMaximumDischargePower = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVMaximumDischargeCurrent");
            if (match_res) {
                r.EVMaximumDischargeCurrent = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVMinimumDischargeCurrent");
            if (match_res) {
                r.EVMinimumDischargeCurrent = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }

        });    
        return r;
    }
    template<> node_t MessageBuilder::strct(iso2AC_EVBidirectionalParameterType v){
        return rec("AC_EVBidirectionalParameter",
            rec("DepartureTime",v.DepartureTime),
            rec("EVMaximumChargePower",v.EVMaximumChargePower),
            rec("EVMaximumChargeCurrent",v.EVMaximumChargeCurrent),
            v.EVTargetEnergyRequest_isUsed == 1? rec("EVTargetEnergyRequest",v.EVTargetEnergyRequest):unused,
            v.EVMaximumEnergyRequest_isUsed == 1 ? rec("EVMaximumEnergyRequest",v.EVMaximumEnergyRequest):unused,
            v.EVMinimumEnergyRequest_isUsed == 1 ? rec("EVMinimumEnergyRequest",v.EVMinimumEnergyRequest):unused,
            rec("EVMaximumVoltage",v.EVMaximumVoltage),
            rec("EVMaximumDischargePower",v.EVMaximumDischargePower),
            rec("EVMaximumDischargeCurrent",v.EVMaximumDischargeCurrent),
            rec("EVMinimumDischargeCurrent",v.EVMinimumDischargeCurrent)
        );    
    }
}