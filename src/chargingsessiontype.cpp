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
    // iso2chargingSessionType
    //

    template<> iso2chargingSessionType MessageBuilder::emit<iso2chargingSessionType>(ceps::ast::Struct & msg){
        iso2chargingSessionType r{};
        map<string,iso2chargingSessionType> str2enum { 
            {"Terminate",iso2chargingSessionType_Terminate},
            {"Pause",iso2chargingSessionType_Pause}};
        for_all_children(msg, [&] (node_t e){
            if (is<ceps::ast::Ast_node_kind::symbol>(e) && kind(as_symbol_ref(e)) == "v2gchargingSessionType" ){
                auto fit = str2enum.find(name(as_symbol_ref(e)));
                if (fit == str2enum.end()) return;
                r = fit->second;
            }
        });
        return r;
    }
}