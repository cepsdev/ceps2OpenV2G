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
    // iso2ServiceDiscoveryResType
    //

    template<> iso2ServiceDiscoveryResType MessageBuilder::emit<iso2ServiceDiscoveryResType>(ceps::ast::Struct & msg){
        iso2ServiceDiscoveryResType r{};

        evse_prolog(r,msg);

        for_all_children(msg, [&](node_t e){
            auto match_res = match_struct(e,"PaymentOptionList");
            if (match_res) {

                auto n = write_bytes(   as_struct_ptr(e),
                                        r.PaymentOptionList.PaymentOption.array, 
                                        r.PaymentOptionList.PaymentOption.array + 
                                            sizeof(r.PaymentOptionList.PaymentOption.array));
                r.PaymentOptionList.PaymentOption.arrayLen = n;
                return;
            }
            match_res = match_struct(e,"EnergyTransferServiceList");
            if (match_res) {
                r.EnergyTransferServiceList = emit<iso2ServiceListType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"VASList");
            if (match_res) {
                r.VASList = emit<iso2ServiceListType>(as_struct_ref(e));
                r.VASList_isUsed = 1;
                return;
            }
        });
        return r;
    }
    template<> node_t MessageBuilder::strct(iso2ServiceDiscoveryResType v){
        return rec("ServiceDiscoveryResType",
            rec("PaymentOptionList",v.PaymentOptionList),
            rec("EnergyTransferServiceList",v.EnergyTransferServiceList),
            v.VASList_isUsed == 1? rec("VASList",v.VASList):unused
        );    
    }
}