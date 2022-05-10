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


namespace ceps2openv2g{
    using namespace std;
    using namespace ceps::ast;
    using namespace ceps::interpreter;

    static Ism4ceps_plugin_interface* plugin_master = nullptr;
    static const std::string version_info = "generator v0.1";
    static constexpr bool print_debug_info{true};
    ceps::ast::node_t plugin_entrypoint(ceps::ast::node_callparameters_t params);

    class MessageBuilder{

        template<typename T> T emit(ceps::ast::Struct & );
        

        public:

        std::uint8_t* build(ceps::ast::node_t data);
    };

    template<> iso2EVSEStatusType MessageBuilder::emit<iso2EVSEStatusType>(ceps::ast::Struct & msg);
    template<> iso2SessionSetupReqType MessageBuilder::emit<iso2SessionSetupReqType>(ceps::ast::Struct & msg);
    template<> iso2SessionSetupResType MessageBuilder::emit<iso2SessionSetupResType>(ceps::ast::Struct & msg);

    template <typename F> void for_all_children(ceps::ast::Struct & s, F f){
        for (auto e : children(s)){
            f(e);
        }
    }

    optional< pair<string,ceps::ast::Struct> > match_struct(node_t e){
        if (!ceps::ast::is<ceps::ast::Ast_node_kind::structdef>(e)) return {};
        return make_pair(ceps::ast::name(*ceps::ast::as_struct_ptr(e)), *ceps::ast::as_struct_ptr(e)) ;
    }

    optional< ceps::ast::Struct > match_struct(node_t e, string what){
        if (!ceps::ast::is<ceps::ast::Ast_node_kind::structdef>(e)) return {};
        if (what != ceps::ast::name(*ceps::ast::as_struct_ptr(e))) return {};        
        return *ceps::ast::as_struct_ptr(e) ;
    }

    optional<iso2responseCodeType> get_v2g_responsecode(ceps::ast::Struct& msg){
        if (children(msg).size() != 1) return {};
        auto elem_raw = children(msg)[0];
        if (!ceps::ast::is<ceps::ast::Ast_node_kind::symbol>(elem_raw)) return {};
        auto& elem = as_symbol_ref(elem_raw);
        if (kind(elem) != "v2gResponsCodeType") return {};

        map<string,iso2responseCodeType> str2enum { 
            {"OK",iso2responseCodeType_OK},
        	{"OK_NewSessionEstablished", iso2responseCodeType_OK_NewSessionEstablished},
	        {"OK_OldSessionJoined", iso2responseCodeType_OK_OldSessionJoined },
	        {"OK_CertificateExpiresSoon", iso2responseCodeType_OK_CertificateExpiresSoon},
	        {"OK_IsolationValid", iso2responseCodeType_OK_IsolationValid },
	        {"OK_IsolationWarning", iso2responseCodeType_OK_IsolationWarning},
	        {"WARNING_CertificateExpired", iso2responseCodeType_WARNING_CertificateExpired},
	        {"WARNING_NoCertificateAvailable", iso2responseCodeType_WARNING_NoCertificateAvailable},
	        {"WARNING_CertValidationError", iso2responseCodeType_WARNING_CertValidationError},
	        {"WARNING_CertVerificationError", iso2responseCodeType_WARNING_CertVerificationError},
	        {"WARNING_ContractCanceled", iso2responseCodeType_WARNING_ContractCanceled},
	        {"FAILED", iso2responseCodeType_FAILED },
	        {"FAILED_SequenceError", iso2responseCodeType_FAILED_SequenceError },
	        {"FAILED_ServiceIDInvalid", iso2responseCodeType_FAILED_ServiceIDInvalid},
	        {"FAILED_UnknownSession", iso2responseCodeType_FAILED_UnknownSession},
	        {"FAILED_ServiceSelectionInvalid", iso2responseCodeType_FAILED_ServiceSelectionInvalid},
	        {"FAILED_SignatureError", iso2responseCodeType_FAILED_SignatureError},
	        {"FAILED_PaymentSelectionInvalid", iso2responseCodeType_FAILED_PaymentSelectionInvalid},
	        {"FAILED_ChallengeInvalid", iso2responseCodeType_FAILED_ChallengeInvalid },
	        {"FAILED_WrongChargeParameter", iso2responseCodeType_FAILED_WrongChargeParameter},
	        {"FAILED_IsolationFault", iso2responseCodeType_FAILED_IsolationFault},
	        {"FAILED_PowerDeliveryNotApplied", iso2responseCodeType_FAILED_PowerDeliveryNotApplied},
	        {"FAILED_TariffSelectionInvalid", iso2responseCodeType_FAILED_TariffSelectionInvalid},
	        {"FAILED_ChargingProfileInvalid", iso2responseCodeType_FAILED_ChargingProfileInvalid},
	        {"FAILED_MeteringSignatureNotValid", iso2responseCodeType_FAILED_MeteringSignatureNotValid},
	        {"FAILED_NoChargeServiceSelected", iso2responseCodeType_FAILED_NoChargeServiceSelected},
	        {"FAILED_WrongEnergyTransferMode", iso2responseCodeType_FAILED_WrongEnergyTransferMode},
	        {"FAILED_ContactorError", iso2responseCodeType_FAILED_ContactorError},
	        {"FAILED_CertificateRevoked", iso2responseCodeType_FAILED_CertificateRevoked},
	        {"FAILED_CertificateNotYetValid", iso2responseCodeType_FAILED_CertificateNotYetValid}
        };

        auto it = str2enum.find(name(elem));
        if (it == str2enum.end()) return {};
        return it->second;
    }
    optional<iso2EVSENotificationType> get_v2g_EVSENotification(ceps::ast::Struct& msg){
        if (children(msg).size() != 1) return {};
        auto elem_raw = children(msg)[0];
        if (!ceps::ast::is<ceps::ast::Ast_node_kind::symbol>(elem_raw)) return {};
        auto& elem = as_symbol_ref(elem_raw);
        if (kind(elem) != "v2gEVSENotificationType") return {};

        map<string,iso2EVSENotificationType> str2enum { 
            {"StopCharging",iso2EVSENotificationType_StopCharging},
        	{"ReNegotiation", iso2EVSENotificationType_ReNegotiation}
        };

        auto it = str2enum.find(name(elem));
        if (it == str2enum.end()) return {};
        return it->second;
    }

    
    
