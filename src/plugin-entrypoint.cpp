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
    // iso2WeldingDetectionReqType
    //

    template<> iso2WeldingDetectionReqType MessageBuilder::emit<iso2WeldingDetectionReqType>(ceps::ast::Struct & msg){
        iso2WeldingDetectionReqType r{};
        return r;
    }

    //
    // iso2WeldingDetectionResType
    //

    template<> iso2WeldingDetectionResType MessageBuilder::emit<iso2WeldingDetectionResType>(ceps::ast::Struct & msg){
        iso2WeldingDetectionResType r{};
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
    // iso2chargingSessionType
    //

    template<> iso2chargingSessionType MessageBuilder::emit<iso2chargingSessionType>(ceps::ast::Struct & msg){
        iso2chargingSessionType r{};
        map<string,iso2chargingSessionType> str2enum { 
            {"Terminate",iso2chargingSessionType_Terminate},
            {"Pause",iso2chargingSessionType_Pause}};
        for_all_children(msg, [&] (node_t e){
            if (is<ceps::ast::Ast_node_kind::symbol>(e) && kind(as_symbol_ref(e)) == "v2gchargingSessionType" ){
                auto fit = str2enum.find(name(as_symbol_ref(e)));
                if (fit == str2enum.end()) return;
                r = fit->second;
            }
        });
        return r;
    }

    //
    // iso2SessionStopReqType
    //

    template<> iso2SessionStopReqType MessageBuilder::emit<iso2SessionStopReqType>(ceps::ast::Struct & msg){
        iso2SessionStopReqType r{};
        for_all_children(msg, [&](node_t e){
            auto match_res = match_struct(e,"ChargingSession");
            if (match_res) {
                r.ChargingSession = emit<iso2chargingSessionType>(as_struct_ref(e));
                return;
            }
        });        
        return r;
    }

    //
    // iso2SessionStopResType
    //

    template<> iso2SessionStopResType MessageBuilder::emit<iso2SessionStopResType>(ceps::ast::Struct & msg){
        iso2SessionStopResType r{};
        evse_prolog(r,msg);
        return r;
    }


    //
    // appHandAppProtocolType
    //

    template<> appHandAppProtocolType MessageBuilder::emit<appHandAppProtocolType>(ceps::ast::Struct & msg){
        appHandAppProtocolType r{};
        for_all_children(msg, [&](node_t e){
            auto match_res = match_struct(e,"ProtocolNamespace");
            if (match_res) {
                auto field_value = get_string_field(as_struct_ref(e));
                if (!field_value) return;
                strncpy(r.ProtocolNamespace.characters,field_value->c_str(), sizeof(r.ProtocolNamespace.characters - 1)); 
                r.ProtocolNamespace.charactersLen = field_value->length();
                return;
            }
            match_res = match_struct(e,"VersionNumberMajor");
            if (match_res) {
                auto field_value = get_numerical_field<uint32_t>(as_struct_ref(e));
                if (!field_value) return;
                r.VersionNumberMajor = *field_value;
                return;
            }
            match_res = match_struct(e,"VersionNumberMinor");
            if (match_res) {
                auto field_value = get_numerical_field<uint32_t>(as_struct_ref(e));
                if (!field_value) return;
                r.VersionNumberMinor = *field_value;
                return;
            }
            match_res = match_struct(e,"SchemaID");
            if (match_res) {
                auto field_value = get_numerical_field<uint8_t>(as_struct_ref(e));
                if (!field_value) return;
                r.SchemaID = *field_value;
                return;
            }
            match_res = match_struct(e,"Priority");
            if (match_res) {
                auto field_value = get_numerical_field<uint8_t>(as_struct_ref(e));
                if (!field_value) return;
                r.Priority = *field_value;
                return;
            }
        });        
        return r;
    }

    //
    // appHandAnonType_supportedAppProtocolReq
    //

    template<> appHandAnonType_supportedAppProtocolReq MessageBuilder::emit<appHandAnonType_supportedAppProtocolReq>(ceps::ast::Struct & msg){
        appHandAnonType_supportedAppProtocolReq r{};
        r.AppProtocol.arrayLen = 
            read_array<appHandAppProtocolType>(msg, r.AppProtocol, string{"AppProtocol"});          
        return r;
    }

    //
    // appHandresponseCodeType
    //

    template<> appHandresponseCodeType MessageBuilder::emit<appHandresponseCodeType>(ceps::ast::Struct & msg){
        appHandresponseCodeType r{};
        map<string,appHandresponseCodeType> str2enum { 
            {"OK_SuccessfulNegotiation",appHandresponseCodeType_OK_SuccessfulNegotiation},
            {"OK_SuccessfulNegotiationWithMinorDeviation",appHandresponseCodeType_OK_SuccessfulNegotiationWithMinorDeviation},
            {"Failed_NoNegotiation",appHandresponseCodeType_Failed_NoNegotiation}};
        for_all_children(msg, [&] (node_t e){
            if (is<ceps::ast::Ast_node_kind::symbol>(e) && kind(as_symbol_ref(e)) == "v2gappHandresponseCodeType" ){
                auto fit = str2enum.find(name(as_symbol_ref(e)));
                if (fit == str2enum.end()) return;
                r = fit->second;
            }
        });
        return r;
    }


    //
    // appHandAnonType_supportedAppProtocolRes
    //

    template<> appHandAnonType_supportedAppProtocolRes MessageBuilder::emit<appHandAnonType_supportedAppProtocolRes>(ceps::ast::Struct & msg){
        appHandAnonType_supportedAppProtocolRes r{};
        for_all_children(msg, [&](node_t e){
            auto match_res = match_struct(e,"ResponseCode");
            if (match_res) {
                r.ResponseCode = emit<appHandresponseCodeType>(as_struct_ref(e));
                return;
            }
            match_res = match_struct(e,"SchemaID");
            if (match_res) {
                auto field_value = get_numerical_field<uint8_t>(as_struct_ref(e));
                if (!field_value) return;
                r.SchemaID = *field_value;
                r.SchemaID_isUsed = 1;
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
        return nullptr;
    }
}

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

