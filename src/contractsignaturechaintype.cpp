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
}