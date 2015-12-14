// ESBWebService.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ESBWebService.h"


// This is an example of an exported variable
ESBWEBSERVICE_API int nESBWebService=0;

// This is an example of an exported function.
ESBWEBSERVICE_API int fnESBWebService(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see ESBWebService.h for the class definition
CESBWebService::CESBWebService()
{
    return;
}