    optional<uint16_t> get_uint16_field(ceps::ast::Struct& msg){
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

    template<> iso2SessionSetupReqType MessageBuilder::emit<iso2SessionSetupReqType>(ceps::ast::Struct & msg){
        iso2SessionSetupReqType r{};
        for_all_children(msg, [&](node_t e){
            auto match_res = match_struct(e,"EVCCID");
            if (!match_res) return;
            auto strct = *match_res;
            auto n = write_bytes(&strct,(r.EVCCID.bytes), r.EVCCID.bytes + sizeof(r.EVCCID.bytes));
            r.EVCCID.bytesLen = n;            
        });
        return r;
    }

    template<> iso2SessionSetupResType MessageBuilder::emit<iso2SessionSetupResType>(ceps::ast::Struct & msg){
        iso2SessionSetupResType r{};
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
        return r;
    }

    template<> iso2EVSEStatusType MessageBuilder::emit<iso2EVSEStatusType>(ceps::ast::Struct & msg){
        iso2EVSEStatusType r{};

        for_all_children(msg, [&](node_t e){
            auto match_res = match_struct(e,"NotificationMaxDelay");
            if (match_res) {
                auto field_value = get_uint16_field(as_struct_ref(e));
                if (!field_value) return;
                r.NotificationMaxDelay = *field_value;
                return;
            }
            match_res = match_struct(e,"EVSENotification");
            if (match_res) {
                auto field_value = get_v2g_EVSENotification(as_struct_ref(e));
                if (!field_value) return;
                r.EVSENotification = *field_value;
                return;
            }
        });
        return r;
    }
    std::uint8_t* MessageBuilder::build(ceps::ast::node_t data){
        if (!is<Ast_node_kind::structdef>(data)) return {};
        auto& ceps_struct = *as_struct_ptr(data);
        if (name(ceps_struct)== "SessionSetupReq") 
            emit<iso2SessionSetupReqType>(ceps_struct);
        else if (name(ceps_struct)== "SessionSetupRes") 
            emit<iso2SessionSetupResType>(ceps_struct);

        return nullptr;
    }
}

ceps::ast::node_t ceps2openv2g::plugin_entrypoint(ceps::ast::node_callparameters_t params){
    using namespace std;
    using namespace ceps::ast;
    using namespace ceps::interpreter;

    auto data = get_first_child(params);    
    if (!is<Ast_node_kind::structdef>(data)) return nullptr;
    auto& ceps_struct = *as_struct_ptr(data);
    MessageBuilder msg_builder;
    msg_builder.build(data);
    cout << "generator::plugin_entrypoint:\n";

    for(auto e : children(ceps_struct)){
        cout <<"\t"<< * e << "\n";
    }
    cout <<"\n\n";
    auto result = mk_struct("result");
    children(*result).push_back(mk_int_node(42));
    return result;
}

extern "C" void init_plugin(IUserdefined_function_registry* smc)
{
  ceps2openv2g::plugin_master = smc->get_plugin_interface();
  ceps2openv2g::plugin_master->reg_ceps_phase0plugin("v2g", ceps2openv2g::plugin_entrypoint);
}

