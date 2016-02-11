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

#ifdef _DEBUG
#pragma comment (lib, "UtilsRuntimeD")
#else
#pragma comment (lib, "UtilsRuntime")
#endif

#define WITH_PURE_VIRTUAL
//#define WITH_OPENSSL