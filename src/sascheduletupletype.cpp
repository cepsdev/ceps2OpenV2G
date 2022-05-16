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
    // iso2SAScheduleTupleType
    //


    template<> iso2SAScheduleTupleType MessageBuilder::emit<iso2SAScheduleTupleType>(ceps::ast::Struct & msg){
        iso2SAScheduleTupleType r{};
        for_all_children(msg, [&](node_t e){            
            auto match_res = match_struct(e,"SAScheduleTupleID");
            if (match_res) {
                auto field_value = get_numerical_field<uint8_t>(as_struct_ref(e));
                if (!field_value) return;
                r.SAScheduleTupleID = *field_value;
                return;
            }    
            match_res = match_struct(e,"PMaxSchedule");
            if (match_res) {
                r.PMaxSchedule = emit<iso2PMaxScheduleType>(as_struct_ref(e));                
                return;
            }
            match_res = match_struct(e,"PMaxDischargeSchedule");
            if (match_res) {
                r.PMaxDischargeSchedule = emit<iso2PMaxScheduleType>(as_struct_ref(e));
                r.PMaxDischargeSchedule_isUsed = 1;               
                return;
            }
            match_res = match_struct(e,"SalesTariff");
            if (match_res) {
                r.SalesTariff = emit<iso2SalesTariffType>(as_struct_ref(e));
                r.SalesTariff_isUsed = 1;               
                return;
            }
            match_res = match_struct(e,"BuyBackTariff");
            if (match_res) {
                r.BuyBackTariff = emit<iso2SalesTariffType>(as_struct_ref(e));
                r.BuyBackTariff_isUsed = 1;               
                return;
            }            
        });
        return r;
    }
}