// DQServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DQServer.h"

using namespace ESBMidService;
using namespace ESBWebService;
using namespace std;

CDQServerApp::CDQServerApp()
	: m_spService(CreateESBService()),
	m_appCfg(L".\\DQServer.xml")
{

}

CDQServerApp::~CDQServerApp()
{

}

BOOL CDQServerApp::OnInitialization()
{
	InitCallbacks();

	if(!m_appCfg.Load())
	{
		wcerr << L"Failed to load configuration file." << endl;
		//return FALSE;
	}
	SAppConfig config = m_appCfg.GetConfig();

	if (!m_dbConnection.Connect(config.dbConnection))
	{
		wcerr << L"Failed to connect to database." << endl;
		return FALSE;
	}

	if (!m_spService->Start(config.nPort))
	{
		wcerr << L"Failed to start service at port " << config.nPort << L"." << endl;
		return FALSE;
	}

	if(FAILED(m_spService->RegisterToHub(config.hubConnection.szHubURL,
		config.hubConnection.szServiceURL,
		config.hubConnection.szServiceGUID,
		config.hubConnection.szServiceName,
		10)))
	{
		wcerr << L"Failed to register the service on the hub." << endl;
		return FALSE;
	}
	return TRUE;
}

void CDQServerApp::OnFinalization()
{
	m_spService->Stop();
	wcout << L"Press any key to finish." << endl;
}

void CDQServerApp::InitCallbacks()
{
	m_spService->SetCallback_OnStarted([](IESBWebServiceServer *sender) {
		wcout << L"Service started." << endl;
	});

	m_spService->SetCallback_OnStoped([](IESBWebServiceServer *sender) {
		wcout << L"Service stoped." << endl;
	});

	m_spService->SetCallback_OnClientInvoke([this](SREF(Utils::Thread::IThread) pthread,
	struct soap* psoap,
		const std::wstring& wsSession,
		const std::wstring& wsInputs,
		std::wstring& wsResults) -> int
	{
		if (!m_dbConnection.IsConnected())
		{
			wsResults = L"Database invalid.";
			return 1001;
		}
		else
		{
			try
			{
				wsResults = m_dbConnection.Execute(wsInputs);
			}
			catch (...)
			{
				wsResults = L"Error!";
				return 1050;
			}
		}
		return 0;
	});
}

int main()
{
	::CoInitialize(NULL);
	CDQServerApp().Run();
	::CoUninitialize();
    return 0;
}

