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

    bool chain(node_t , MessageBuilder* ){
        return true;
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

    
    optional<string> get_string_field(ceps::ast::Struct& msg){
        if (children(msg).size() != 1) return {};
        auto elem = children(msg)[0];
        if (is<ceps::ast::Ast_node_kind::string_literal>(elem))
            return value(as_string_ref(elem));
        return {};    
    }



    template<> int MessageBuilder::emit<int>(ceps::ast::Struct & msg){
        auto v = get_numerical_field<int>(msg);
        if (v) return *v;
        return 0;        
    }

    template<> uint8_t MessageBuilder::emit<uint8_t>(ceps::ast::Struct & msg){
        auto v = get_numerical_field<uint8_t>(msg);
        if (v) return *v;
        return 0;        
    }

    template<> int8_t MessageBuilder::emit<int8_t>(ceps::ast::Struct & msg){
        auto v = get_numerical_field<int8_t>(msg);
        if (v) return *v;
        return 0;        
    }

    template<> uint16_t MessageBuilder::emit<uint16_t>(ceps::ast::Struct & msg){
        auto v = get_numerical_field<uint16_t>(msg);
        if (v) return *v;
        return 0;        
    }

    template<> int16_t MessageBuilder::emit<int16_t>(ceps::ast::Struct & msg){
        auto v = get_numerical_field<int16_t>(msg);
        if (v) return *v;
        return 0;        
    }

    template<> String_t MessageBuilder::emit<String_t>(ceps::ast::Struct & msg){
        String_t r{};
        auto field_value = get_string_field(msg);
        if (!field_value) return r;
        strncpy(r.characters,field_value->c_str(), sizeof(r.characters - 1)); 
        r.charactersLen = field_value->length();
        return r;        
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

    std::pair<std::uint8_t*,size_t> MessageBuilder::build(ceps::ast::node_t data){
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
        // iso2WeldingDetectionReqType / iso2WeldingDetectionResType
        else if(name(ceps_struct)== "WeldingDetectionReq")
         emit<iso2WeldingDetectionReqType>(ceps_struct);
        else if(name(ceps_struct)== "WeldingDetectionRes")
         emit<iso2WeldingDetectionResType>(ceps_struct);        
        // iso2SessionStopReqType/iso2SessionStopResType
        else if(name(ceps_struct)== "SessionStopReq")
         emit<iso2SessionStopReqType>(ceps_struct);
        else if(name(ceps_struct)== "SessionStopRes")
         emit<iso2SessionStopResType>(ceps_struct);
        // supportedAppProtocolReq/supportedAppProtocolRes   
        else if(name(ceps_struct)== "supportedAppProtocolReq")
         emit<appHandAnonType_supportedAppProtocolReq>(ceps_struct);
        else if(name(ceps_struct)== "supportedAppProtocolRes")
         emit<appHandAnonType_supportedAppProtocolRes>(ceps_struct);
        else if(name(ceps_struct)== "physicalValue"){
         auto r = emit<iso2PhysicalValueType>(ceps_struct);
         auto res = new uint8_t[sizeof(r)];
         memcpy(res,(uint8_t*)&r,sizeof(r));
         return {res,sizeof(r)};         
        } else if (name(ceps_struct)== "AC_EVBidirectionalParameter" ){
         auto r = emit<iso2AC_EVBidirectionalParameterType>(ceps_struct);
         auto res = new uint8_t[sizeof(r)];
         memcpy(res,(uint8_t*)&r,sizeof(r));
         return {res,sizeof(r)};         
        }

        return {nullptr,0};
    }


    node_struct_t MessageBuilder::rec(string name, int v){
        auto r = mk_struct(name); auto r2 = mk_int_node(v); children(*r).push_back(r2);
        return r;    
   }

    node_struct_t MessageBuilder::rec(string name, int8_t v)
    {
        return rec(name,(int)v);
    }
   
    node_struct_t MessageBuilder::rec(string name, uint8_t v)
    {
        return rec(name,(int)v);
    }

    node_struct_t MessageBuilder::rec(string name, int16_t v)
    {
        return rec(name,(int)v);
    }

    node_struct_t MessageBuilder::rec(string name, uint16_t v)
    {
        return rec(name,(int)v);
    }

    node_struct_t MessageBuilder::rec(string name, int64_t v){
        auto r = mk_struct(name); auto r2 = mk_int64_node(v); children(*r).push_back(r2);
        return r;    
    }
    node_struct_t MessageBuilder::rec(string name, uint64_t v){
        auto r = mk_struct(name); auto r2 = mk_int64_node(v); children(*r).push_back(r2);
        return r;    
    }

   node_struct_t MessageBuilder::rec(string name, string v){
        auto r = mk_struct(name); auto r2 = mk_string(v); children(*r).push_back(r2);
        return r;    
   }
   node_struct_t MessageBuilder::rec(string name, node_struct_t e1){
        auto r = mk_struct(name);
        if (e1) children(*r).push_back(e1);
        return r;
    }

    node_struct_t MessageBuilder::rec(string name, vector<node_t> nodes)
    {
        auto r = mk_struct(name);
        children(*r) = nodes;
        return r;
    }

   node_struct_t MessageBuilder::rec(string name){
        auto r = mk_struct(name);
        return r;
    }
    node_struct_t MessageBuilder::rec(string name, AppProtocol_t v){
        auto r = mk_struct(name);
        for(auto it = v.array; it != v.array + v.arrayLen; ++it)
        {
            children(*r).push_back(rec("appHandAppProtocol",*it));
        }
        return r;
    }

}