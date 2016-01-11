// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"



#include <stdlib.h>
#include <iostream>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <Utils/SafeCoding/SmtPtr.h>
#include <XMLParser/XMLParser.h>
#include <Common/Common.h>

#ifdef _DEBUG
#pragma comment(lib, "UtilsRuntimeD")
#pragma comment(lib, "XMLParserD")
#pragma comment(lib, "ESBCommonLibD")
#else
#pragma comment(lib, "UtilsRuntime")
#pragma comment(lib, "XMLParser")
#pragma comment(lib, "ESBCommonLib")
#endif


#include <ESBDataSerializer/Serializer.h>

// TODO: reference additional headers your program requires here
