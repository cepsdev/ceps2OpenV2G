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
    // iso2ServiceParameterListType
    //

    template<> iso2ServiceParameterListType MessageBuilder::emit<iso2ServiceParameterListType>(ceps::ast::Struct & msg){
        iso2ServiceParameterListType r{};
        r.ParameterSet.arrayLen = read_array<iso2ParameterSetType>(msg, r.ParameterSet, string{"ParameterSet"});
        return r;
    }
    template<> node_t MessageBuilder::strct(iso2ServiceParameterListType v){
        return rec("ServiceParameterListType",
            rec("ParameterSet",v.ParameterSet)
        );    
    }    
}