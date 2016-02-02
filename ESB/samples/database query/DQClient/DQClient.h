#pragma once

#include "AppConfig.h"

class CDQClientApp : public Utils::Thread::CApplication
{
private:
	SREF(ESBMidClient::IESBConnection)			m_spConnection;
	SREF(Utils::Thread::IThread)				m_frontThread;
	CAppConfig									m_appCfg;
public:
	CDQClientApp();
	~CDQClientApp();

	virtual BOOL OnInitialization();
	virtual void OnFinalization();
private:
	void InitCallbacks();
};
