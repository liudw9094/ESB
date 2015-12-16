// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <map>
#include <list>
#include <time.h>


#include <ESBWebService/ESBWebService.h>
#include <ESBMidService/ESBMidService.h>
#include <ESBMidClient/ESBMidClient.h>
#include <Common/Common.h>
#include <ESBDataSerializer/Serializer.h>
#include <Utils/Utils.h>
#include <XMLParser/XMLParser.h>

#ifdef _DEBUG
#pragma comment (lib, "UtilsRuntimeD")
#pragma comment (lib, "ESBDataSerializerD")
#pragma comment (lib, "ESBWebServiceD")
#pragma comment (lib, "ESBMidServiceD")
#pragma comment (lib, "ESBCommonLibD")
#pragma comment (lib, "XMLParserD")
#else
#pragma comment (lib, "UtilsRuntime")
#pragma comment (lib, "ESBDataSerializer")
#pragma comment (lib, "ESBWebService")
#pragma comment (lib, "ESBMidService")
#pragma comment (lib, "ESBCommonLib")
#pragma comment (lib, "XMLParser")
#endif
