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
            v.MaxSupportingPoints_isUsed == 1 ? rec("MaxSupportingPoints",v.MaxSupportingPoints):unused,
            rec("EVEnergyTransferParameter",v.EVEnergyTransferParameter),
            v.AC_EVChargeParameter_isUsed == 1 ? rec("AC_EVChargeParameter",v.AC_EVChargeParameter):unused,
            v.AC_EVBidirectionalParameter_isUsed == 1 ? rec("AC_EVBidirectionalParameter",v.AC_EVBidirectionalParameter):unused,
            v.DC_EVChargeParameter_isUsed == 1 ? rec("DC_EVChargeParameter",v.DC_EVChargeParameter):unused,
            v.DC_EVBidirectionalParameter_isUsed == 1 ? rec("DC_EVBidirectionalParameter",v.DC_EVBidirectionalParameter):unused,
            v.WPT_EVChargeParameter_isUsed == 1 ? rec("iWPT_EVChargeParameter",v.WPT_EVChargeParameter):unused,
            v.MinimumPMaxRequest_isUsed == 1 ? rec("MinimumPMaxRequest",v.MinimumPMaxRequest):unused
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