// ESBMidConn.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ESBMidConn.h"


// This is an example of an exported variable
ESBMIDCONN_API int nESBMidConn=0;

// This is an example of an exported function.
ESBMIDCONN_API int fnESBMidConn(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see ESBMidConn.h for the class definition
CESBMidConn::CESBMidConn()
{
    return;
}
