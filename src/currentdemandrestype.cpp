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
    // iso2CurrentDemandResType
    //

    template<> node_t MessageBuilder::strct(iso2CurrentDemandResType v){
        return rec("CurrentDemandRes",

            rec("ResponseCode",v.ResponseCode),
            rec("EVSEPresentCurrent",v.EVSEPresentCurrent),
            rec("EVSEPresentVoltage",v.EVSEPresentVoltage),
            rec("EVSEPowerLimitAchieved",v.EVSEPowerLimitAchieved),
            rec("EVSECurrentLimitAchieved",v.EVSECurrentLimitAchieved),
            rec("EVSEVoltageLimitAchieved",v.EVSEVoltageLimitAchieved),
            v.EVSEMaximumPower_isUsed == 1 ? rec("EVSEMaximumPower",v.EVSEMaximumPower):unused,
            v.EVSEMaximumCurrent_isUsed == 1 ? rec("EVSEMaximumCurrent",v.EVSEMaximumCurrent):unused,
            v.EVSEMaximumVoltage_isUsed == 1 ? rec("EVSEMaximumVoltage",v.EVSEMaximumVoltage):unused,
            rec("EVSEID",v.EVSEID),
            v.SAScheduleTupleID_isUsed == 1 ? rec("SAScheduleTupleID",v.SAScheduleTupleID):unused,
            v.MeterInfo_isUsed == 1 ? rec("MeterInfo",v.MeterInfo):unused,
            v.ReceiptRequired_isUsed == 1 ? rec("ReceiptRequired",v.ReceiptRequired_isUsed):unused
        );    
    }    


    template<> iso2CurrentDemandResType MessageBuilder::emit<iso2CurrentDemandResType>(ceps::ast::Struct & msg){
        iso2CurrentDemandResType r{};
        evse_prolog(r,msg);
        for_all_children(msg, [&](node_t e){            
            auto match_res = match_struct(e,"EVSEPresentCurrent");
            if (match_res) {
                r.EVSEPresentCurrent = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVSEPresentVoltage");
            if (match_res) {
                r.EVSEPresentVoltage = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVSEPowerLimitAchieved");
            if (match_res) {
                auto field_value = get_numerical_field<int>(as_struct_ref(e));
                if (!field_value) return;
                r.EVSEPowerLimitAchieved = *field_value;
                return;
            }            
            match_res = match_struct(e,"EVSECurrentLimitAchieved");
            if (match_res) {
                auto field_value = get_numerical_field<int>(as_struct_ref(e));
                if (!field_value) return;
                r.EVSECurrentLimitAchieved = *field_value;
                return;
            }            
            match_res = match_struct(e,"EVSEVoltageLimitAchieved");
            if (match_res) {
                auto field_value = get_numerical_field<int>(as_struct_ref(e));
                if (!field_value) return;
                r.EVSEVoltageLimitAchieved = *field_value;
                return;
            } 
            match_res = match_struct(e,"EVSEMaximumPower");
            if (match_res) {
                r.EVSEMaximumPower = emit<iso2PhysicalValueType>(as_struct_ref(e));
                r.EVSEMaximumPower_isUsed = 1;
                return;
            }         
            match_res = match_struct(e,"EVSEMaximumCurrent");
            if (match_res) {
                r.EVSEMaximumCurrent = emit<iso2PhysicalValueType>(as_struct_ref(e));
                r.EVSEMaximumCurrent_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"EVSEMaximumVoltage");
            if (match_res) {
                r.EVSEMaximumVoltage = emit<iso2PhysicalValueType>(as_struct_ref(e));
                r.EVSEMaximumVoltage_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"EVSEID");
            if (match_res) {
                auto field_value = get_string_field(as_struct_ref(e));
                if (!field_value) return;
                strncpy(r.EVSEID.characters,field_value->c_str(), sizeof(r.EVSEID.characters - 1)); 
                r.EVSEID.charactersLen = field_value->length();
                return;
            }
            match_res = match_struct(e,"SAScheduleTupleID");
            if (match_res) {
                auto field_value = get_numerical_field<uint8_t>(as_struct_ref(e));
                if (!field_value) return;
                r.SAScheduleTupleID = *field_value;
                r.SAScheduleTupleID_isUsed = 1;
                return;
            } 
            match_res = match_struct(e,"MeterInfo");
            if (match_res) {
                r.MeterInfo = emit<iso2MeterInfoType>(as_struct_ref(e));
                r.MeterInfo_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"ReceiptRequired");
            if (match_res) {
                auto field_value = get_numerical_field<int32_t>(as_struct_ref(e));
                if (!field_value) return;
                r.ReceiptRequired = *field_value;
                r.ReceiptRequired_isUsed = 1;
                return;
            }
        });
        return r;
    }
}