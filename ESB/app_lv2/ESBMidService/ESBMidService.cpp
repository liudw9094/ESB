// ESBMidService.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ESBMidService.h"


// This is an example of an exported variable
ESBMIDSERVICE_API int nESBMidService=0;

// This is an example of an exported function.
ESBMIDSERVICE_API int fnESBMidService(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see ESBMidService.h for the class definition
CESBMidService::CESBMidService()
{
    return;
}
