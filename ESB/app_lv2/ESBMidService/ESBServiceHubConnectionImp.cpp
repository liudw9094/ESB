#include "stdafx.h"
#include "ESBServiceHubConnectionImp.h"

using namespace ESBCommon;

CESBServiceHubConnectionImp::CESBServiceHubConnectionImp()
{
}


CESBServiceHubConnectionImp::~CESBServiceHubConnectionImp()
{
}

int	CESBServiceHubConnectionImp::RegisterToHub(const std::wstring& wsHubURL,
	const std::wstring& wsServiceURL,
	const GUID guidService,
	const std::wstring& wsServiceName,
	UINT maximumSession)
{
	ESBService_RegisterToHub reg;
	reg.wsServiceURL = wsServiceURL;
	reg.guidService = guidService;
	reg.wsServiceName = wsServiceName;
	reg.maximumSession = maximumSession;
}

BOOL CESBServiceHubConnectionImp::IsValid()
{

}

int CESBServiceHubConnectionImp::ModifySessionLimitation(int nLimitation)
{

}

int CESBServiceHubConnectionImp::IncreaseSessionLoad()
{

}

int CESBServiceHubConnectionImp::DecreaseSessionLoad()
{

}

int CESBServiceHubConnectionImp::Unregister()
{
	if (!IsValid())
		return -1;
}