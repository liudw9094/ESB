// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <iostream>

// ADO Database
#import "dll/win32/msado15.dll" no_namespace rename ("EOF", "adoEOF")
// ADO C++ Extensions
#include <icrsint.h>

#include <Utils/Utils.h>
#include <ESBMidService/ESBMidService.h>
#include <XMLParser/XMLParser.h>
#include <ESBDataSerializer/Serializer.h>

#ifdef _DEBUG
#pragma comment(lib, "ESBMidServiceD")
#pragma comment(lib, "UtilsRuntimeD")
#pragma comment(lib, "XMLParserD")
#pragma comment(lib, "ESBDataSerializerD")
#else
#pragma comment(lib, "ESBMidService")
#pragma comment(lib, "UtilsRuntime")
#pragma comment(lib, "XMLParser")
#pragma comment(lib, "ESBDataSerializer")
#endif // _DEBUG

// TODO: reference additional headers your program requires here
