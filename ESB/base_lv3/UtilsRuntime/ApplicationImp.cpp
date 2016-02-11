#include "stdafx.h"
#include "UtilsRuntime.h"
#include <Utils/SafeCoding/Finalize.h>
#include <Utils/Thread/Application.h>
#include "ThreadImp.h"
#include "AppplicationImp.h"

using namespace std;
using namespace Utils::Thread;
using namespace Utils::SafeCoding;

extern thread_local Utils::Thread::IThread* g_pCurThread;

class Utils::Thread::CApplicationImp : public CThreadImp
{
private:
public:
	CApplicationImp();
	virtual ~CApplicationImp();

	virtual UINT Run();
	virtual void DispatchInvoke(WPARAM wparam, LPARAM lparam);
};


// CApplicationImp
CApplicationImp::CApplicationImp()
	: CThreadImp(CThreadImp::Init_AttachCurrentThread())
{
	//MSG msg;
	//::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);

	// Add thread information.
	if (g_pCurThread)
	{
		throw runtime_error("Invalid main thread information: main thread has already existed.");
	}
	g_pCurThread = this;
}

CApplicationImp::~CApplicationImp()
{

}

UINT CApplicationImp::Run()
{
	m_spDispatcher = CreateDispatcher([this](DWORD dwThreadID) -> BOOL {
		return ::PostThreadMessage(dwThreadID, MSG_INVOKE, 0, 0);
	});

	MSG msg;
	try
	{
		while (::GetMessage(&msg, NULL, 0, 0))
		{
			if (msg.message == MSG_INVOKE)
				DispatchInvoke(msg.wParam, msg.lParam);
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
	// Catch WM_QUIT exception for jumping out from the message looping.
	catch (const CExpWMQuit&)
	{
		// Jumped out and do nothing.
	}

	m_spDispatcher = NULL;
	//::CloseHandle(m_hThread);

	// Clean up thread information.
	g_pCurThread = NULL;

	return 0;
}

void CApplicationImp::DispatchInvoke(WPARAM wparam, LPARAM lparam)
{
	return CThreadImp::DispatchInvoke(wparam, lparam);
}

// CApplication
CApplication::CApplication()
	: m_pClassImp(new CApplicationImp)
{

}

CApplication::~CApplication()
{
	delete m_pClassImp;
}

UINT CApplication::Run()
{
	CFinalize fin([this]() {
		OnFinalization();
	});
	if (OnInitialization())
		return m_pClassImp->Run();
	else
		return 1;
}

void CApplication::Invoke(const std::function<void()> &func)
{
	return m_pClassImp->Invoke(func);
}

SREF(IAsynTask) CApplication::AsynInvoke(const std::function<void()> &func)
{
	return m_pClassImp->AsynInvoke(func);
}

void CApplication::DoEvents()
{
	return m_pClassImp->DoEvents();
}

SREF(IDispatcher) CApplication::GetDispatcher()
{
	return m_pClassImp->GetDispatcher();
}

Utils::Thread::ITimer* CApplication::NewTimer(unsigned long millisec_interval, bool bEnable/* = true*/)
{
	return m_pClassImp->NewTimer(millisec_interval, bEnable);
}

void CApplication::Dispose()
{
	delete this;
}

void CApplication::OnMessage()
{
	return m_pClassImp->DispatchInvoke(NULL, NULL);
}