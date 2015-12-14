// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the ESBWEBSERVICE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// ESBWEBSERVICE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef ESBWEBSERVICE_EXPORTS
#define ESBWEBSERVICE_API __declspec(dllexport)
#else
#define ESBWEBSERVICE_API __declspec(dllimport)
#endif

// This class is exported from the ESBWebService.dll
class ESBWEBSERVICE_API CESBWebService {
public:
	CESBWebService(void);
	// TODO: add your methods here.
};

extern ESBWEBSERVICE_API int nESBWebService;

ESBWEBSERVICE_API int fnESBWebService(void);
