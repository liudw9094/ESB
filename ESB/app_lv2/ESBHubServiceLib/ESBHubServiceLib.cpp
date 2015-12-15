// ESBHubServiceLib.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ESBHubServiceLib.h"


// This is an example of an exported variable
ESBHUBSERVICELIB_API int nESBHubServiceLib=0;

// This is an example of an exported function.
ESBHUBSERVICELIB_API int fnESBHubServiceLib(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see ESBHubServiceLib.h for the class definition
CESBHubServiceLib::CESBHubServiceLib()
{
    return;
}
