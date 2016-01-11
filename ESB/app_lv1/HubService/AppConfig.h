#pragma once

struct SAppConfig
{
	int nPort;
	SAppConfig() :
		nPort(13000)
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

