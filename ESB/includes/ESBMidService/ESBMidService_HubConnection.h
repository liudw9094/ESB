#ifndef _INCLUDE_ESBMIDSERVICE_HUBCONNECTION_H_
#define _INCLUDE_ESBMIDSERVICE_HUBCONNECTION_H_

#include "../Utils/SafeCoding/IDisposable.h"

namespace ESBMidService
{
	class IESBServiceHubConnection
	{
	public:
		/*
		// TODO: remove the codes later.
		virtual int	RegisterToHub(const std::wstring& wsHubURL,
			const std::wstring& wsServiceURL,
			const GUID guidService,
			const std::wstring& wsServiceName,
			UINT maximumSession) = 0;
		*/
		virtual int Unregister() = 0;
		virtual BOOL IsValid() const = 0;
		virtual UINT GetMaximumSessionNum() const = 0;
		virtual UINT GetCurrentSessionNum() const = 0;
		/*
		// TODO: remove the codes later.
		virtual int ModifySessionLimitation(int nLimitation) = 0;
		virtual int IncreaseSessionLoad() = 0;
		virtual int DecreaseSessionLoad() = 0;
		*/
	};

};

#endif //_INCLUDE_ESBMIDSERVICE_HUBCONNECTION_H_