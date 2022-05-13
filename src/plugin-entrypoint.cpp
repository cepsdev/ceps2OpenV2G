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


size_t write_bytes(ceps::ast::Struct_ptr strct, iso2paymentOptionType* beg, iso2paymentOptionType* end){
  using namespace ceps::ast;
  using namespace std;
  auto sit = strct->children().begin();
  size_t written = 0;
  for(auto it = beg; it != end && sit != strct->children().end();++sit){
    if (is<ceps::ast::Ast_node_kind::symbol>(*sit) && kind(as_symbol_ref(*sit)) == "v2gpaymentOptionType" ){
        map<string,iso2paymentOptionType> str2enum { 
            {"Contract",iso2paymentOptionType_Contract},
            {"ExternalPayment",iso2paymentOptionType_ExternalPayment}
        };
        auto fit = str2enum.find(name(as_symbol_ref(*sit)));
        if (fit == str2enum.end()) continue;
        *it = fit->second;
        ++it;
        ++written;
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
        template<typename T> void evse_prolog(T& r, ceps::ast::Struct & msg);
        template <typename T0, typename T1> size_t read_array(  ceps::ast::Struct & msg,
                                                             T1& r, 
                                                             string name);
        public:

        std::uint8_t* build(ceps::ast::node_t data);

    };

    template<> iso2EVSEStatusType MessageBuilder::emit<iso2EVSEStatusType>(ceps::ast::Struct & msg);
    template<> iso2SessionSetupReqType MessageBuilder::emit<iso2SessionSetupReqType>(ceps::ast::Struct & msg);
    template<> iso2SessionSetupResType MessageBuilder::emit<iso2SessionSetupResType>(ceps::ast::Struct & msg);
    template<> iso2SessionSetupReqType MessageBuilder::emit<iso2SessionSetupReqType>(ceps::ast::Struct & msg);
    template<> iso2SessionSetupResType MessageBuilder::emit<iso2SessionSetupResType>(ceps::ast::Struct & msg);
    template<> iso2ServiceListType MessageBuilder::emit<iso2ServiceListType>(ceps::ast::Struct & msg);

    


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
    
    optional<string> get_string_field(ceps::ast::Struct& msg){
        if (children(msg).size() != 1) return {};
        auto elem = children(msg)[0];
        if (is<ceps::ast::Ast_node_kind::string_literal>(elem))
            return value(as_string_ref(elem));
        return {};    
    }

    template <typename T> void MessageBuilder::evse_prolog(T& r, ceps::ast::Struct & msg){
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

    //
    // SessionSetupReqType
    //

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

    //
    // SessionSetupResType
    //

    template<> iso2SessionSetupResType MessageBuilder::emit<iso2SessionSetupResType>(ceps::ast::Struct & msg){
        iso2SessionSetupResType r{};
        evse_prolog(r,msg);
        for_all_children(msg, [&](node_t e){            
            auto match_res = match_struct(e,"EVSETimeStamp");
            if (match_res) {
                auto field_value = get_numerical_field<int64_t>(as_struct_ref(e));
                if (!field_value) return;
                r.EVSETimeStamp_isUsed = 1;
                r.EVSETimeStamp = *field_value;
                return;
            }  
        });
        return r;
    }

    //
    //EVSEStatusType
    //

    template<> iso2EVSEStatusType MessageBuilder::emit<iso2EVSEStatusType>(ceps::ast::Struct & msg){
        iso2EVSEStatusType r{};

        for_all_children(msg, [&](node_t e){
            auto match_res = match_struct(e,"NotificationMaxDelay");
            if (match_res) {
                auto field_value = get_numerical_field<uint16_t>(as_struct_ref(e));
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

    //
    //ServiceDiscoveryReq
    //

    template<> iso2ServiceDiscoveryReqType MessageBuilder::emit<iso2ServiceDiscoveryReqType>(ceps::ast::Struct & msg){
        iso2ServiceDiscoveryReqType r{};

        for_all_children(msg, [&](node_t e){
            auto match_res = match_struct(e,"SupportedServiceIDs");
            if (match_res) {
                auto n = write_bytes(   as_struct_ptr(e),
                                        r.SupportedServiceIDs.ServiceID.array, 
                                        r.SupportedServiceIDs.ServiceID.array + sizeof(r.SupportedServiceIDs.ServiceID.array));
                r.SupportedServiceIDs.ServiceID.arrayLen = n;
                r.SupportedServiceIDs_isUsed = (n != 0) ? 1 : 0;
            }
        });
        return r;
    }

    //
    // iso2ServiceType
    //
    

    template<> iso2ServiceType MessageBuilder::emit<iso2ServiceType>(ceps::ast::Struct & msg){
        iso2ServiceType r{};
        for_all_children(msg, [&](node_t e){
            auto match_res = match_struct(e,"ServiceID");
            if (match_res) {
                auto field = get_numerical_field<uint16_t>(as_struct_ref(e));
                if (!field) return;
                r.ServiceID = *field;
                return;
            }
            match_res = match_struct(e,"FreeService");
            if (match_res) {
                auto field = get_numerical_field<int32_t>(as_struct_ref(e));
                if (!field) return;
                r.FreeService = *field;
                return;
            }
        });
        return r;
    }


    //
    // iso2ServiceListType
    //
    
    template<> iso2ServiceListType MessageBuilder::emit<iso2ServiceListType>(ceps::ast::Struct & msg){
        iso2ServiceListType r{};
        r.Service.arrayLen = read_array<iso2ServiceType>(msg, r.Service, string{"Service"});
        return r;
    }


    //
    // iso2ServiceDiscoveryResType
    //

    template<> iso2ServiceDiscoveryResType MessageBuilder::emit<iso2ServiceDiscoveryResType>(ceps::ast::Struct & msg){
        iso2ServiceDiscoveryResType r{};

        evse_prolog(r,msg);

        for_all_children(msg, [&](node_t e){
            auto match_res = match_struct(e,"PaymentOptionList");
            if (match_res) {

                auto n = write_bytes(   as_struct_ptr(e),
                                        r.PaymentOptionList.PaymentOption.array, 
                                        r.PaymentOptionList.PaymentOption.array + 
                                            sizeof(r.PaymentOptionList.PaymentOption.array));
                r.PaymentOptionList.PaymentOption.arrayLen = n;
                return;
            }
            match_res = match_struct(e,"EnergyTransferServiceList");
            if (match_res) {
                r.EnergyTransferServiceList = emit<iso2ServiceListType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"VASList");
            if (match_res) {
                r.VASList = emit<iso2ServiceListType>(as_struct_ref(e));
                r.VASList_isUsed = 1;
                return;
            }
        });
        return r;
    }


    //
    // iso2ServiceDetailReqType
    //

    template<> iso2ServiceDetailReqType MessageBuilder::emit<iso2ServiceDetailReqType>(ceps::ast::Struct & msg){
        iso2ServiceDetailReqType r{};
        for_all_children(msg, [&](node_t e){
            auto match_res = match_struct(e,"ServiceID");
            if (match_res) {
                auto field = get_numerical_field<uint16_t>(as_struct_ref(e));
                if (!field) return;
                r.ServiceID = *field;
                return;
            }
        });
        return r;
    }


    //
    // iso2PhysicalValueType
    //

    template<> iso2PhysicalValueType MessageBuilder::emit<iso2PhysicalValueType>(ceps::ast::Struct & msg){
        iso2PhysicalValueType r{};
        for_all_children(msg, [&](node_t e){
            auto match_res = match_struct(e,"Exponent");
            if (match_res) {
                auto field = get_numerical_field<uint8_t>(as_struct_ref(e));
                if (!field) return;
                r.Exponent = *field;
                return;
            }
            match_res = match_struct(e,"Value");
            if (match_res) {
                auto field = get_numerical_field<uint16_t>(as_struct_ref(e));
                if (!field) return;
                r.Value = *field;
                return;
            }
        });
        return r;
    }


    //
    // iso2ParameterType
    //

    template<> iso2ParameterType MessageBuilder::emit<iso2ParameterType>(ceps::ast::Struct & msg){
        iso2ParameterType r{};
        for_all_children(msg, [&](node_t e){
            auto match_res = match_struct(e,"Name");
            if (match_res) {
                auto field_value = get_string_field(as_struct_ref(e));
                if (!field_value) return;
                strncpy(r.Name.characters,field_value->c_str(), sizeof(r.Name.characters - 1)); 
                r.Name.charactersLen = field_value->length();
                return;
            }
            match_res = match_struct(e,"boolValue");
            if (match_res) {
                auto field_value = get_numerical_field<int32_t>(as_struct_ref(e));
                if (!field_value) return;
                r.boolValue = *field_value;
                r.boolValue_isUsed = 1;
                return;
            }            
            match_res = match_struct(e,"byteValue");
            if (match_res) {
                auto field_value = get_numerical_field<int8_t>(as_struct_ref(e));
                if (!field_value) return;
                r.byteValue = *field_value;
                r.byteValue_isUsed = 1;
                return;
            }            
            match_res = match_struct(e,"shortValue");
            if (match_res) {
                auto field_value = get_numerical_field<int16_t>(as_struct_ref(e));
                if (!field_value) return;
                r.shortValue = *field_value;
                r.shortValue_isUsed = 1;
                return;
            }            
            match_res = match_struct(e,"intValue");
            if (match_res) {
                auto field_value = get_numerical_field<int32_t>(as_struct_ref(e));
                if (!field_value) return;
                r.intValue = *field_value;
                r.intValue_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"physicalValue");
            if (match_res) {
                auto field_value = emit<iso2PhysicalValueType>(as_struct_ref(e));
                r.physicalValue = field_value;
                r.physicalValue_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"stringValue");
            if (match_res) {
                auto field_value = get_string_field(as_struct_ref(e));
                if (!field_value) return;
                strncpy(r.stringValue.characters, field_value->c_str(), sizeof(r.stringValue.characters));
                r.stringValue.charactersLen = field_value->length();
                r.stringValue_isUsed = 1;
                return;
            }
        });
        return r;
    }

    //
    // iso2ParameterSetType
    //

    template <typename T0, typename T1> size_t MessageBuilder::read_array(  ceps::ast::Struct & msg,
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

    template<> iso2ParameterSetType MessageBuilder::emit<iso2ParameterSetType>(ceps::ast::Struct & msg){
        iso2ParameterSetType r{};
        r.Parameter.arrayLen = read_array<iso2ParameterType>(msg, r.Parameter, string{"Parameter"});
        for_all_children(msg, [&](node_t e){
            auto match_res = match_struct(e,"ParameterSetID");
            if (match_res) {
                auto field_value = get_numerical_field<int16_t>(as_struct_ref(e));
                if (!field_value) return;
                r.ParameterSetID = *field_value;
                return;
            }            
        });
        return r;
    }    


    //
    // iso2ServiceParameterListType
    //

    template<> iso2ServiceParameterListType MessageBuilder::emit<iso2ServiceParameterListType>(ceps::ast::Struct & msg){
        iso2ServiceParameterListType r{};
        r.ParameterSet.arrayLen = read_array<iso2ParameterSetType>(msg, r.ParameterSet, string{"ParameterSet"});
        return r;
    }    

    //
    // iso2ServiceDetailResType
    //

    template<> iso2ServiceDetailResType MessageBuilder::emit<iso2ServiceDetailResType>(ceps::ast::Struct & msg){
        iso2ServiceDetailResType r{};
        evse_prolog(r,msg);
        for_all_children(msg, [&](node_t e){
            auto match_res = match_struct(e,"ServciceID");
            if (match_res) {
                auto field_value = get_numerical_field<uint16_t>(as_struct_ref(e));
                if (!field_value) return;
                r.ServiceID = *field_value;
                return;
            }
            match_res = match_struct(e,"ServiceParameterList");
            if (match_res) {
                auto field_value = emit<iso2ServiceParameterListType>(as_struct_ref(e));
                r.ServiceParameterList = field_value;
                r.ServiceParameterList_isUsed = 1;
                return;
            }//
        });
        return r;
    }

    //
    // iso2paymentOptionType
    //

    template<> iso2paymentOptionType MessageBuilder::emit<iso2paymentOptionType>(ceps::ast::Struct & msg){
        iso2paymentOptionType r{};
        map<string,iso2paymentOptionType> str2enum { 
            {"Contract",iso2paymentOptionType_Contract},
            {"ExternalPayment",iso2paymentOptionType_ExternalPayment}
        };
        for_all_children(msg, [&] (node_t e){
            if (is<ceps::ast::Ast_node_kind::symbol>(e) && kind(as_symbol_ref(e)) == "v2gpaymentOptionType" ){
                auto fit = str2enum.find(name(as_symbol_ref(e)));
                if (fit == str2enum.end()) return;
                r = fit->second;
            }
        });
        return r;
    }


    //
    // iso2SelectedServiceType
    //

    template<> iso2SelectedServiceType MessageBuilder::emit<iso2SelectedServiceType>(ceps::ast::Struct & msg){
        iso2SelectedServiceType r{};
        for_all_children(msg, [&] (node_t e){
            auto match_res = match_struct(e,"ServciceID");
            if (match_res) {
                auto field_value = get_numerical_field<uint16_t>(as_struct_ref(e));
                if (!field_value) return;
                r.ServiceID = *field_value;
                return;
            }
            match_res = match_struct(e,"ParameterSetID");
            if (match_res) {
                auto field_value = get_numerical_field<uint16_t>(as_struct_ref(e));
                if (!field_value) return;
                r.ParameterSetID = *field_value;
                return;
            }            
        });
        return r;
    }

    //
    // iso2SelectedServiceListType
    //

    template<> iso2SelectedServiceListType MessageBuilder::emit<iso2SelectedServiceListType>(ceps::ast::Struct & msg){
        iso2SelectedServiceListType r{};
        r.SelectedService.arrayLen = read_array<iso2SelectedServiceType>(msg, r.SelectedService, string{"SelectedService"});
        return r;
    }    

    //
    // iso2PaymentServiceSelectionReqType
    //
    
    template<> iso2PaymentServiceSelectionReqType MessageBuilder::emit<iso2PaymentServiceSelectionReqType>(ceps::ast::Struct & msg){
        iso2PaymentServiceSelectionReqType r{};
        for_all_children(msg, [&](node_t e){
            auto match_res = match_struct(e,"SelectedPaymentOption");
            if (match_res) {
                r.SelectedPaymentOption = emit<iso2paymentOptionType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"SelectedEnergyTransferService");
            if (match_res) {
                r.SelectedEnergyTransferService = emit<iso2SelectedServiceType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"SelectedVASList");
            if (match_res) {
                r.SelectedVASList = emit<iso2SelectedServiceListType>(as_struct_ref(e));
                r.SelectedVASList_isUsed = 1;
                return;
            }
        });
        return r;
    }


    //
    // iso2PaymentServiceSelectionResType
    //
    
    template<> iso2PaymentServiceSelectionResType MessageBuilder::emit<iso2PaymentServiceSelectionResType>(ceps::ast::Struct & msg){
        iso2PaymentServiceSelectionResType r{};
        evse_prolog(r,msg);        
        return r;
    }

    struct cert_t {
		uint8_t bytes[iso2SubCertificatesType_Certificate_BYTES_SIZE];
		uint16_t bytesLen;
	};

    struct certs_t{
		cert_t  array[iso2SubCertificatesType_Certificate_ARRAY_SIZE];
		uint16_t arrayLen;
	};

    template<> cert_t MessageBuilder::emit<cert_t>(ceps::ast::Struct & msg){
        cert_t r{};
        r.bytesLen = write_bytes(&msg,r.bytes, r.bytes + sizeof(r.bytes));
        return r;
    }

    //
    // ContractSignatureCertChain
    //

    template<> iso2CertificateChainType MessageBuilder::emit<iso2CertificateChainType>(ceps::ast::Struct & msg){
        iso2CertificateChainType r{};
        for_all_children(msg, [&](node_t e){
            auto match_res = match_struct(e,"Id");
            if (match_res) {
                auto field_value = get_string_field(as_struct_ref(e));
                if (!field_value) return;
                strncpy(r.Id.characters,field_value->c_str(), sizeof(r.Id.characters - 1)); 
                r.Id.charactersLen = field_value->length();
                r.Id_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"Certificate");
            if (match_res) {
                r.Certificate.bytesLen = write_bytes(as_struct_ptr(e),r.Certificate.bytes, r.Certificate.bytes + sizeof(r.Certificate.bytes));
                return;
            }
            match_res = match_struct(e,"SubCertificates");
            if (match_res) {
                r.SubCertificates.Certificate.arrayLen = 
                 read_array<cert_t>(as_struct_ref(e), (certs_t&)r.SubCertificates.Certificate, string{"Certificate"});
                return;
            }            
        });
        return r;
    }

    //
    // iso2PaymentDetailsReqType
    //
    
    template<> iso2PaymentDetailsReqType MessageBuilder::emit<iso2PaymentDetailsReqType>(ceps::ast::Struct & msg){
        iso2PaymentDetailsReqType r{};
        for_all_children(msg, [&](node_t e){
            auto match_res = match_struct(e,"eMAID");
            if (match_res) {
                auto field_value = get_string_field(as_struct_ref(e));
                if (!field_value) return;
                strncpy(r.eMAID.characters,field_value->c_str(), sizeof(r.eMAID.characters - 1)); 
                r.eMAID.charactersLen = field_value->length();
                return;
            }
            match_res = match_struct(e,"ContractSignatureCertChain");
            if (match_res) {
                r.ContractSignatureCertChain = emit<iso2CertificateChainType>(as_struct_ref(e));
                return;
            }
        });
        return r;
    }

    //
    // iso2PaymentDetailsResType
    //
    
    template<> iso2PaymentDetailsResType MessageBuilder::emit<iso2PaymentDetailsResType>(ceps::ast::Struct & msg){
        iso2PaymentDetailsResType r{};
        evse_prolog(r,msg);
        for_all_children(msg, [&](node_t e){
            auto match_res = match_struct(e,"GenChallenge");
            if (match_res) {
                r.GenChallenge.bytesLen = write_bytes(as_struct_ptr(e),r.GenChallenge.bytes, 
                                                        r.GenChallenge.bytes + sizeof(r.GenChallenge.bytes));
                return;
            }
            match_res = match_struct(e,"EVSETimeStamp");
            if (match_res) {
                auto field_value = get_numerical_field<int64_t>(as_struct_ref(e));
                if (!field_value) return;
                r.EVSETimeStamp = *field_value;
            }
        });
        return r;
    }


    //
    // iso2AuthorizationReqType
    //
    
    template<> iso2AuthorizationReqType MessageBuilder::emit<iso2AuthorizationReqType>(ceps::ast::Struct & msg){
        iso2AuthorizationReqType r{};
        for_all_children(msg, [&](node_t e){            
            auto match_res = match_struct(e,"Id");
            if (match_res) {
                auto field_value = get_string_field(as_struct_ref(e));
                if (!field_value) return;
                strncpy(r.Id.characters,field_value->c_str(), sizeof(r.Id.characters - 1)); 
                r.Id.charactersLen = field_value->length();
                return;
            }            
            match_res = match_struct(e,"GenChallenge");
            if (match_res) {
                r.GenChallenge.bytesLen = write_bytes(as_struct_ptr(e),r.GenChallenge.bytes, 
                                                        r.GenChallenge.bytes + sizeof(r.GenChallenge.bytes));
                r.GenChallenge_isUsed = 1;
                return;
            }
        });
        return r;
    }


    //
    // iso2EVSEProcessingType
    //

    template<> iso2EVSEProcessingType MessageBuilder::emit<iso2EVSEProcessingType>(ceps::ast::Struct & msg){
        iso2EVSEProcessingType r{};
        map<string,iso2EVSEProcessingType> str2enum { 
            {"Finished",iso2EVSEProcessingType_Finished},
            {"Ongoing",iso2EVSEProcessingType_Ongoing},
            {"Ongoing_WaitingForCustomerInteraction",iso2EVSEProcessingType_Ongoing_WaitingForCustomerInteraction}
        };
        for_all_children(msg, [&] (node_t e){
            if (is<ceps::ast::Ast_node_kind::symbol>(e) && kind(as_symbol_ref(e)) == "v2gEVSEProcessingType" ){
                auto fit = str2enum.find(name(as_symbol_ref(e)));
                if (fit == str2enum.end()) return;
                r = fit->second;
            }
        });
        return r;
    }

    //
    // iso2AuthorizationResType
    //
    
    template<> iso2AuthorizationResType MessageBuilder::emit<iso2AuthorizationResType>(ceps::ast::Struct & msg){
        iso2AuthorizationResType r{};
        evse_prolog(r,msg);
        for_all_children(msg, [&](node_t e){            
            auto match_res = match_struct(e,"EVSEProcessing");
            if (match_res) {
                r.EVSEProcessing = emit<iso2EVSEProcessingType>(as_struct_ref(e));
                return;
            }            
        });
        return r;
    }


    //
    // iso2EVEnergyTransferParameterType
    //

    template<> iso2EVEnergyTransferParameterType MessageBuilder::emit<iso2EVEnergyTransferParameterType>(ceps::ast::Struct & msg){
        iso2EVEnergyTransferParameterType r{};
        for_all_children(msg, [&](node_t e){            
            auto match_res = match_struct(e,"DepartureTime");
            if (match_res) {
                auto field_value = get_numerical_field<uint32_t>(as_struct_ref(e));
                if (!field_value) return;
                r.DepartureTime = *field_value;
                return;
            }}
        );
        return r;
    }

    //
    // iso2AC_EVChargeParameterType
    //

    template<> iso2AC_EVChargeParameterType MessageBuilder::emit<iso2AC_EVChargeParameterType>(ceps::ast::Struct & msg){
        iso2AC_EVChargeParameterType r{};
        for_all_children(msg, [&](node_t e){            
            auto match_res = match_struct(e,"DepartureTime");
            if (match_res) {
                auto field_value = get_numerical_field<uint32_t>(as_struct_ref(e));
                if (!field_value) return;
                r.DepartureTime = *field_value;
                return;
            }
            match_res = match_struct(e,"EVMaximumChargePower");
            if (match_res) {
                r.EVMaximumChargePower = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVMaximumChargeCurrent");
            if (match_res) {
                r.EVMaximumChargeCurrent = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVMinimumChargeCurrent");
            if (match_res) {
                r.EVMinimumChargeCurrent = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVTargetEnergyRequest");
            if (match_res) {
                r.EVTargetEnergyRequest = emit<iso2PhysicalValueType>(as_struct_ref(e));
                r.EVTargetEnergyRequest_isUsed=1;
                return;
            }
            match_res = match_struct(e,"EVMaximumEnergyRequest");
            if (match_res) {
                r.EVMaximumEnergyRequest = emit<iso2PhysicalValueType>(as_struct_ref(e));
                r.EVMaximumEnergyRequest_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"EVMinimumEnergyRequest");
            if (match_res) {
                r.EVMinimumEnergyRequest = emit<iso2PhysicalValueType>(as_struct_ref(e));
                r.EVMinimumEnergyRequest_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"EVMaximumVoltage");
            if (match_res) {
                r.EVMaximumVoltage = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
        });    
        return r;
    }





    //
    // iso2AC_EVBidirectionalParameterType
    //

    template<> iso2AC_EVBidirectionalParameterType MessageBuilder::emit<iso2AC_EVBidirectionalParameterType>(ceps::ast::Struct & msg){
        iso2AC_EVBidirectionalParameterType r{};
        for_all_children(msg, [&](node_t e){            
            auto match_res = match_struct(e,"DepartureTime");
            if (match_res) {
                auto field_value = get_numerical_field<uint32_t>(as_struct_ref(e));
                if (!field_value) return;
                r.DepartureTime = *field_value;
                return;
            }
            match_res = match_struct(e,"EVMaximumChargePower");
            if (match_res) {
                r.EVMaximumChargePower = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVMaximumChargeCurrent");
            if (match_res) {
                r.EVMaximumChargeCurrent = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVMinimumChargeCurrent");
            if (match_res) {
                r.EVMinimumChargeCurrent = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVTargetEnergyRequest");
            if (match_res) {
                r.EVTargetEnergyRequest = emit<iso2PhysicalValueType>(as_struct_ref(e));
                r.EVTargetEnergyRequest_isUsed=1;
                return;
            }
            match_res = match_struct(e,"EVMaximumEnergyRequest");
            if (match_res) {
                r.EVMaximumEnergyRequest = emit<iso2PhysicalValueType>(as_struct_ref(e));
                r.EVMaximumEnergyRequest_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"EVMinimumEnergyRequest");
            if (match_res) {
                r.EVMinimumEnergyRequest = emit<iso2PhysicalValueType>(as_struct_ref(e));
                r.EVMinimumEnergyRequest_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"EVMaximumVoltage");
            if (match_res) {
                r.EVMaximumVoltage = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }

            match_res = match_struct(e,"EVMaximumDischargePower");
            if (match_res) {
                r.EVMaximumDischargePower = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVMaximumDischargeCurrent");
            if (match_res) {
                r.EVMaximumDischargeCurrent = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVMinimumDischargeCurrent");
            if (match_res) {
                r.EVMinimumDischargeCurrent = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }

        });    
        return r;
    }



    //
    // iso2ChargeParameterDiscoveryReqType
    //

    template<> iso2ChargeParameterDiscoveryReqType MessageBuilder::emit<iso2ChargeParameterDiscoveryReqType>(ceps::ast::Struct & msg){
        iso2ChargeParameterDiscoveryReqType r{};
        for_all_children(msg, [&](node_t e){            
            auto match_res = match_struct(e,"MaxSupportingPoints");
            if (match_res) {
                auto field_value = get_numerical_field<uint16_t>(as_struct_ref(e));
                if (!field_value) return;
                r.MaxSupportingPoints = *field_value;
                r.MaxSupportingPoints_isUsed = 1;
                return;
            }    
            match_res = match_struct(e,"EVEnergyTransferParameter");
            if (match_res) {
                r.EVEnergyTransferParameter = 
                                            emit<iso2EVEnergyTransferParameterType>(as_struct_ref(e));                
                r.EVEnergyTransferParameter_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"AC_EVChargeParameter");
            if (match_res) {
                r.AC_EVChargeParameter = emit<iso2AC_EVChargeParameterType>(as_struct_ref(e));
                r.AC_EVChargeParameter_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"AC_EVBidirectionalParameter");
            if (match_res) {
                r.AC_EVBidirectionalParameter = emit<iso2AC_EVBidirectionalParameterType>(as_struct_ref(e));
                r.AC_EVBidirectionalParameter_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"DC_EVChargeParameter");
            if (match_res) {
                r.DC_EVChargeParameter_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"DC_EVBidirectionalParameter");
            if (match_res) {
                r.DC_EVBidirectionalParameter_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"WPT_EVChargeParameter");
            if (match_res) {
                r.WPT_EVChargeParameter_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"MinimumPMaxRequest");
            if (match_res) {
                r.MinimumPMaxRequest_isUsed = 1;
                return;
            }
        });
        return r;
    }





    //
    // MessageBuilder::build
    //

    std::uint8_t* MessageBuilder::build(ceps::ast::node_t data){
        if (!is<Ast_node_kind::structdef>(data)) return {};
        auto& ceps_struct = *as_struct_ptr(data);
        if (name(ceps_struct)== "SessionSetupReq") 
            emit<iso2SessionSetupReqType>(ceps_struct);
        else if (name(ceps_struct)== "SessionSetupRes") 
            emit<iso2SessionSetupResType>(ceps_struct);
        else if(name(ceps_struct)== "ServiceDiscoveryReq")
         emit<iso2ServiceDiscoveryReqType>(ceps_struct); 
        else if(name(ceps_struct)== "ServiceDiscoveryRes")
         emit<iso2ServiceDiscoveryResType>(ceps_struct); 
        else if(name(ceps_struct)== "ServiceDetailReq")
         emit<iso2ServiceDetailReqType>(ceps_struct); 
        else if(name(ceps_struct)== "ServiceDetailRes")
         emit<iso2ServiceDetailResType>(ceps_struct); 
        else if(name(ceps_struct)== "PaymentServiceSelectionReq")
         emit<iso2PaymentServiceSelectionReqType>(ceps_struct); 
        else if(name(ceps_struct)== "PaymentServiceSelectionRes")
         emit<iso2PaymentServiceSelectionResType>(ceps_struct); 
        else if(name(ceps_struct)== "PaymentDetailsReq")
         emit<iso2PaymentDetailsReqType>(ceps_struct); 
        else if(name(ceps_struct)== "PaymentDetailsRes")
         emit<iso2PaymentDetailsResType>(ceps_struct);
        else if(name(ceps_struct)== "AuthorizationReq")
         emit<iso2AuthorizationReqType>(ceps_struct);
        else if(name(ceps_struct)== "AuthorizationRes")
         emit<iso2AuthorizationResType>(ceps_struct);
        else if(name(ceps_struct)== "ChargeParameterDiscoveryReq")
         emit<iso2ChargeParameterDiscoveryReqType>(ceps_struct);


          

         


    
        return nullptr;
    }
}


//
// Plugin specific code
//

ceps::ast::node_t ceps2openv2g::plugin_entrypoint(ceps::ast::node_callparameters_t params){
    using namespace std;
    using namespace ceps::ast;
    using namespace ceps::interpreter;

    auto data = get_first_child(params);    
    if (!is<Ast_node_kind::structdef>(data)) return nullptr;
    auto& ceps_struct = *as_struct_ptr(data);
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

