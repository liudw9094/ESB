#pragma once

struct SAppConfig
{
	int nPort;
	bool bAuthentication;
	ESBWebService::SAuthentication authentication;
	SAppConfig() :
		nPort(13000),
		bAuthentication(false)
	{
	}
};

class CAppConfig
{
private:
	std::wstring	m_szFilePath;
	SAppConfig		m_cfg;
public:
	explicit CAppConfig(const wchar_t *path);
	~CAppConfig();
	BOOL Load();
	BOOL Save();
	SAppConfig&& GetConfig();
};

