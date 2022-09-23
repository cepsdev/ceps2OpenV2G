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
    // iso2DisplayParametersType
    //

    template<> iso2DisplayParametersType MessageBuilder::emit<iso2DisplayParametersType>(ceps::ast::Struct & msg){
        iso2DisplayParametersType r{};
        for_all_children(msg, [&](node_t e){            
            auto match_res = match_struct(e,"CurrentRange");
            if (match_res) {
                auto field_value = get_numerical_field<uint16_t>(as_struct_ref(e));
                if (!field_value) return;
                r.CurrentRange = *field_value;
                r.CurrentRange_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"CurrentSOC");
            if (match_res) {
                auto field_value = get_numerical_field<int8_t>(as_struct_ref(e));
                if (!field_value) return;
                r.CurrentSOC = *field_value;
                r.CurrentSOC_isUsed = 1;
                return;
            }            
            match_res = match_struct(e,"TargetSOC");
            if (match_res) {
                auto field_value = get_numerical_field<int8_t>(as_struct_ref(e));
                if (!field_value) return;
                r.TargetSOC = *field_value;
                r.TargetSOC_isUsed = 1;
                return;
            }            
            match_res = match_struct(e,"BulkSOC");
            if (match_res) {
                auto field_value = get_numerical_field<int8_t>(as_struct_ref(e));
                if (!field_value) return;
                r.BulkSOC = *field_value;
                r.BulkSOC_isUsed = 1;
                return;
            }            
            match_res = match_struct(e,"MinimumSOC");
            if (match_res) {
                auto field_value = get_numerical_field<int8_t>(as_struct_ref(e));
                if (!field_value) return;
                r.MinimumSOC = *field_value;
                r.MinimumSOC_isUsed = 1;
                return;
            }            
            match_res = match_struct(e,"ChargingPerformance");
            if (match_res) {
                r.ChargingPerformance = emit<iso2PhysicalValueType>(as_struct_ref(e));
                r.ChargingPerformance_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"RemainingTimeToTargetSOC");
            if (match_res) {
                auto field_value = get_numerical_field<int8_t>(as_struct_ref(e));
                if (!field_value) return;
                r.RemainingTimeToTargetSOC = *field_value;
                r.RemainingTimeToTargetSOC_isUsed = 1;
                return;
            }            
            match_res = match_struct(e,"RemainingTimeToBulkSOC");
            if (match_res) {
                auto field_value = get_numerical_field<int8_t>(as_struct_ref(e));
                if (!field_value) return;
                r.RemainingTimeToBulkSOC = *field_value;
                r.RemainingTimeToBulkSOC_isUsed = 1;
                return;
            }            
            match_res = match_struct(e,"RemainingTimeToMinimumSOC");
            if (match_res) {
                auto field_value = get_numerical_field<int8_t>(as_struct_ref(e));
                if (!field_value) return;
                r.RemainingTimeToMinimumSOC = *field_value;
                r.RemainingTimeToMinimumSOC_isUsed = 1;
                return;
            }            
            match_res = match_struct(e,"ChargingComplete");
            if (match_res) {
                auto field_value = get_numerical_field<int>(as_struct_ref(e));
                if (!field_value) return;
                r.ChargingComplete = *field_value;
                r.ChargingComplete_isUsed = 1;
                return;
            }            
            match_res = match_struct(e,"BulkChargingComplete");
            if (match_res) {
                auto field_value = get_numerical_field<int>(as_struct_ref(e));
                if (!field_value) return;
                r.BulkChargingComplete = *field_value;
                r.BulkChargingComplete_isUsed = 1;
                return;
            }            
            match_res = match_struct(e,"InletHot");
            if (match_res) {
                auto field_value = get_numerical_field<int>(as_struct_ref(e));
                if (!field_value) return;
                r.InletHot = *field_value;
                r.InletHot_isUsed = 1;
                return;
            }            
        });
        return r;
    }
     template<> node_t MessageBuilder::strct(iso2DisplayParametersType v){
        return rec("DisplayParametersType",
            v.CurrentRange_isUsed == 1? rec("CurrentRange",v.CurrentRange):unused,
            v.CurrentSOC_isUsed == 1 ? rec("CurrentSOC",v.CurrentSOC):unused,
            v.TargetSOC_isUsed == 1 ? rec("TargetSOC",v.TargetSOC):unused,
            v.BulkSOC_isUsed == 1 ? rec("BulkSOC",v.BulkSOC):unused,
            v.MinimumSOC_isUsed == 1 ? rec("MinimumSOC",v.MinimumSOC):unused,
            v.ChargingPerformance_isUsed == 1 ? rec("ChargingPerformance",v.ChargingPerformance):unused,
            v.RemainingTimeToTargetSOC_isUsed == 1 ? rec("RemainingTimeToTargetSOC",v.RemainingTimeToTargetSOC):unused,
            v.RemainingTimeToBulkSOC_isUsed == 1 ? rec("RemainingTimeToBulkSOC",v.RemainingTimeToBulkSOC):unused,
            v.RemainingTimeToMinimumSOC_isUsed == 1 ? rec("RemainingTimeToMinimumSOC",v.RemainingTimeToMinimumSOC):unused,
            v.ChargingComplete_isUsed == 1 ? rec("ChargingComplete",v.ChargingComplete):unused,
            v.BulkChargingComplete_isUsed == 1 ? rec("BulkChargingComplete",v.BulkChargingComplete):unused,
            v.InletHot_isUsed == 1 ? rec("InletHot",v.InletHot):unused
        );    
    }
}