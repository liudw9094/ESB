#pragma once

extern const GUID SERVICE_GUID;

struct SAppConfig
{

	std::wstring szHubURL;
	GUID szServiceGUID;
	bool bAuthentication;
	ESBWebService::SAuthentication authentication;
	SAppConfig()
		: szHubURL(L"127.0.0.1:13000"),
		szServiceGUID(SERVICE_GUID)
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

