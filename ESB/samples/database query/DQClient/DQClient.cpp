// DQClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DQClient.h"

using namespace ESBMidClient;
using namespace std;

CDQClientApp::CDQClientApp()
	: m_spConnection(CreateESBConnection()),
	m_appCfg(L".\\DQClient.xml")
{

}

CDQClientApp::~CDQClientApp()
{

}

BOOL CDQClientApp::OnInitialization()
{
	InitCallbacks();

	wcout << L"Loading configuration files..." << endl;
	if (!m_appCfg.Load())
	{
		wcerr << L"Failed to load configuration file!" << endl;
		//return FALSE;
	}
	SAppConfig config = m_appCfg.GetConfig();

	wcout << L"Connecting to service..." << endl;
	if (m_spConnection->StartSession(config.szHubURL, config.szServiceGUID) != 0)
	{
		wcerr << L"Failed to connect to service at URL \"" << config.szHubURL << L"\"" << endl;
		return FALSE;
	}

	wcout << L"Connected successfully!" << endl;

	wstring command;
	wstring commandOrig;
	for (;;)
	{
		std::getline(std::wcin, commandOrig);
		command = commandOrig;
		locale loc;
		for (auto elem : command)
			std::tolower(elem, loc);

		if (command == L"quit")
			break;
		else
		{
			wstring result;
			int error = 0;
			if ((error = m_spConnection->Send(commandOrig, result)) == 0)
			{
				result =	L"===================================\n" +
							result +
							L"\n===================================";
				wcout << result << endl;
			}
			else
			{
				wcerr << L"Error occured: " << error << endl;
				wcerr << L"Error content: " << result << endl;
			}
		}
	}

	return FALSE;
}

void CDQClientApp::OnFinalization()
{
	m_spConnection->EndSession();
	wcout << L"Enter any string to finish." << endl;
	getchar();
}

void CDQClientApp::InitCallbacks()
{

}

int main()
{
	::CoInitialize(NULL);
	CDQClientApp().Run();
	::CoUninitialize();
	return 0;
}