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
    // iso2PhysicalValueType
    //

    template <> bool MessageBuilder::filter<iso2PhysicalValueType> (iso2PhysicalValueType& r, node_t e){
        using T = iso2PhysicalValueType;
        auto res = chain(e,this,
            fld<T,Exponent,decltype(r.Exponent)> {r.Exponent, r},fld<T,Value,decltype(r.Value)> {r.Value, r}
        );
        return res;
    }

    template<> node_t MessageBuilder::strct(iso2PhysicalValueType v){
        using namespace ceps::interpreter;
        auto r = mk_struct("physicalValue");
        auto e = mk_struct("Exponent");
        children(*e).push_back(mk_int_node(v.Exponent));
        children(*r).push_back(e);
        e = mk_struct("Value");
        children(*e).push_back(mk_int_node(v.Value));
        children(*r).push_back(e); 
        return r;
    }
}