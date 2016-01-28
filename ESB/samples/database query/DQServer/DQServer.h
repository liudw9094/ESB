#pragma once

#include "AppConfig.h"
#include "DbCon.h"

class CDQServerApp : public Utils::Thread::CApplication
{
private:
	SREF(ESBMidService::IESBService)			m_spService;
	SREF(Utils::Thread::IThread)				m_frontThread;
	CAppConfig									m_appCfg;
	CDbCon										m_dbConnection;
public:
	CDQServerApp();
	~CDQServerApp();

	virtual BOOL OnInitialization();
	virtual void OnFinalization();
private:
	void InitCallbacks();
};
