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
    // iso2ChargeParameterDiscoveryReqType
    //

    template<> iso2ChargeParameterDiscoveryReqType MessageBuilder::emit<iso2ChargeParameterDiscoveryReqType>(ceps::ast::Struct & msg){
        iso2ChargeParameterDiscoveryReqType r{};
        for_all_children(msg, [&](node_t e){            
            auto match_res = match_struct(e,"MaxSupportingPoints");
            if (match_res) {
                auto field_value = get_numerical_field<uint16_t>(as_struct_ref(e));
                if (!field_value) return;
                r.MaxSupportingPoints = *field_value;
                r.MaxSupportingPoints_isUsed = 1;
                return;
            }    
            match_res = match_struct(e,"EVEnergyTransferParameter");
            if (match_res) {
                r.EVEnergyTransferParameter = 
                                            emit<iso2EVEnergyTransferParameterType>(as_struct_ref(e));                
                r.EVEnergyTransferParameter_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"AC_EVChargeParameter");
            if (match_res) {
                r.AC_EVChargeParameter = emit<iso2AC_EVChargeParameterType>(as_struct_ref(e));
                r.AC_EVChargeParameter_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"AC_EVBidirectionalParameter");
            if (match_res) {
                r.AC_EVBidirectionalParameter = emit<iso2AC_EVBidirectionalParameterType>(as_struct_ref(e));
                r.AC_EVBidirectionalParameter_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"DC_EVChargeParameter");
            if (match_res) {
                r.DC_EVChargeParameter_isUsed = 1;
                r.DC_EVChargeParameter = emit<iso2DC_EVChargeParameterType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"DC_EVBidirectionalParameter");
            if (match_res) {
                r.DC_EVBidirectionalParameter = emit<iso2DC_EVBidirectionalParameterType>(as_struct_ref(e));
                r.DC_EVBidirectionalParameter_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"WPT_EVChargeParameter");
            if (match_res) {
                r.WPT_EVChargeParameter_isUsed = 1;
                r.WPT_EVChargeParameter = emit<iso2WPT_EVChargeParameterType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"MinimumPMaxRequest");
            if (match_res) {
                r.MinimumPMaxRequest = emit<iso2MinimumPMaxRequestType>(as_struct_ref(e));
                r.MinimumPMaxRequest_isUsed = 1;
                return;
            }
        });
        return r;
    }
}