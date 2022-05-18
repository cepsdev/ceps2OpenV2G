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

#include <stdlib.h>
#include <iostream>
#include <ctype.h>
#include <chrono>
#include <sstream>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <queue>
#include <unordered_map>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdlib.h>
#include <map>
#include <algorithm>
#include <future>
#include <netinet/sctp.h> 
#include "ceps_ast.hh"
#include "core/include/state_machine_simulation_core.hpp"
#include "iso2EXIDatatypes.h" 
#include "appHandEXIDatatypes.h" 

template<typename Iter>
size_t write_bytes(ceps::ast::Struct_ptr strct, Iter beg, Iter end){
  using namespace ceps::ast;
  auto sit = strct->children().begin();
  size_t written = 0;
  for(auto it = beg; it != end && sit != strct->children().end();++it,++sit){
    if (is<ceps::ast::Ast_node_kind::int_literal>(*sit)){
      *it = value(as_int_ref(*sit));
      ++written;
    }
    else if (is<ceps::ast::Ast_node_kind::long_literal>(*sit)){
      *it = value(as_int64_ref(*sit));
      ++written;
    }
    else if (is<ceps::ast::Ast_node_kind::float_literal>(*sit)){
      *it = value(as_double_ref(*sit));
      ++written;
    }
    else if (is<ceps::ast::Ast_node_kind::string_literal>(*sit)){
      auto const & v = value(as_string_ref(*sit));
      for(size_t i = 0; i < v.length() && it != end; ++i,++it){
        *it = v.at(i);++written;
      }
      if (it == end) break;
    }
  }
  return written;
}
size_t write_bytes(ceps::ast::Struct_ptr strct, iso2paymentOptionType* beg, iso2paymentOptionType* end);


namespace ceps2openv2g{
    using namespace std;
    using namespace ceps::ast;
    using namespace ceps::interpreter;

    extern Ism4ceps_plugin_interface* plugin_master;
    static const std::string version_info = "generator v0.1";
    static constexpr bool print_debug_info{true};
    ceps::ast::node_t plugin_entrypoint(ceps::ast::node_callparameters_t params);

    struct String_t{
		exi_string_character_t characters[iso2ParameterType_Name_CHARACTERS_SIZE];
		uint16_t charactersLen;
	};

    struct cert_t {
		uint8_t bytes[iso2SubCertificatesType_Certificate_BYTES_SIZE];
		uint16_t bytesLen;
	};

    struct certs_t{
		cert_t  array[iso2SubCertificatesType_Certificate_ARRAY_SIZE];
		uint16_t arrayLen;
	};

    struct AppProtocol_t{
		struct appHandAppProtocolType array[appHandAnonType_supportedAppProtocolReq_AppProtocol_ARRAY_SIZE];
		uint16_t arrayLen;
	} ;

    optional< pair<string,ceps::ast::Struct> > match_struct(node_t e);
    optional< ceps::ast::Struct > match_struct(node_t e, string what);
    template <typename F> void for_all_children(ceps::ast::Struct & s, F f){
        for (auto e : children(s)){
            f(e);
        }
    }
    optional<iso2responseCodeType> get_v2g_responsecode(ceps::ast::Struct&);

    class MessageBuilder{
        template <typename T> 
            bool filter (T&, ceps::ast::node_t e);
        template <typename T0, typename T1> size_t read_array(  ceps::ast::Struct & msg,
                                                             T1& r, 
                                                             string name) {
            auto it = r.array;
            auto it_begin = it;
            auto it_end = r.array + sizeof(r.array);
            for_all_children(msg, [&](node_t e){
                auto match_res = match_struct(e,name);
                if (match_res && (it != it_end) ) {
                    auto field = emit< T0 >(as_struct_ref(e));
                    *it = field;
                    ++it;
                    return;
                }
            });
            return it - it_begin;
        }


        public:
        template <typename T> void evse_prolog(T& r, ceps::ast::Struct & msg){
            for_all_children(msg, [&](node_t e){
                    auto match_res = match_struct(e,"ResponseCode");
                    if (match_res) {
                    auto code = get_v2g_responsecode(ceps::ast::as_struct_ref(e));
                    if (!code) return;
                    r.ResponseCode = *code;
                    return;               
                    }
                    match_res = match_struct(e,"EVSEStatus");
                    if (match_res) {

                        r.EVSEStatus_isUsed = 1;
                        r.EVSEStatus = emit<iso2EVSEStatusType>(ceps::ast::as_struct_ref(e));
                        return;
                    }
                });
        }
        template<typename T> T emit(ceps::ast::Struct & );
        template<typename T> node_t strct(T);
        std::pair<std::uint8_t*,size_t> build(ceps::ast::node_t data);
//@TODO: Rewrite stuff below using variadics
        static constexpr node_struct_t unused = nullptr;

