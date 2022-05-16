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
    msg_builder.build(data);
    //cout << "generator::plugin_entrypoint:\n";

   // for(auto e : children(ceps_struct)){
   //     cout <<"\t"<< * e << "\n";
   // }
   // cout <<"\n\n";
    auto result = mk_struct("result");
    children(*result).push_back(mk_int_node(42));
    return result;
}

extern "C" void init_plugin(IUserdefined_function_registry* smc)
{
  ceps2openv2g::plugin_master = smc->get_plugin_interface();
  ceps2openv2g::plugin_master->reg_ceps_phase0plugin("v2g", ceps2openv2g::plugin_entrypoint);
}

