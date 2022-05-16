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
    // appHandAnonType_supportedAppProtocolReq
    //

    template<> appHandAnonType_supportedAppProtocolReq MessageBuilder::emit<appHandAnonType_supportedAppProtocolReq>(ceps::ast::Struct & msg){
        appHandAnonType_supportedAppProtocolReq r{};
        r.AppProtocol.arrayLen = 
            read_array<appHandAppProtocolType>(msg, r.AppProtocol, string{"AppProtocol"});          
        return r;
    }
}