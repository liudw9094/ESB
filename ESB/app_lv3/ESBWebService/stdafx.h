// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <Objbase.h>
#include <locale>
#include <codecvt>
#include <string>
#include <Utils/Utils.h>
#include <Common/Common.h>

#ifdef _DEBUG
#pragma comment (lib, "UtilsRuntimeD")
#pragma comment (lib, "ESBCommonLibD")
#pragma comment (lib, "libeay32MTd")
#pragma comment (lib, "ssleay32MTd")
#else
#pragma comment (lib, "UtilsRuntime")
#pragma comment (lib, "ESBCommonLib")
#pragma comment (lib, "libeay32MT")
#pragma comment (lib, "ssleay32MT")
#endif

#define WITH_PURE_VIRTUAL
//#define WITH_OPENSSL