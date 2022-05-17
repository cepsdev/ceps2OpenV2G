# ceps2OpenV2G
## What it is for
*OpenV2G* [http://openv2g.sourceforge.net/] is the basis of many ISO 15118-2 implementations on the market today. *cepS* [https://github.com/cepsdev/machines4ceps] is a tool to write specifications and tests in a modern, i.e. formal and test-driven, way (cepS := reverse(Spec)). *ceps2OpenV2G* is a plugin which enables implementors of OpenV2G based solutions to use the feature-rich cepS framework for specification, testing and simulation.

## Status
- Transformation cepS => OpenV2G (100%)
- Transformation OpenV2G => cepS (5%)
- Feature complete in approx. 5 days.
- 
## How to install
- Install ceps (https://github.com/cepsdev/machines4ceps)
- Clone this repo. Assumption: repo ends up in the same directory which also contains ceps, machines4ceps, log4ceps.
- If necessary edit build_all.sh/rebuild_all.sh, i.e. set the values of CEPSCORE, MACHINES4CEPS, LOG4CEPS according to your configuration.
- $ cd repo
- $ ./rebuild_all.sh && ./run_test.sh test/simple_test.ceps
- Your terminal output should look like (modulo compiler version etc.):

```

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
[ 50%] Building CXX object CMakeFiles/ceps2openV2G-plugin.dir/src/plugin-entrypoint.cpp.o
[100%] Linking CXX shared library libceps2openV2G-plugin.so
[100%] Built target ceps2openV2G-plugin

Executing test/simple_test.ceps
==============================================================


generator::plugin_entrypoint:
        (STRUCT "EVCCID" (INT 1  )(INT 2  )(INT 3  )(INT 4  )(INT 5  )(INT 6  ))


generator::plugin_entrypoint:
        (STRUCT "ResponseCode" (SYMBOL "FAILED_MeteringSignatureNotValid" "v2gResponsCodeType" ))
        (STRUCT "EVSEID" (INT 1234567  ))
        (STRUCT "EVSETimeStamp" (INT 123456789  ))
        (STRUCT "EVSEStatus" (STRUCT "NotificationMaxDelay" (INT 11  ))(STRUCT "EVSENotification" (SYMBOL "ReNegotiation" "v2gEVSENotificationType" )))


```
