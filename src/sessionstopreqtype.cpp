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
    // iso2SessionStopReqType
    //

    template<> iso2SessionStopReqType MessageBuilder::emit<iso2SessionStopReqType>(ceps::ast::Struct & msg){
        iso2SessionStopReqType r{};
        for_all_children(msg, [&](node_t e){
            auto match_res = match_struct(e,"ChargingSession");
            if (match_res) {
                r.ChargingSession = emit<iso2chargingSessionType>(as_struct_ref(e));
                return;
            }
        });        
        return r;
    }
    template<> node_t MessageBuilder::strct(iso2SessionStopReqType v){
        return rec("SessionStopReqType",
            rec("ChargingSession",v.ChargingSession)
        );    
    }
}