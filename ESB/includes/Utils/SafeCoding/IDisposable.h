#ifndef _ESB_UTILS_SAFECODING_IDISPOSABLE_H_
#define _ESB_UTILS_SAFECODING_IDISPOSABLE_H_

namespace Utils
{
	namespace SafeCoding
	{
		class IDisposable
		{
		public:
			virtual void Dispose() = 0;
		};
	};
}

#endif // _ESB_UTILS_SAFECODING_IDISPOSABLE_H_
