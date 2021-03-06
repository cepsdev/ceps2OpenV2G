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
    // iso2ChargeParameterDiscoveryResType
    //

    template<> node_t MessageBuilder::strct(iso2ChargeParameterDiscoveryResType v){
        return rec("ChargeParameterDiscoveryReq",
            rec("ResponseCode",v.ResponseCode),
            v.EVSEStatus_isUsed == 1 ? rec("EVSEStatus",v.EVSEStatus):unused,
            rec("EVSEProcessing",v.EVSEProcessing),
            rec("SAScheduleList",v.SAScheduleList),            
            v.EVSEEnergyTransferParameter_isUsed == 1 ? rec("EVSEEnergyTransferParameter",v.EVSEEnergyTransferParameter):unused,
            v.AC_EVSEChargeParameter_isUsed == 1 ? rec("AC_EVSEChargeParameter",v.AC_EVSEChargeParameter):unused,
            v.AC_EVSEBidirectionalParameter_isUsed == 1 ? rec("AC_EVSEBidirectionalParameter",v.AC_EVSEBidirectionalParameter):unused,
            v.DC_EVSEChargeParameter_isUsed == 1 ? rec("DC_EVSEChargeParameter",v.DC_EVSEChargeParameter):unused,
            v.DC_EVSEBidirectionalParameter_isUsed == 1 ? rec("DC_EVSEBidirectionalParameter",v.DC_EVSEBidirectionalParameter):unused,
            v.WPT_EVSEChargeParameter_isUsed == 1 ? rec("WPT_EVSEChargeParameter",v.WPT_EVSEChargeParameter):unused
        );    
    }


    template<> iso2ChargeParameterDiscoveryResType MessageBuilder::emit<iso2ChargeParameterDiscoveryResType>(ceps::ast::Struct & msg){
        iso2ChargeParameterDiscoveryResType r{};
        evse_prolog(r,msg);
        for_all_children(msg, [&](node_t e){            
            auto match_res = match_struct(e,"EVSEProcessing");
            if (match_res) {
                r.EVSEProcessing = emit<iso2EVSEProcessingType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"SAScheduleList");
            if (match_res) {
                r.SAScheduleList = emit<iso2SAScheduleListType>(as_struct_ref(e));                
                r.SAScheduleList_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"EVSEEnergyTransferParameter");
            if (match_res) {
                r.EVSEEnergyTransferParameter = emit<iso2EVSEEnergyTransferParameterType>(as_struct_ref(e));                
                r.EVSEEnergyTransferParameter_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"AC_EVSEChargeParameter");
            if (match_res) {
                r.AC_EVSEChargeParameter = emit<iso2AC_EVSEChargeParameterType>(as_struct_ref(e));                
                r.AC_EVSEChargeParameter_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"AC_EVSEBidirectionalParameter");
            if (match_res) {
                r.AC_EVSEBidirectionalParameter = emit<iso2AC_EVSEBidirectionalParameterType>(as_struct_ref(e));                
                r.AC_EVSEBidirectionalParameter_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"DC_EVSEChargeParameter");
            if (match_res) {
                r.DC_EVSEChargeParameter = emit<iso2DC_EVSEChargeParameterType>(as_struct_ref(e));                
                r.DC_EVSEChargeParameter_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"DC_EVSEBidirectionalParameter");
            if (match_res) {
                r.DC_EVSEBidirectionalParameter = emit<iso2DC_EVSEBidirectionalParameterType>(as_struct_ref(e));                
                r.DC_EVSEBidirectionalParameter_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"WPT_EVSEChargeParameter");
            if (match_res) {
                r.WPT_EVSEChargeParameter = emit<iso2WPT_EVSEChargeParameterType>(as_struct_ref(e));                
                r.WPT_EVSEChargeParameter_isUsed = 1;
                return;
            }
        });
        return r;
    }
}