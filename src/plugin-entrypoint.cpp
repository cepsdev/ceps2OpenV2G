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


//
// Plugin specific code
//


Ism4ceps_plugin_interface* ceps2openv2g::plugin_master = nullptr;


ceps::ast::node_t ceps2openv2g::plugin_entrypoint(ceps::ast::node_callparameters_t params){
    using namespace std;
    using namespace ceps::ast;
    using namespace ceps::interpreter;

    auto data = get_first_child(params);    
    if (!is<Ast_node_kind::structdef>(data)) return nullptr;
    MessageBuilder msg_builder;

    auto fill_struct = [](uint8_t* m, size_t n, ceps::ast::Struct& s){
        size_t i = 0;
        for(auto e: children(s)){
            if (i >= n) break;
            if (!is<Ast_node_kind::int_literal>(e)) continue;
            auto v = value(as_int_ref(e));
            m[i++] = v;
        }
    };

    if (name(as_struct_ref(data)).substr(0,9) == ("open_v2g_")){
        auto what = name(as_struct_ref(data)).substr(9);
        node_t result = nullptr;
        if (what == "physicalValue"){
            iso2PhysicalValueType m{};
            fill_struct((uint8_t*)&m, sizeof(m), as_struct_ref(data));
            result = msg_builder.strct(m);
        } else if (what == "AC_EVBidirectionalParameter"){
            iso2AC_EVBidirectionalParameterType m{};
            fill_struct((uint8_t*)&m, sizeof(m), as_struct_ref(data));
            result = msg_builder.strct(m);
        }
        return result;
    } else {    
        auto msg = msg_builder.build(data);
        auto result = mk_struct("open_v2g_"+name(as_struct_ref(data)));    
        for(size_t i = 0; i < msg.second; ++i){
            uint8_t ch = msg.first[i];
            children(result).push_back(mk_int_node(ch));             
        }
        return result;
    }
    return nullptr;
}

extern "C" void init_plugin(IUserdefined_function_registry* smc)
{
  ceps2openv2g::plugin_master = smc->get_plugin_interface();
  ceps2openv2g::plugin_master->reg_ceps_phase0plugin("v2g", ceps2openv2g::plugin_entrypoint);
}