        node_struct_t rec(string name, node_struct_t e1);
        node_struct_t rec(string name, vector<node_t> nodes);
        node_struct_t rec(string name, int v);
        node_struct_t rec(string name, int8_t v);
        node_struct_t rec(string name, uint8_t v);
        node_struct_t rec(string name, int16_t v);
        node_struct_t rec(string name, uint16_t v);
        node_struct_t rec(string name, int64_t v);
        node_struct_t rec(string name, uint64_t v);
        node_struct_t rec(string name, string v);
        node_struct_t rec(string name);

        node_struct_t rec(string name, AppProtocol_t);


        template<typename T> node_struct_t rec(string name, T v){
            return rec(name,children(as_struct_ref(strct<T>(v))));
        }

        template<typename T, typename... Ts> node_struct_t rec(string name, T a1, Ts... rest){
            auto r = rec(name,rest...);
            if (a1) children(*r).insert(children(*r).begin(),a1);
            return r;
        }
    };


    template <typename T> optional<T> get_numerical_field(ceps::ast::Struct& msg){
        if (children(msg).size() != 1) return {};
        auto elem = children(msg)[0];
        if (is<ceps::ast::Ast_node_kind::int_literal>(elem))
            return value(as_int_ref(elem));
        else if (is<ceps::ast::Ast_node_kind::long_literal>(elem))
            return value(as_int64_ref(elem));
        else if (is<ceps::ast::Ast_node_kind::float_literal>(elem))
            return value(as_double_ref(elem));
        return {};    
    }

    static constexpr char const * field_names[] = {"physicalValue","boolValue","Name",
    "byteValue","shortValue","intValue","stringValue","ServiceID","FreeService","Exponent","Value"};

    static constexpr size_t physicalValue = 0;
    static constexpr size_t boolValue = 1;
    static constexpr size_t Name = 2;
    static constexpr size_t byteValue = 3;
    static constexpr size_t shortValue = 4;
    static constexpr size_t intValue = 5;
    static constexpr size_t stringValue = 6;
    static constexpr size_t ServiceID = 7;
    static constexpr size_t FreeService = 8;
    static constexpr size_t Exponent = 9;
    static constexpr size_t Value = 10;

    optional<string> get_string_field(ceps::ast::Struct& msg);
    optional< pair<string,ceps::ast::Struct> > match_struct(node_t e);
    optional< ceps::ast::Struct > match_struct(node_t e, string what);
    optional<string> get_string_field(ceps::ast::Struct& msg);

    bool chain(node_t , MessageBuilder* );

    template<typename T, typename... Ts> 
    bool chain(node_t e, MessageBuilder* b, T arg, Ts... args){
        arg(e,b);
        chain(e,b,args...);
        return false;
    }

    template<typename T, size_t nm, typename V> struct fld{
        V& v;
        T& r;
        fld(V& v,T& r): v{v}, r{r} { }
        void operator () (node_t e, MessageBuilder* b){
                if (match_struct(e,field_names[nm])){
                    v = b->emit<V>(as_struct_ref(e));
                }
        }
    };

    template<typename T, size_t nm, typename V, typename W> struct fld2{
        V& v;
        T& r;
        W w;
        fld2(V& v,T& r, W w): v{v}, r{r}, w{w} { }
        void operator () (node_t e, MessageBuilder* b){
                if (match_struct(e,field_names[nm])){
                    v = b->emit<V>(as_struct_ref(e));
                    w();
                }
        }
    };

    template<typename T> 
     T MessageBuilder::emit(ceps::ast::Struct & msg){
        T r{};
        for_all_children(msg, [&](node_t e){
            filter(r,e);
        });
        return r;
    }

    optional<iso2responseCodeType> get_v2g_responsecode(ceps::ast::Struct&);
    optional<iso2EVSENotificationType> get_v2g_EVSENotification(ceps::ast::Struct&);


}

 
