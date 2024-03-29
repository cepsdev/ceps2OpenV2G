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
// iso2DC_EVSEBidirectionalParameterType
//

    template<> iso2DC_EVSEBidirectionalParameterType MessageBuilder::emit<iso2DC_EVSEBidirectionalParameterType>(ceps::ast::Struct & msg){
        iso2DC_EVSEBidirectionalParameterType r{};
        for_all_children(msg, [&](node_t e){            
            auto match_res = match_struct(e,"EVSEMaximumChargePower");
            if (match_res) {
                r.EVSEMaximumChargePower = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVSEMaximumChargeCurrent");
            if (match_res) {
                r.EVSEMaximumChargeCurrent = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVSEMinimumChargeCurrent");
            if (match_res) {
                r.EVSEMinimumChargeCurrent = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVSEMaximumVoltage");
            if (match_res) {
                r.EVSEMaximumVoltage = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVSEMinimumVoltage");
            if (match_res) {
                r.EVSEMinimumVoltage = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVSECurrentRegulationTolerance");
            if (match_res) {
                r.EVSECurrentRegulationTolerance = emit<iso2PhysicalValueType>(as_struct_ref(e));
                r.EVSECurrentRegulationTolerance_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"EVSEPeakCurrentRipple");
            if (match_res) {
                r.EVSEPeakCurrentRipple = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVSEEnergyToBeDelivered");
            if (match_res) {
                r.EVSEEnergyToBeDelivered = emit<iso2PhysicalValueType>(as_struct_ref(e));
                r.EVSEEnergyToBeDelivered_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"EVSEMaximumDischargePower");
            if (match_res) {
                r.EVSEMaximumDischargePower = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVSEMaximumDischargeCurrent");
            if (match_res) {
                r.EVSEMaximumDischargeCurrent = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVSEMinimumDischargeCurrent");
            if (match_res) {
                r.EVSEMinimumDischargeCurrent = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
        });
        return r;
    }
     template<> node_t MessageBuilder::strct(iso2DC_EVSEBidirectionalParameterType v){
        return rec("EVSEBidirectionalParameterType",
            rec("EVSEMaximumChargePower",v.EVSEMaximumChargePower),
            rec("EVMaximumChargeCurrent",v.EVSEMaximumChargeCurrent),
            rec("EVSEMinimumChargeCurrent",v.EVSEMinimumChargeCurrent),
            rec("EVSEMaximumVoltage",v.EVSEMaximumVoltage),
            rec("EVSEMinimumVoltage",v.EVSEMinimumVoltage),
            v.EVSECurrentRegulationTolerance_isUsed == 1? rec("EVSECurrentRegulationTolerance",v.EVSECurrentRegulationTolerance):unused,
            rec("EVSEPeakCurrentRipple",v.EVSEPeakCurrentRipple),
            v.EVSEEnergyToBeDelivered_isUsed == 1 ? rec("EVSEEnergyToBeDelivered",v.EVSEEnergyToBeDelivered):unused,
            rec("EVSEMaximumDischargePower",v.EVSEMaximumDischargePower),
            rec("EVSEMaximumDischargeCurrent",v.EVSEMaximumDischargeCurrent),
            rec("EVSEMinimumDischargeCurrent",v.EVSEMinimumDischargeCurrent)
        );    
    }
}