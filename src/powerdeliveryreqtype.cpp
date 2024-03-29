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
    // iso2PowerDeliveryReqType
    //

    template<> iso2PowerDeliveryReqType MessageBuilder::emit<iso2PowerDeliveryReqType>(ceps::ast::Struct & msg){
        iso2PowerDeliveryReqType r{};
        for_all_children(msg, [&](node_t e){            
            auto match_res = match_struct(e,"ChargeProgress");
            if (match_res) {
                r.ChargeProgress = emit<iso2chargeProgressType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVOperation");
            if (match_res) {
                r.EVOperation_isUsed = 1;
                r.EVOperation = emit<iso2EVOperationType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"SAScheduleTupleID");
            if (match_res) {
                auto field_value = get_numerical_field<uint8_t>(as_struct_ref(e));
                if (!field_value) return;
                r.SAScheduleTupleID = *field_value;
                return;
            }    
            match_res = match_struct(e,"ChargingProfile");
            if (match_res) {
                r.ChargingProfile_isUsed = 1;
                r.ChargingProfile = emit<iso2ChargingProfileType>(as_struct_ref(e));
                return;
            }
        });
        return r;
    }
    template<> node_t MessageBuilder::strct(iso2PowerDeliveryReqType v){
        return rec("PowerDeliveryReqType",
            rec("ChargeProgress",v.ChargeProgress),
            v.EVOperation_isUsed == 1? rec("EVOperation",v.EVOperation):unused,
            rec("SAScheduleTupleID",v.SAScheduleTupleID),
            v.ChargingProfile_isUsed == 1 ? rec("ChargingProfile",v.ChargingProfile):unused
        );    
    }
}