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
    // iso2PaymentServiceSelectionReqType
    //
    
    template<> iso2PaymentServiceSelectionReqType MessageBuilder::emit<iso2PaymentServiceSelectionReqType>(ceps::ast::Struct & msg){
        iso2PaymentServiceSelectionReqType r{};
        for_all_children(msg, [&](node_t e){
            auto match_res = match_struct(e,"SelectedPaymentOption");
            if (match_res) {
                r.SelectedPaymentOption = emit<iso2paymentOptionType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"SelectedEnergyTransferService");
            if (match_res) {
                r.SelectedEnergyTransferService = emit<iso2SelectedServiceType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"SelectedVASList");
            if (match_res) {
                r.SelectedVASList = emit<iso2SelectedServiceListType>(as_struct_ref(e));
                r.SelectedVASList_isUsed = 1;
                return;
            }
        });
        return r;
    }
    template<> node_t MessageBuilder::strct(iso2PaymentServiceSelectionReqType v){
        return rec("PaymentServiceSelectionReqType",
            rec("SelectedPaymentOption",v.SelectedPaymentOption),
            rec("SelectedEnergyTransferService",v.SelectedEnergyTransferService),
            v.SelectedVASList_isUsed == 1? rec("SelectedVASList",v.SelectedVASList):unused
        );    
    }
}