#include "stdafx.h"
#include "DbCon.h"

using namespace std;

#define SAFE_COM_TRY_BEGIN try{

#ifdef _DEBUG
#define SAFE_COM_TRY_END(f) 	}															\
								catch(_com_error &e)										\
								{															\
									BSTR sz;												\
									e.ErrorInfo()->GetDescription(&sz);						\
									wstring sz1(e.ErrorMessage());							\
									sz1 += _T("\n");										\
									sz1 += sz;												\
									std::wcerr<<sz1<<endl;									\
									/*MessageBox(NULL, sz1.c_str(), _T("ERROR"), 0);	*/	\
									return (f);												\
								}															\
								catch(...)													\
								{															\
									return (f);												\
								}

#else

#define SAFE_COM_TRY_END(f) 	}															\
								catch(...)													\
								{															\
									return (f);												\
								}
#endif

#define FS_RETURN(e,f)	if(FAILED(e)) return(f);
#define FS_BOOL(e)		FS_RETURN((e),false)
#define FS_DWORD(e)		FS_RETURN((e),-1)

CDbCon::CDbCon()
	: m_bConnected(false),
	m_csDb(Utils::Thread::CreateCriticalSection())
{
	m_pConnection.CreateInstance(__uuidof(Connection));
}
CDbCon::~CDbCon()
{
	SLOCK(m_csDb);
	Disconnect();
	if(m_pConnection)
		m_pConnection.Release();
}

BOOL CDbCon::Connect(const SAppConfig::SDbConnection& config)
{
	SLOCK(m_csDb);
	if(!m_pConnection || m_bConnected)
		Disconnect();
	return _Connect(config.szConStr, config.szUsername, config.szPwd);
}

std::wstring CDbCon::Execute(const std::wstring& szCommand)
{
	SLOCK(m_csDb);
	wstring result, empty;
	try
	{
		_CommandPtr pCommand;
		FS_RETURN(pCommand.CreateInstance(__uuidof(Command)), empty);
		pCommand->PutActiveConnection(m_pConnection.GetInterfacePtr());
		pCommand->PutCommandText(_bstr_t(szCommand.c_str()));
		pCommand->PutCommandType(adCmdUnknown);
		pCommand->Parameters->Refresh();

		_variant_t recordAffected;
		_RecordsetPtr pRecordset = pCommand->Execute(&recordAffected, NULL, adCmdUnknown);

		long lRecAfec = recordAffected;
		if(lRecAfec > 0)
			result = result + to_wstring(lRecAfec) + L" item(s) effected.\n";

		if (pRecordset->State == adStateOpen)
		{
			Utils::SafeCoding::CFinalize fin([pRecordset] {
				pRecordset->Close();
			});

			long lFieldCount = pRecordset->Fields->GetCount();

			for (long i = 0; i < lFieldCount; ++i)
			{
				_variant_t var = pRecordset->Fields->GetItem(i)->GetName();
				wstring field;
				if (var.vt == VT_NULL)
					field = L"-";
				field = (const wchar_t *)_bstr_t(var);
				result = result + L"\t" + field;
			}
			result = result + L"\n";
			long long resultCount = 0;
			for (;VARIANT_FALSE == pRecordset->adoEOF; pRecordset->MoveNext(), ++resultCount)
			{
				_variant_t var;
				for (long i = 0; i < lFieldCount; ++i)
				{
					wstring field;
					if ((var = pRecordset->GetCollect(i)).vt == VT_NULL)
						field = L"-";
					field = (const wchar_t *)_bstr_t(var);
					result = result + L"\t" + field;
				}
				result = result + L"\n";
			}

			result = result + to_wstring(resultCount) + L" results.\n";
		}
	}
	catch(_com_error &e)
	{
		BSTR errDes;
		e.ErrorInfo()->GetDescription(&errDes);
		wstring errMsg(e.ErrorMessage());
		errMsg += _T("\n");
		errMsg += errDes;
		return errMsg;
	}
	catch(...)
	{
	}
	result = L"SQL Executed successfully!\n" + result;
	return result;
}
BOOL CDbCon::_Connect(const std::wstring& szConnectionString,
	const std::wstring& szUsername,
	const std::wstring& szPassword)
{
	SLOCK(m_csDb);
	if(!m_pConnection || m_bConnected)
		return false;
	SAFE_COM_TRY_BEGIN
		m_pConnection->Open(szConnectionString.c_str(), szUsername.c_str(), szPassword.c_str(), adModeUnknown);
	SAFE_COM_TRY_END(false)
	::InterlockedExchange((volatile long *)&m_bConnected, TRUE);
	return true;
}
void CDbCon::Disconnect()
{
	SLOCK(m_csDb);
	if(m_pConnection && m_bConnected)
	try
	{
		m_pConnection->Close();
	}
	catch(...)
	{
	}
	::InterlockedExchange((volatile long *)&m_bConnected, FALSE);
}