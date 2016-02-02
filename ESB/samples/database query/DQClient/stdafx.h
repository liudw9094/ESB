// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <locale> 
#include <windows.h>

#include <Utils/Utils.h>
#include <ESBMidClient/ESBMidClient.h>
#include <XMLParser/XMLParser.h>
#include <ESBDataSerializer/Serializer.h>


#ifdef _DEBUG
#pragma comment(lib, "ESBMidConnD")
#pragma comment(lib, "UtilsRuntimeD")
#pragma comment(lib, "XMLParserD")
#pragma comment(lib, "ESBDataSerializerD")
#else
#pragma comment(lib, "ESBMidConn")
#pragma comment(lib, "UtilsRuntime")
#pragma comment(lib, "XMLParser")
#pragma comment(lib, "ESBDataSerializer")
#endif // _DEBUG