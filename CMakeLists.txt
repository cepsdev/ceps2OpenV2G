cmake_minimum_required(VERSION 3.10)
project(ceps2OpenVG)

# add_compile_options(-O  -Wall -MD  -std=c++2a  -fPIC -static -Wno-undef -Wl,--whole-archive )
add_compile_options( -Wall -MD -std=c++2a )
add_link_options( -fPIC )

IF(NOT( DEFINED ENV{CEPSCORE}))
    MESSAGE(FATAL_ERROR "Could not find ceps core (Environment variable CEPSCORE not set).")
ENDIF()

IF(NOT( DEFINED ENV{MACHINES4CEPS}))
    MESSAGE(FATAL_ERROR "Could not find machines4ceps (Environment variable MACHINES4CEPS not set).")
ENDIF()

IF(NOT( DEFINED ENV{LOG4CEPS}))
    MESSAGE(FATAL_ERROR "Could not find log4ceps (Environment variable LOG4CEPS not set).")
ENDIF()

include_directories($ENV{CEPSCORE}/include)
include_directories($ENV{LOG4CEPS}/include)
include_directories($ENV{MACHINES4CEPS})
include_directories($ENV{MACHINES4CEPS}/core/src_gen/logging)
include_directories(include)
include_directories(../include)
include_directories(include/tests)

link_directories($ENV{CEPSCORE}/bin)

add_library(ceps2openV2G-plugin SHARED 
           src/plugin-entrypoint.cpp
           src/base.cpp
           src/parametertype.cpp
           src/sessionsetupreqtype.cpp
           src/sessionsetuprestype.cpp
           src/evsestatustype.cpp
           src/servicediscoveryreq.cpp
           src/servicetype.cpp
           src/servicelisttype.cpp
           src/servicediscoveryrestype.cpp
           src/servicedetailreqtype.cpp
           src/physicalvaluetype.cpp
           src/parametersettype.cpp
           src/serviceparameterlisttype.cpp
           src/servicedetailrestype.cpp
           src/paymentoptiontype.cpp
           src/selectedservicetype.cpp
           src/selectedservicelisttype.cpp
           src/paymentserviceselectionreqtype.cpp
           src/paymentserviceselectionrestype.cpp
           src/contractsignaturechaintype.cpp
           src/paymentdetailsreqtype.cpp
           src/paymentdetailsrestype.cpp
           src/authorizationreqtype.cpp
           src/evseprocessingtype.cpp
           src/authorizationrestype.cpp
           src/evenergytransferparametertype.cpp
           src/ac_evchargeparametertype.cpp
           src/ac_evbidirectionalparametertype.cpp
           src/dc_evchargeparametertype.cpp
           src/dc_evbidirectionalparametertype.cpp
           src/displayparameterstype.cpp
           src/currentdemandrestype.cpp
           src/currentdemandreqtype.cpp
           src/dc_evsebidirectionalparametertype.cpp
           src/chargeparameterdiscoveryreqtype.cpp
           src/meterinfotype.cpp
           src/chargeparameterdiscoveryrestype.cpp
           src/wpt_evchargeparametertype.cpp
           src/relativetimeintervaltype.cpp
           src/pmaxscheduleentrytype.cpp
           src/minimumpmaxrequesttype.cpp
           src/pmaxscheduletype.cpp
           src/sascheduletupletype.cpp
           src/saschedulelisttype.cpp
           src/evseenergytransferparametertype.cpp
           src/ac_evsechargeparametertype.cpp
           src/dc_evsechargeparametertype.cpp
           src/ac_evsebidirectionalparametertype.cpp
           src/wpt_evsechargeparametertype.cpp
           src/chargeprogresstype.cpp
           src/evoperationtype.cpp
           src/chargingprofiletype.cpp
           src/powerdeliveryreqtype.cpp
           src/powerdeliveryrestype.cpp
           src/cablecheckreqtype.cpp
           src/cablecheckrestype.cpp
           src/prechargereqtype.cpp
           src/prechargerestype.cpp
           src/meteringreceiptreqtype.cpp
           src/meteringreceiptrestype.cpp
           src/weldingdetectionreqtype.cpp
           src/weldingdetectionrestype.cpp
           src/chargingsessiontype.cpp
           src/sessionstopreqtype.cpp
           src/sessionstoprestype.cpp
           src/apphandappprotocoltype.cpp
           src/apphandanontype_supportedappprotocolreq.cpp
           src/apphandresponsecodetype.cpp
           src/apphandanontype_supportedappprotocolres.cpp
           )

target_link_libraries(ceps2openV2G-plugin cepscore sctp)
