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
    //ServiceDiscoveryReq
    //

    template<> iso2ServiceDiscoveryReqType MessageBuilder::emit<iso2ServiceDiscoveryReqType>(ceps::ast::Struct & msg){
        iso2ServiceDiscoveryReqType r{};

        for_all_children(msg, [&](node_t e){
            auto match_res = match_struct(e,"SupportedServiceIDs");
            if (match_res) {
                auto n = write_bytes(   as_struct_ptr(e),
                                        r.SupportedServiceIDs.ServiceID.array, 
                                        r.SupportedServiceIDs.ServiceID.array + sizeof(r.SupportedServiceIDs.ServiceID.array));
                r.SupportedServiceIDs.ServiceID.arrayLen = n;
                r.SupportedServiceIDs_isUsed = (n != 0) ? 1 : 0;
            }
        });
        return r;
    }

}