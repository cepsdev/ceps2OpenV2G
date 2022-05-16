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
}