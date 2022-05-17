# ceps2OpenV2G
## What it is for
*OpenV2G* [http://openv2g.sourceforge.net/] is the basis of many ISO 15118-2 implementations on the market today. *cepS* [https://github.com/cepsdev/machines4ceps] is a tool to write specifications and tests in a modern, i.e. formal and test-driven, way (cepS := reverse(Spec)). *ceps2OpenV2G* is a plugin which enables implementors of OpenV2G based solutions to use the feature-rich cepS framework for specification, testing and simulation.

## Status
- Transformation cepS => OpenV2G (100%)
- Transformation OpenV2G => cepS (5%)
- Feature complete in approx. 5 days.

## How to install
- Install ceps (https://github.com/cepsdev/machines4ceps)
- Clone this repo. Assumption: repo ends up in the same directory which also contains ceps, machines4ceps, log4ceps.
- If necessary edit build_all.sh/rebuild_all.sh, i.e. set the values of CEPSCORE, MACHINES4CEPS, LOG4CEPS according to your configuration.
- $ cd repo
- $ ./rebuild_all.sh && ./run_test.sh test/simple_test.ceps
- Your terminal output should look like (modulo compiler version etc.):

```

tomas@DESKTOP-4BO2QKJ:~/dev/ceps2OpenV2G$  ./rebuild_all.sh && ./run_test.sh test/physicalValue.ceps 
-- The C compiler identification is GNU 9.3.0
-- The CXX compiler identification is GNU 9.3.0
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: /home/tomas/dev/ceps2OpenV2G/bin
Scanning dependencies of target ceps2openV2G-plugin
[  1%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/servicediscoveryreq.cpp.o
[  5%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/base.cpp.o
[  4%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/servicelisttype.cpp.o
[  5%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/plugin-entrypoint.cpp.o
[  8%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/parametertype.cpp.o
[  9%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/sessionsetuprestype.cpp.o
[  9%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/sessionsetupreqtype.cpp.o
[ 11%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/evsestatustype.cpp.o
[ 12%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/servicetype.cpp.o
[ 14%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/servicediscoveryrestype.cpp.o
[ 15%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/servicedetailreqtype.cpp.o
[ 18%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/serviceparameterlisttype.cpp.o
[ 18%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/physicalvaluetype.cpp.o
[ 19%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/parametersettype.cpp.o
[ 21%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/servicedetailrestype.cpp.o
[ 22%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/paymentoptiontype.cpp.o
[ 23%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/selectedservicetype.cpp.o
[ 25%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/selectedservicelisttype.cpp.o
[ 26%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/paymentserviceselectionreqtype.cpp.o
[ 29%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/contractsignaturechaintype.cpp.o
[ 30%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/paymentdetailsreqtype.cpp.o
[ 30%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/paymentserviceselectionrestype.cpp.o
[ 32%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/paymentdetailsrestype.cpp.o
[ 33%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/authorizationreqtype.cpp.o
[ 35%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/evseprocessingtype.cpp.o
[ 38%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/evenergytransferparametertype.cpp.o
[ 38%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/authorizationrestype.cpp.o
[ 40%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/ac_evbidirectionalparametertype.cpp.o
[ 40%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/ac_evchargeparametertype.cpp.o
[ 42%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/dc_evchargeparametertype.cpp.o
[ 43%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/dc_evbidirectionalparametertype.cpp.o
[ 45%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/displayparameterstype.cpp.o
[ 46%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/currentdemandrestype.cpp.o
[ 47%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/currentdemandreqtype.cpp.o
[ 49%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/dc_evsebidirectionalparametertype.cpp.o
[ 50%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/chargeparameterdiscoveryreqtype.cpp.o
[ 52%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/meterinfotype.cpp.o
[ 53%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/chargeparameterdiscoveryrestype.cpp.o
[ 54%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/wpt_evchargeparametertype.cpp.o
[ 56%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/relativetimeintervaltype.cpp.o
[ 57%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/pmaxscheduleentrytype.cpp.o
[ 59%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/minimumpmaxrequesttype.cpp.o
[ 60%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/pmaxscheduletype.cpp.o
[ 61%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/sascheduletupletype.cpp.o
[ 63%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/saschedulelisttype.cpp.o
[ 64%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/evseenergytransferparametertype.cpp.o
[ 66%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/ac_evsechargeparametertype.cpp.o
[ 67%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/dc_evsechargeparametertype.cpp.o
[ 69%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/ac_evsebidirectionalparametertype.cpp.o
[ 70%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/wpt_evsechargeparametertype.cpp.o
[ 71%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/chargeprogresstype.cpp.o
[ 73%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/evoperationtype.cpp.o
[ 74%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/chargingprofiletype.cpp.o
[ 76%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/powerdeliveryreqtype.cpp.o
[ 77%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/powerdeliveryrestype.cpp.o
[ 78%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/cablecheckreqtype.cpp.o
[ 80%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/cablecheckrestype.cpp.o
[ 81%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/prechargereqtype.cpp.o
[ 83%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/prechargerestype.cpp.o
[ 84%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/meteringreceiptreqtype.cpp.o
[ 85%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/meteringreceiptrestype.cpp.o
[ 87%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/weldingdetectionreqtype.cpp.o
[ 88%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/weldingdetectionrestype.cpp.o
[ 90%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/chargingsessiontype.cpp.o
[ 91%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/sessionstopreqtype.cpp.o
[ 92%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/sessionstoprestype.cpp.o
[ 94%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/apphandappprotocoltype.cpp.o
[ 95%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/apphandanontype_supportedappprotocolreq.cpp.o
[ 97%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/apphandresponsecodetype.cpp.o
[ 98%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/apphandanontype_supportedappprotocolres.cpp.o
[100%] Linking CXX shared library libceps2openV2G-plugin.so
[100%] Built target ceps2openV2G-plugin

Executing test/physicalValue.ceps
==============================================================


PASSED.
```
