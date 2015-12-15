// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the ESBHUBCLIENTLIB_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// ESBHUBCLIENTLIB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef ESBHUBCLIENTLIB_EXPORTS
#define ESBHUBCLIENTLIB_API __declspec(dllexport)
#else
#define ESBHUBCLIENTLIB_API __declspec(dllimport)
#endif

// This class is exported from the ESBHubClientLib.dll
class ESBHUBCLIENTLIB_API CESBHubClientLib {
public:
	CESBHubClientLib(void);
	// TODO: add your methods here.
};

extern ESBHUBCLIENTLIB_API int nESBHubClientLib;

ESBHUBCLIENTLIB_API int fnESBHubClientLib(void);
