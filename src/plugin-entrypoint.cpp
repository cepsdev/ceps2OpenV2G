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
    // iso2DC_EVChargeParameterType
    //

    template<> iso2DC_EVChargeParameterType MessageBuilder::emit<iso2DC_EVChargeParameterType>(ceps::ast::Struct & msg){
        iso2DC_EVChargeParameterType r{};
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
                r.EVMaximumChargePower_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"EVMinimumChargePower");
            if (match_res) {
                r.EVMinimumChargePower = emit<iso2PhysicalValueType>(as_struct_ref(e));
                r.EVMinimumChargePower_isUsed = 1;
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

            match_res = match_struct(e,"EVMaximumVoltage");
            if (match_res) {
                r.EVMaximumVoltage = emit<iso2PhysicalValueType>(as_struct_ref(e));
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
            match_res = match_struct(e,"CurrentSOC");
            if (match_res) {
                auto field_value = get_numerical_field<int8_t>(as_struct_ref(e));
                if (!field_value) return;
                r.CurrentSOC = *field_value;
                r.CurrentSOC_isUsed = 1;
                return;
            }    
            match_res = match_struct(e,"TargetSOC");
            if (match_res) {
                auto field_value = get_numerical_field<int8_t>(as_struct_ref(e));
                if (!field_value) return;
                r.TargetSOC = *field_value;
                r.TargetSOC_isUsed = 1;
                return;
            }    
            match_res = match_struct(e,"BulkSOC");
            if (match_res) {
                auto field_value = get_numerical_field<int8_t>(as_struct_ref(e));
                if (!field_value) return;
                r.BulkSOC = *field_value;
                r.BulkSOC_isUsed = 1;
                return;
            } 
        });    
        return r;
    }




    //
    // iso2DC_EVBidirectionalParameterType
    //

    template<> iso2DC_EVBidirectionalParameterType MessageBuilder::emit<iso2DC_EVBidirectionalParameterType>(ceps::ast::Struct & msg){
        iso2DC_EVBidirectionalParameterType r{};
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
                r.EVMaximumChargePower_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"EVMinimumChargePower");
            if (match_res) {
                r.EVMinimumChargePower = emit<iso2PhysicalValueType>(as_struct_ref(e));
                r.EVMinimumChargePower_isUsed = 1;
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

            match_res = match_struct(e,"EVMaximumVoltage");
            if (match_res) {
                r.EVMaximumVoltage = emit<iso2PhysicalValueType>(as_struct_ref(e));
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
            match_res = match_struct(e,"CurrentSOC");
            if (match_res) {
                auto field_value = get_numerical_field<int8_t>(as_struct_ref(e));
                if (!field_value) return;
                r.CurrentSOC = *field_value;
                r.CurrentSOC_isUsed = 1;
                return;
            }    
            match_res = match_struct(e,"TargetSOC");
            if (match_res) {
                auto field_value = get_numerical_field<int8_t>(as_struct_ref(e));
                if (!field_value) return;
                r.TargetSOC = *field_value;
                r.TargetSOC_isUsed = 1;
                return;
            }    
            match_res = match_struct(e,"BulkSOC");
            if (match_res) {
                auto field_value = get_numerical_field<int8_t>(as_struct_ref(e));
                if (!field_value) return;
                r.BulkSOC = *field_value;
                r.BulkSOC_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"EVMaximumDischargePower");
            if (match_res) {
                r.EVMaximumDischargePower = emit<iso2PhysicalValueType>(as_struct_ref(e));
                r.EVMaximumDischargePower_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"EVMinimumDischargePower");
            if (match_res) {
                r.EVMinimumDischargePower = emit<iso2PhysicalValueType>(as_struct_ref(e));
                r.EVMinimumDischargePower_isUsed = 1;
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
            match_res = match_struct(e,"EVMinimumVoltage");
            if (match_res) {
                r.EVMinimumVoltage = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"MinimumSOC");
            if (match_res) {
                auto field_value = get_numerical_field<int8_t>(as_struct_ref(e));
                if (!field_value) return;
                r.MinimumSOC = *field_value;
                r.MinimumSOC_isUsed = 1;
                return;
            }            
        });    
        return r;
    }


    //
    // iso2WPT_EVChargeParameterType
    //

    template<> iso2WPT_EVChargeParameterType MessageBuilder::emit<iso2WPT_EVChargeParameterType>(ceps::ast::Struct & msg){
        iso2WPT_EVChargeParameterType r{};
        for_all_children(msg, [&](node_t e){            
            auto match_res = match_struct(e,"DepartureTime");
            if (match_res) {
                auto field_value = get_numerical_field<uint32_t>(as_struct_ref(e));
                if (!field_value) return;
                r.DepartureTime = *field_value;
                return;
            }
            match_res = match_struct(e,"EVMaximumPower");
            if (match_res) {
                r.EVMaximumPower = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVMinimumPower");
            if (match_res) {
                r.EVMinimumPower = emit<iso2PhysicalValueType>(as_struct_ref(e));
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
        });    
        return r;
    }

    //
    // iso2RelativeTimeIntervalType
    //

    template<> iso2RelativeTimeIntervalType MessageBuilder::emit<iso2RelativeTimeIntervalType>(ceps::ast::Struct & msg){
        iso2RelativeTimeIntervalType r{};
        for_all_children(msg, [&](node_t e){            
            auto match_res = match_struct(e,"start");
            if (match_res) {
                auto field_value = get_numerical_field<uint32_t>(as_struct_ref(e));
                if (!field_value) return;
                r.start = *field_value;
                return;
            } 
            match_res = match_struct(e,"duration");
            if (match_res) {
                auto field_value = get_numerical_field<uint32_t>(as_struct_ref(e));
                if (!field_value) return;
                r.duration = *field_value;
                r.duration_isUsed = 1;
                return;
            }
        });
        return r;
    }

    //
    // iso2PMaxScheduleEntryType
    //

    template<> iso2PMaxScheduleEntryType MessageBuilder::emit<iso2PMaxScheduleEntryType>(ceps::ast::Struct & msg){
        iso2PMaxScheduleEntryType r{};
        r.PMax.arrayLen = 
            read_array<iso2PhysicalValueType>(msg, r.PMax, string{"PMax"});
        for_all_children(msg, [&](node_t e){            
            auto match_res = match_struct(e,"RelativeTimeInterval");
            if (match_res) {
                r.RelativeTimeInterval = emit<iso2RelativeTimeIntervalType>(as_struct_ref(e));
                return;
            }
        });
        return r;            
    }

    //
    // iso2MinimumPMaxRequestType
    //

    template<> iso2MinimumPMaxRequestType MessageBuilder::emit<iso2MinimumPMaxRequestType>(ceps::ast::Struct & msg){
        iso2MinimumPMaxRequestType r{};
        r.MinimumPMaxScheduleEntry.arrayLen = 
            read_array<iso2PMaxScheduleEntryType>(msg, r.MinimumPMaxScheduleEntry, string{"PMaxScheduleEntry"});
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
                r.DC_EVChargeParameter = emit<iso2DC_EVChargeParameterType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"DC_EVBidirectionalParameter");
            if (match_res) {
                r.DC_EVBidirectionalParameter = emit<iso2DC_EVBidirectionalParameterType>(as_struct_ref(e));
                r.DC_EVBidirectionalParameter_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"WPT_EVChargeParameter");
            if (match_res) {
                r.WPT_EVChargeParameter_isUsed = 1;
                r.WPT_EVChargeParameter = emit<iso2WPT_EVChargeParameterType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"MinimumPMaxRequest");
            if (match_res) {
                r.MinimumPMaxRequest = emit<iso2MinimumPMaxRequestType>(as_struct_ref(e));
                r.MinimumPMaxRequest_isUsed = 1;
                return;
            }
        });
        return r;
    }

    //
    // iso2PMaxScheduleType
    //

    template<> iso2PMaxScheduleType MessageBuilder::emit<iso2PMaxScheduleType>(ceps::ast::Struct & msg){
        iso2PMaxScheduleType r{};
        r.PMaxScheduleEntry.arrayLen = 
            read_array<iso2PMaxScheduleEntryType>(msg, r.PMaxScheduleEntry, string{"PMaxScheduleEntry"});
        return r;
    }   


    //
    // iso2SAScheduleTupleType
    //


    template<> iso2SAScheduleTupleType MessageBuilder::emit<iso2SAScheduleTupleType>(ceps::ast::Struct & msg){
        iso2SAScheduleTupleType r{};
        for_all_children(msg, [&](node_t e){            
            auto match_res = match_struct(e,"SAScheduleTupleID");
            if (match_res) {
                auto field_value = get_numerical_field<uint8_t>(as_struct_ref(e));
                if (!field_value) return;
                r.SAScheduleTupleID = *field_value;
                return;
            }    
            match_res = match_struct(e,"PMaxSchedule");
            if (match_res) {
                r.PMaxSchedule = emit<iso2PMaxScheduleType>(as_struct_ref(e));                
                return;
            }
            match_res = match_struct(e,"PMaxDischargeSchedule");
            if (match_res) {
                r.PMaxDischargeSchedule = emit<iso2PMaxScheduleType>(as_struct_ref(e));
                r.PMaxDischargeSchedule_isUsed = 1;               
                return;
            }
            match_res = match_struct(e,"SalesTariff");
            if (match_res) {
                r.SalesTariff = emit<iso2SalesTariffType>(as_struct_ref(e));
                r.SalesTariff_isUsed = 1;               
                return;
            }
            match_res = match_struct(e,"BuyBackTariff");
            if (match_res) {
                r.BuyBackTariff = emit<iso2SalesTariffType>(as_struct_ref(e));
                r.BuyBackTariff_isUsed = 1;               
                return;
            }            
        });
        return r;
    }

    //
    // iso2SAScheduleListType
    //

    template<> iso2SAScheduleListType MessageBuilder::emit<iso2SAScheduleListType>(ceps::ast::Struct & msg){
        iso2SAScheduleListType r{};
        r.SAScheduleTuple.arrayLen = 
            read_array<iso2SAScheduleTupleType>(msg, r.SAScheduleTuple, string{"SAScheduleTuple"});
        return r;
    }    


    //
    // iso2EVSEEnergyTransferParameterType
    //

    template<> iso2EVSEEnergyTransferParameterType MessageBuilder::emit<iso2EVSEEnergyTransferParameterType>(ceps::ast::Struct & msg){
        iso2EVSEEnergyTransferParameterType r{};
        return r;
    } 

    //
    // iso2AC_EVSEChargeParameterType
    //

    template<> iso2AC_EVSEChargeParameterType MessageBuilder::emit<iso2AC_EVSEChargeParameterType>(ceps::ast::Struct & msg){
        iso2AC_EVSEChargeParameterType r{};
        for_all_children(msg, [&](node_t e){            
            auto match_res = match_struct(e,"EVSEMaximumChargeCurrent");
            if (match_res) {
                r.EVSEMaximumChargeCurrent = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVSENominalVoltage");
            if (match_res) {
                r.EVSENominalVoltage = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVSENominalFrequency");
            if (match_res) {
                r.EVSENominalFrequency = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
        });
        return r;
    }

    //
    // iso2DC_EVSEChargeParameterType
    //

    template<> iso2DC_EVSEChargeParameterType MessageBuilder::emit<iso2DC_EVSEChargeParameterType>(ceps::ast::Struct & msg){
        iso2DC_EVSEChargeParameterType r{};
        for_all_children(msg, [&](node_t e){            
            auto match_res = match_struct(e,"EVSEMaximumChargePower");
            if (match_res) {
                r.EVSEMaximumChargePower = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVSEMaximumChargeCurrent");
            if (match_res) {
                r.EVSEMaximumChargeCurrent = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVSEMinimumChargeCurrent");
            if (match_res) {
                r.EVSEMinimumChargeCurrent = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVSEMaximumVoltage");
            if (match_res) {
                r.EVSEMaximumVoltage = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVSEMinimumVoltage");
            if (match_res) {
                r.EVSEMinimumVoltage = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVSECurrentRegulationTolerance");
            if (match_res) {
                r.EVSECurrentRegulationTolerance = emit<iso2PhysicalValueType>(as_struct_ref(e));
                r.EVSECurrentRegulationTolerance_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"EVSEPeakCurrentRipple");
            if (match_res) {
                r.EVSEPeakCurrentRipple = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVSEEnergyToBeDelivered");
            if (match_res) {
                r.EVSEEnergyToBeDelivered = emit<iso2PhysicalValueType>(as_struct_ref(e));
                r.EVSEEnergyToBeDelivered_isUsed = 1;
                return;
            }
        });
        return r;
    }


    //
    // iso2AC_EVSEBidirectionalParameterType
    //

    template<> iso2AC_EVSEBidirectionalParameterType MessageBuilder::emit<iso2AC_EVSEBidirectionalParameterType>(ceps::ast::Struct & msg){
        iso2AC_EVSEBidirectionalParameterType r{};
        for_all_children(msg, [&](node_t e){            
            auto match_res = match_struct(e,"EVSEMaximumChargeCurrent");
            if (match_res) {
                r.EVSEMaximumChargeCurrent = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVSENominalVoltage");
            if (match_res) {
                r.EVSENominalVoltage = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVSENominalFrequency");
            if (match_res) {
                r.EVSENominalFrequency = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVSEMaximumDischargeCurrent");
            if (match_res) {
                r.EVSEMaximumDischargeCurrent = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
        });
        return r;
    }

    //
    // iso2DC_EVSEBidirectionalParameterType
    //

    template<> iso2DC_EVSEBidirectionalParameterType MessageBuilder::emit<iso2DC_EVSEBidirectionalParameterType>(ceps::ast::Struct & msg){
        iso2DC_EVSEBidirectionalParameterType r{};
        for_all_children(msg, [&](node_t e){            
            auto match_res = match_struct(e,"EVSEMaximumChargePower");
            if (match_res) {
                r.EVSEMaximumChargePower = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVSEMaximumChargeCurrent");
            if (match_res) {
                r.EVSEMaximumChargeCurrent = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVSEMinimumChargeCurrent");
            if (match_res) {
                r.EVSEMinimumChargeCurrent = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVSEMaximumVoltage");
            if (match_res) {
                r.EVSEMaximumVoltage = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVSEMinimumVoltage");
            if (match_res) {
                r.EVSEMinimumVoltage = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVSECurrentRegulationTolerance");
            if (match_res) {
                r.EVSECurrentRegulationTolerance = emit<iso2PhysicalValueType>(as_struct_ref(e));
                r.EVSECurrentRegulationTolerance_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"EVSEPeakCurrentRipple");
            if (match_res) {
                r.EVSEPeakCurrentRipple = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVSEEnergyToBeDelivered");
            if (match_res) {
                r.EVSEEnergyToBeDelivered = emit<iso2PhysicalValueType>(as_struct_ref(e));
                r.EVSEEnergyToBeDelivered_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"EVSEMaximumDischargePower");
            if (match_res) {
                r.EVSEMaximumDischargePower = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVSEMaximumDischargeCurrent");
            if (match_res) {
                r.EVSEMaximumDischargeCurrent = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVSEMinimumDischargeCurrent");
            if (match_res) {
                r.EVSEMinimumDischargeCurrent = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
        });
        return r;
    }

    //
    // iso2WPT_EVSEChargeParameterType
    //

    template<> iso2WPT_EVSEChargeParameterType MessageBuilder::emit<iso2WPT_EVSEChargeParameterType>(ceps::ast::Struct & msg){
        iso2WPT_EVSEChargeParameterType r{};
        for_all_children(msg, [&](node_t e){            
            auto match_res = match_struct(e,"EVSEMaximumPower");
            if (match_res) {
                r.EVSEMaximumPower = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVSEMinimumPower");
            if (match_res) {
                r.EVSEMinimumPower = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
        });    
        return r;
    }

    //
    // iso2ChargeParameterDiscoveryResType
    //

    template<> iso2ChargeParameterDiscoveryResType MessageBuilder::emit<iso2ChargeParameterDiscoveryResType>(ceps::ast::Struct & msg){
        iso2ChargeParameterDiscoveryResType r{};
        evse_prolog(r,msg);
        for_all_children(msg, [&](node_t e){            
            auto match_res = match_struct(e,"EVSEProcessing");
            if (match_res) {
                r.EVSEProcessing = emit<iso2EVSEProcessingType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"SAScheduleList");
            if (match_res) {
                r.SAScheduleList = emit<iso2SAScheduleListType>(as_struct_ref(e));                
                r.SAScheduleList_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"EVSEEnergyTransferParameter");
            if (match_res) {
                r.EVSEEnergyTransferParameter = emit<iso2EVSEEnergyTransferParameterType>(as_struct_ref(e));                
                r.EVSEEnergyTransferParameter_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"AC_EVSEChargeParameter");
            if (match_res) {
                r.AC_EVSEChargeParameter = emit<iso2AC_EVSEChargeParameterType>(as_struct_ref(e));                
                r.AC_EVSEChargeParameter_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"AC_EVSEBidirectionalParameter");
            if (match_res) {
                r.AC_EVSEBidirectionalParameter = emit<iso2AC_EVSEBidirectionalParameterType>(as_struct_ref(e));                
                r.AC_EVSEBidirectionalParameter_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"DC_EVSEChargeParameter");
            if (match_res) {
                r.DC_EVSEChargeParameter = emit<iso2DC_EVSEChargeParameterType>(as_struct_ref(e));                
                r.DC_EVSEChargeParameter_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"DC_EVSEBidirectionalParameter");
            if (match_res) {
                r.DC_EVSEBidirectionalParameter = emit<iso2DC_EVSEBidirectionalParameterType>(as_struct_ref(e));                
                r.DC_EVSEBidirectionalParameter_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"WPT_EVSEChargeParameter");
            if (match_res) {
                r.WPT_EVSEChargeParameter = emit<iso2WPT_EVSEChargeParameterType>(as_struct_ref(e));                
                r.WPT_EVSEChargeParameter_isUsed = 1;
                return;
            }
        });
        return r;
    }

    //
    // iso2chargeProgressType
    //
 
    template<> iso2chargeProgressType MessageBuilder::emit<iso2chargeProgressType>(ceps::ast::Struct & msg){
        iso2chargeProgressType r{};
        map<string,iso2chargeProgressType> str2enum { 
            {"Start",iso2chargeProgressType_Start},
            {"Stop",iso2chargeProgressType_Stop},
            {"Renegotiate",iso2chargeProgressType_Renegotiate}
        };
        for_all_children(msg, [&] (node_t e){
            if (is<ceps::ast::Ast_node_kind::symbol>(e) && kind(as_symbol_ref(e)) == "v2gchargeProgressType" ){
                auto fit = str2enum.find(name(as_symbol_ref(e)));
                if (fit == str2enum.end()) return;
                r = fit->second;
            }
        });
        return r;
    }

    //
    // iso2EVOperationType
    //


    template<> iso2EVOperationType MessageBuilder::emit<iso2EVOperationType>(ceps::ast::Struct & msg){
        iso2EVOperationType r{};
        map<string,iso2EVOperationType> str2enum { 
            {"Charge",iso2EVOperationType_Charge},
            {"BPT",iso2EVOperationType_BPT}};
        for_all_children(msg, [&] (node_t e){
            if (is<ceps::ast::Ast_node_kind::symbol>(e) && kind(as_symbol_ref(e)) == "v2gEVOperationType" ){
                auto fit = str2enum.find(name(as_symbol_ref(e)));
                if (fit == str2enum.end()) return;
                r = fit->second;
            }
        });
        return r;
    }


    //
    // iso2ChargingProfileType
    //

    template<> iso2ChargingProfileType MessageBuilder::emit<iso2ChargingProfileType>(ceps::ast::Struct & msg){
        iso2ChargingProfileType r{};
        r.ProfileEntry.arrayLen = 
            read_array<iso2PMaxScheduleEntryType>(msg, r.ProfileEntry, string{"ProfileEntry"});
        return r;
    }    

    //
    // iso2PowerDeliveryReqType
    //

    template<> iso2PowerDeliveryReqType MessageBuilder::emit<iso2PowerDeliveryReqType>(ceps::ast::Struct & msg){
        iso2PowerDeliveryReqType r{};
        for_all_children(msg, [&](node_t e){            
            auto match_res = match_struct(e,"ChargeProgress");
            if (match_res) {
                r.ChargeProgress = emit<iso2chargeProgressType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVOperation");
            if (match_res) {
                r.EVOperation_isUsed = 1;
                r.EVOperation = emit<iso2EVOperationType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"SAScheduleTupleID");
            if (match_res) {
                auto field_value = get_numerical_field<uint8_t>(as_struct_ref(e));
                if (!field_value) return;
                r.SAScheduleTupleID = *field_value;
                return;
            }    
            match_res = match_struct(e,"ChargingProfile");
            if (match_res) {
                r.ChargingProfile_isUsed = 1;
                r.ChargingProfile = emit<iso2ChargingProfileType>(as_struct_ref(e));
                return;
            }
        });
        return r;
    }

    //
    // iso2PowerDeliveryResType
    //

    template<> iso2PowerDeliveryResType MessageBuilder::emit<iso2PowerDeliveryResType>(ceps::ast::Struct & msg){
        iso2PowerDeliveryResType r{};
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
    // iso2CableCheckReqType
    //

    template<> iso2CableCheckReqType MessageBuilder::emit<iso2CableCheckReqType>(ceps::ast::Struct & msg){
        iso2CableCheckReqType r{};
        return r;
    }

    //
    // iso2CableCheckResType
    //

    template<> iso2CableCheckResType MessageBuilder::emit<iso2CableCheckResType>(ceps::ast::Struct & msg){
        iso2CableCheckResType r{};
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
    // iso2PreChargeReqType
    //

    template<> iso2PreChargeReqType MessageBuilder::emit<iso2PreChargeReqType>(ceps::ast::Struct & msg){
        iso2PreChargeReqType r{};
        for_all_children(msg, [&](node_t e){            
            auto match_res = match_struct(e,"EVTargetVoltage");
            if (match_res) {
                r.EVTargetVoltage = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVTargetCurrent");
            if (match_res) {
                r.EVTargetCurrent = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
        });
        return r;
    }

    //
    // iso2PreChargeResType
    //

    template<> iso2PreChargeResType MessageBuilder::emit<iso2PreChargeResType>(ceps::ast::Struct & msg){
        iso2PreChargeResType r{};
        evse_prolog(r,msg);
        for_all_children(msg, [&](node_t e){            
            auto match_res = match_struct(e,"EVSEPresentVoltage");
            if (match_res) {
                r.EVSEPresentVoltage = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
        });
        return r;
    }

    //
    // iso2DisplayParametersType
    //

    template<> iso2DisplayParametersType MessageBuilder::emit<iso2DisplayParametersType>(ceps::ast::Struct & msg){
        iso2DisplayParametersType r{};
        for_all_children(msg, [&](node_t e){            
            auto match_res = match_struct(e,"CurrentRange");
            if (match_res) {
                auto field_value = get_numerical_field<uint16_t>(as_struct_ref(e));
                if (!field_value) return;
                r.CurrentRange = *field_value;
                r.CurrentRange_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"CurrentSOC");
            if (match_res) {
                auto field_value = get_numerical_field<int8_t>(as_struct_ref(e));
                if (!field_value) return;
                r.CurrentSOC = *field_value;
                r.CurrentSOC_isUsed = 1;
                return;
            }            
            match_res = match_struct(e,"TargetSOC");
            if (match_res) {
                auto field_value = get_numerical_field<int8_t>(as_struct_ref(e));
                if (!field_value) return;
                r.TargetSOC = *field_value;
                r.TargetSOC_isUsed = 1;
                return;
            }            
            match_res = match_struct(e,"BulkSOC");
            if (match_res) {
                auto field_value = get_numerical_field<int8_t>(as_struct_ref(e));
                if (!field_value) return;
                r.BulkSOC = *field_value;
                r.BulkSOC_isUsed = 1;
                return;
            }            
            match_res = match_struct(e,"MinimumSOC");
            if (match_res) {
                auto field_value = get_numerical_field<int8_t>(as_struct_ref(e));
                if (!field_value) return;
                r.MinimumSOC = *field_value;
                r.MinimumSOC_isUsed = 1;
                return;
            }            
            match_res = match_struct(e,"ChargingPerformance");
            if (match_res) {
                r.ChargingPerformance = emit<iso2PhysicalValueType>(as_struct_ref(e));
                r.ChargingPerformance_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"RemainingTimeToTargetSOC");
            if (match_res) {
                auto field_value = get_numerical_field<int8_t>(as_struct_ref(e));
                if (!field_value) return;
                r.RemainingTimeToTargetSOC = *field_value;
                r.RemainingTimeToTargetSOC_isUsed = 1;
                return;
            }            
            match_res = match_struct(e,"RemainingTimeToBulkSOC");
            if (match_res) {
                auto field_value = get_numerical_field<int8_t>(as_struct_ref(e));
                if (!field_value) return;
                r.RemainingTimeToBulkSOC = *field_value;
                r.RemainingTimeToBulkSOC_isUsed = 1;
                return;
            }            
            match_res = match_struct(e,"RemainingTimeToMinimumSOC");
            if (match_res) {
                auto field_value = get_numerical_field<int8_t>(as_struct_ref(e));
                if (!field_value) return;
                r.RemainingTimeToMinimumSOC = *field_value;
                r.RemainingTimeToMinimumSOC_isUsed = 1;
                return;
            }            
            match_res = match_struct(e,"ChargingComplete");
            if (match_res) {
                auto field_value = get_numerical_field<int>(as_struct_ref(e));
                if (!field_value) return;
                r.ChargingComplete = *field_value;
                r.ChargingComplete_isUsed = 1;
                return;
            }            
            match_res = match_struct(e,"BulkChargingComplete");
            if (match_res) {
                auto field_value = get_numerical_field<int>(as_struct_ref(e));
                if (!field_value) return;
                r.BulkChargingComplete = *field_value;
                r.BulkChargingComplete_isUsed = 1;
                return;
            }            
            match_res = match_struct(e,"InletHot");
            if (match_res) {
                auto field_value = get_numerical_field<int>(as_struct_ref(e));
                if (!field_value) return;
                r.InletHot = *field_value;
                r.InletHot_isUsed = 1;
                return;
            }            
        });
        return r;
    }


    //
    // iso2CurrentDemandReqType
    //

    template<> iso2CurrentDemandReqType MessageBuilder::emit<iso2CurrentDemandReqType>(ceps::ast::Struct & msg){
        iso2CurrentDemandReqType r{};
        for_all_children(msg, [&](node_t e){            
            auto match_res = match_struct(e,"EVTargetEnergyRequest");
            if (match_res) {
                r.EVTargetEnergyRequest = emit<iso2PhysicalValueType>(as_struct_ref(e));
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
            match_res = match_struct(e,"DisplayParameters");
            if (match_res) {
                r.DisplayParameters = emit<iso2DisplayParametersType>(as_struct_ref(e));
                r.DisplayParameters_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"EVTargetCurrent");
            if (match_res) {
                r.EVTargetCurrent = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVTargetVoltage");
            if (match_res) {
                r.EVTargetVoltage = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVMaximumCurrent");
            if (match_res) {
                r.EVMaximumCurrent = emit<iso2PhysicalValueType>(as_struct_ref(e));
                r.EVMaximumCurrent_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"EVMaximumPower");
            if (match_res) {
                r.EVMaximumPower = emit<iso2PhysicalValueType>(as_struct_ref(e));
                r.EVMaximumPower_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"EVMaximumVoltage");
            if (match_res) {
                r.EVMaximumVoltage = emit<iso2PhysicalValueType>(as_struct_ref(e));
                r.EVMaximumVoltage_isUsed = 1;
                return;
            }
        });
        return r;
    }

    //
    // iso2MeterInfoType
    //
    template<> iso2MeterInfoType MessageBuilder::emit<iso2MeterInfoType>(ceps::ast::Struct & msg){
        iso2MeterInfoType r{};
        for_all_children(msg, [&](node_t e){
            auto match_res = match_struct(e,"MeterID");
            if (match_res) {
                auto field_value = get_string_field(as_struct_ref(e));
                if (!field_value) return;
                strncpy(r.MeterID.characters,field_value->c_str(), sizeof(r.MeterID.characters - 1)); 
                r.MeterID.charactersLen = field_value->length();
                return;
            }
            match_res = match_struct(e,"MeterReadingCharged");
            if (match_res) {
                auto field_value = get_numerical_field<uint64_t>(as_struct_ref(e));
                if (!field_value) return;
                r.MeterReadingCharged = *field_value;
                r.MeterReadingCharged_isUsed = 1;
                return;
            }            
            match_res = match_struct(e,"MeterReadingDischarged");
            if (match_res) {
                auto field_value = get_numerical_field<uint64_t>(as_struct_ref(e));
                if (!field_value) return;
                r.MeterReadingDischarged = *field_value;
                r.MeterReadingDischarged_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"SigMeterReading");
            if (match_res) {
                r.SigMeterReading.bytesLen = write_bytes(as_struct_ptr(e),r.SigMeterReading.bytes, 
                                                        r.SigMeterReading.bytes + sizeof(r.SigMeterReading.bytes));
                r.SigMeterReading_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"MeterStatus");
            if (match_res) {
                auto field_value = get_numerical_field<int16_t>(as_struct_ref(e));
                if (!field_value) return;
                r.MeterStatus = *field_value;
                r.MeterStatus_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"TMeter");
            if (match_res) {
                auto field_value = get_numerical_field<int64_t>(as_struct_ref(e));
                if (!field_value) return;
                r.TMeter = *field_value;
                r.TMeter_isUsed = 1;
                return;
            }
        });
        return r;
    }            

    //
    // iso2CurrentDemandResType
    //

    template<> iso2CurrentDemandResType MessageBuilder::emit<iso2CurrentDemandResType>(ceps::ast::Struct & msg){
        iso2CurrentDemandResType r{};
        evse_prolog(r,msg);
        for_all_children(msg, [&](node_t e){            
            auto match_res = match_struct(e,"EVSEPresentCurrent");
            if (match_res) {
                r.EVSEPresentCurrent = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVSEPresentVoltage");
            if (match_res) {
                r.EVSEPresentVoltage = emit<iso2PhysicalValueType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"EVSEPowerLimitAchieved");
            if (match_res) {
                auto field_value = get_numerical_field<int>(as_struct_ref(e));
                if (!field_value) return;
                r.EVSEPowerLimitAchieved = *field_value;
                return;
            }            
            match_res = match_struct(e,"EVSECurrentLimitAchieved");
            if (match_res) {
                auto field_value = get_numerical_field<int>(as_struct_ref(e));
                if (!field_value) return;
                r.EVSECurrentLimitAchieved = *field_value;
                return;
            }            
            match_res = match_struct(e,"EVSEVoltageLimitAchieved");
            if (match_res) {
                auto field_value = get_numerical_field<int>(as_struct_ref(e));
                if (!field_value) return;
                r.EVSEVoltageLimitAchieved = *field_value;
                return;
            } 
            match_res = match_struct(e,"EVSEMaximumPower");
            if (match_res) {
                r.EVSEMaximumPower = emit<iso2PhysicalValueType>(as_struct_ref(e));
                r.EVSEMaximumPower_isUsed = 1;
                return;
            }         
            match_res = match_struct(e,"EVSEMaximumCurrent");
            if (match_res) {
                r.EVSEMaximumCurrent = emit<iso2PhysicalValueType>(as_struct_ref(e));
                r.EVSEMaximumCurrent_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"EVSEMaximumVoltage");
            if (match_res) {
                r.EVSEMaximumVoltage = emit<iso2PhysicalValueType>(as_struct_ref(e));
                r.EVSEMaximumVoltage_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"EVSEID");
            if (match_res) {
                auto field_value = get_string_field(as_struct_ref(e));
                if (!field_value) return;
                strncpy(r.EVSEID.characters,field_value->c_str(), sizeof(r.EVSEID.characters - 1)); 
                r.EVSEID.charactersLen = field_value->length();
                return;
            }
            match_res = match_struct(e,"SAScheduleTupleID");
            if (match_res) {
                auto field_value = get_numerical_field<uint8_t>(as_struct_ref(e));
                if (!field_value) return;
                r.SAScheduleTupleID = *field_value;
                r.SAScheduleTupleID_isUsed = 1;
                return;
            } 
            match_res = match_struct(e,"MeterInfo");
            if (match_res) {
                r.MeterInfo = emit<iso2MeterInfoType>(as_struct_ref(e));
                r.MeterInfo_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"ReceiptRequired");
            if (match_res) {
                auto field_value = get_numerical_field<int32_t>(as_struct_ref(e));
                if (!field_value) return;
                r.ReceiptRequired = *field_value;
                r.ReceiptRequired_isUsed = 1;
                return;
            }
        });
        return r;
    }

    //
    // MeteringReceiptReq
    //

    template<> iso2MeteringReceiptReqType MessageBuilder::emit<iso2MeteringReceiptReqType>(ceps::ast::Struct & msg){
        iso2MeteringReceiptReqType r{};
        for_all_children(msg, [&](node_t e){
            auto match_res = match_struct(e,"Id");
            if (match_res) {
                auto field_value = get_string_field(as_struct_ref(e));
                if (!field_value) return;
                strncpy(r.Id.characters,field_value->c_str(), sizeof(r.Id.characters - 1)); 
                r.Id.charactersLen = field_value->length();
                return;
            }
            match_res = match_struct(e,"SessionID");
            if (match_res) {
                r.SessionID.bytesLen = write_bytes(as_struct_ptr(e),r.SessionID.bytes, 
                                                        r.SessionID.bytes + sizeof(r.SessionID.bytes));
                return;
            }
            match_res = match_struct(e,"SAScheduleTupleID");
            if (match_res) {
                auto field_value = get_numerical_field<uint8_t>(as_struct_ref(e));
                if (!field_value) return;
                r.SAScheduleTupleID = *field_value;
                r.SAScheduleTupleID_isUsed = 1;
                return;
            }
            match_res = match_struct(e,"MeterInfo");
            if (match_res) {
                r.MeterInfo = emit<iso2MeterInfoType>(as_struct_ref(e));
                return;
            }
        });
        return r;
    }

    //
    // iso2MeteringReceiptResType
    //

    template<> iso2MeteringReceiptResType MessageBuilder::emit<iso2MeteringReceiptResType>(ceps::ast::Struct & msg){
        iso2MeteringReceiptResType r{};
        evse_prolog(r,msg);
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
        else if(name(ceps_struct)== "ChargeParameterDiscoveryRes")
         emit<iso2ChargeParameterDiscoveryResType>(ceps_struct);
        else if(name(ceps_struct)== "PowerDeliveryReq")
         emit<iso2PowerDeliveryReqType>(ceps_struct);
        else if(name(ceps_struct)== "PowerDeliveryRes")
         emit<iso2PowerDeliveryResType>(ceps_struct);
        else if(name(ceps_struct)== "CableCheckReq")
         emit<iso2CableCheckReqType>(ceps_struct);
        else if(name(ceps_struct)== "CableCheckRes")
         emit<iso2CableCheckResType>(ceps_struct);
        else if(name(ceps_struct)== "PreChargeReq")
         emit<iso2PreChargeReqType>(ceps_struct);
        else if(name(ceps_struct)== "PreChargeRes")
         emit<iso2PreChargeResType>(ceps_struct);
        else if(name(ceps_struct)== "CurrentDemandReq")
         emit<iso2CurrentDemandReqType>(ceps_struct);
        else if(name(ceps_struct)== "CurrentDemandRes")
         emit<iso2CurrentDemandResType>(ceps_struct);
        // MeteringReceiptReq / MeteringReceiptRes
        else if(name(ceps_struct)== "MeteringReceiptReq")
         emit<iso2MeteringReceiptReqType>(ceps_struct);
        else if(name(ceps_struct)== "MeteringReceiptRes")
         emit<iso2MeteringReceiptResType>(ceps_struct);

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

