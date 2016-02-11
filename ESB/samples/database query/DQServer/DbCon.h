#pragma once

#include "AppConfig.h"

class CDbCon
{
private:
	_ConnectionPtr							m_pConnection;
	//_CommandPtr								m_pCommand;
	//_RecordsetPtr							m_pRecordset;
	volatile BOOL							m_bConnected;
	SREF(Utils::Thread::ICriticalSection)	m_csDb;
public:
	CDbCon();
	~CDbCon();
	BOOL Connect(const SAppConfig::SDbConnection& config);
	void Disconnect();
	std::wstring Execute(const std::wstring& szCommand);
	BOOL IsConnected() const { return m_bConnected; };
private:
	BOOL _Connect(const std::wstring& szConnectionString,
		const std::wstring& szUsername,
		const std::wstring& szPassword);

};