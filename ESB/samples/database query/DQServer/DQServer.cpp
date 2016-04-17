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
	::CoInitialize(NULL);

	InitCallbacks();

	wcout << L"Loading configuration files..." << endl;
	if(!m_appCfg.Load())
	{
		wcerr << L"Failed to load configuration file!" << endl;
		//return FALSE;
	}
	SAppConfig config = m_appCfg.GetConfig();

	wcout << L"Connecting to database..." << endl;
	if (!m_dbConnection.Connect(config.dbConnection))
	{
		wcerr << L"Failed to connect to database!" << endl;
		//return FALSE;
	}


	wcout << L"Opening port..." << endl;

	BOOL bStarted = FALSE;
	if (config.bAuthentication)
		bStarted = m_spService->Start(config.nPort, &config.authentication);
	else
		bStarted = m_spService->Start(config.nPort);

	if (!bStarted)
	{
		wcerr << L"Failed to start service at port " << config.nPort << L"!" << endl;
		return FALSE;
	}


	wcout << L"Registering the service..." << endl;
	if (0 != m_spService->RegisterToHub(config.hubConnection.szHubURL,
		config.hubConnection.szServiceURL,
		config.hubConnection.szServiceGUID,
		config.hubConnection.szServiceName,
		config.hubConnection.nMaximumSession))
	{
		wcerr << L"Failed to register the service on the hub!" << endl;
		return FALSE;
	}

	wstring command;
	while (command != L"quit")
	{
		std::getline(std::wcin, command);
		locale loc;
		for (auto elem : command)
			std::tolower(elem, loc);
	}

	return FALSE;
}

void CDQServerApp::OnFinalization()
{
	m_spService->Stop();
	if(m_dbConnection.IsConnected())
		m_dbConnection.Disconnect();
	wcout << L"Press <Enter> to finish." << endl;
	getchar();


	::CoUninitialize();
}

void CDQServerApp::InitCallbacks()
{
	auto f_ShowState = [this]() {
		UINT curNum = m_spService->GetCurrentSessionNum();
		UINT maxNum = m_spService->GetMaximumSessionNum();
		wcout << L"Current connection state: " << curNum << "/" << maxNum << endl;
	};

	m_spService->SetCallback_OnStarted([](IESBWebServiceServer *sender) {
		wcout << L"Service started." << endl;
	});

	m_spService->SetCallback_OnStoped([](IESBWebServiceServer *sender) {
		wcout << L"Service stoped." << endl;
	});

	m_spService->SetCallback_OnClientSessionConfirmed([f_ShowState](IESBWebServiceServer *sender, const wstring& session) {
		wcout << L"Session " << session << " confirmed." << endl;
		f_ShowState();
	});

	m_spService->SetCallback_OnClientSessionEnd([f_ShowState](IESBWebServiceServer *sender, const wstring& session, EMSessionEndReason reason) {
		wcout << L"Session " << session << " end. Reason code: " << (int)reason << endl;
		f_ShowState();
	});

	m_spService->SetCallback_OnRegisteredOnHub([f_ShowState](IESBWebServiceServer *sender) {
		wcout << L"Successfully registered on the hub." << endl;
		f_ShowState();
	});

	m_spService->SetCallback_OnUnregisteredFromHub([](IESBWebServiceServer *sender) {
		wcout << L"Unregistered from the hub." << endl;
	});


	m_spService->SetCallback_OnHubSessionLost([](IESBWebServiceServer *sender) {
		wcout << L"Lost connection from the hub." << endl;
	});

	m_spService->SetCallback_OnClientInvoke([this](SREF(Utils::Thread::IThread) pthread,
	struct soap* psoap,
		const std::wstring& wsSession,
		const std::wstring& wsInputs,
		std::wstring& wsResults) -> int
	{
		wcout << L"Session " << wsSession << L": " << wsInputs << endl;
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

