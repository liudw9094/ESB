#pragma once

extern const GUID SERVICE_GUID;

struct SAppConfig
{
	int nPort;
	struct SHubConnection
	{
		std::wstring szHubURL;
		std::wstring szServiceURL;
		GUID szServiceGUID;
		std::wstring szServiceName;
		int nMaximumSession;
		SHubConnection()
			: szHubURL(L"127.0.0.1:13000"),
			szServiceURL(L"127.0.0.1:14051"),
			szServiceGUID(SERVICE_GUID),
			szServiceName(L"DQServer"),
			nMaximumSession(10)
		{
		}
	} hubConnection;
	struct SDbConnection
	{
		std::wstring szConStr;
		std::wstring szUsername;
		std::wstring szPwd;
	} dbConnection;
	SAppConfig() :
		nPort(14051)
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
private:
	void _LoadHubConnection(const ESBXMLParser::IXMLNode* node);
	void _LoadDbConnection(const ESBXMLParser::IXMLNode* node);
};

