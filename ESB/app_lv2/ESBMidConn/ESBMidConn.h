// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the ESBMIDCONN_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// ESBMIDCONN_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef ESBMIDCONN_EXPORTS
#define ESBMIDCONN_API __declspec(dllexport)
#else
#define ESBMIDCONN_API __declspec(dllimport)
#endif

// This class is exported from the ESBMidConn.dll
class ESBMIDCONN_API CESBMidConn {
public:
	CESBMidConn(void);
	// TODO: add your methods here.
};

extern ESBMIDCONN_API int nESBMidConn;

ESBMIDCONN_API int fnESBMidConn(void);
