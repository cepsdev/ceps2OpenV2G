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


//
// iso2ParameterType ==> cepS
//

#include "ceps2openv2g.h"

template <> bool ceps2openv2g::MessageBuilder::filter<iso2ParameterType> (iso2ParameterType& r, node_t e){
    using T = iso2ParameterType;
    auto g1 = [&](){ r.physicalValue_isUsed = 1;};
    auto g2 = [&](){ r.boolValue_isUsed = 1;};
    auto g3 = [&](){ r.byteValue_isUsed = 1;};
    auto g4 = [&](){ r.shortValue_isUsed = 1;};
    auto g5 = [&](){ r.intValue_isUsed = 1;};
    auto g6 = [&](){ r.stringValue_isUsed = 1;};

    auto res = chain(e,this,
        fld2<T,physicalValue,decltype(r.physicalValue), decltype(g1)> {r.physicalValue, r, g1},
        fld2<T,byteValue,int8_t, decltype(g3)> {r.byteValue, r, g3},
        fld2<T,shortValue,short, decltype(g4)> {r.shortValue, r, g4},
        fld2<T,intValue,int, decltype(g5)> {r.intValue, r, g5},            
        fld<T,Name,String_t> {(String_t&)r.Name, r},
        fld2<T,boolValue,int, decltype(g2)> {r.boolValue, r, g2},
        fld2<T,stringValue,String_t, decltype(g6)> {(String_t&)r.stringValue, r, g6}
    );
    return res;
}
template<> ceps2openv2g::node_t ceps2openv2g::MessageBuilder::strct(iso2ParameterType v){
        return rec("ParameterType",
            v.physicalValue_isUsed == 1? rec("physicalValue",v.physicalValue):unused,
            v.boolValue_isUsed == 1 ? rec("boolValue",v.boolValue):unused,
            v.byteValue_isUsed == 1 ? rec("byteValue",v.byteValue):unused,
            rec("Name",v.Name),
            v.shortValue_isUsed == 1 ? rec("shortValue",v.shortValue):unused,
            v.intValue_isUsed == 1 ? rec("intValue",v.intValue):unused,
            v.stringValue_isUsed == 1 ? rec("stringValue",v.stringValue):unused
        );    
    }
