#ifndef _ESB_UTILS_SAFECODING_FINALIZE_H_
#define _ESB_UTILS_SAFECODING_FINALIZE_H_

#include <functional>
namespace Utils
{
	namespace SafeCoding
	{
		class CFinalize
		{
		private:
			std::function<void()> _fincall;
		public:
			inline CFinalize(const std::function<void()>& _finalize) : _fincall(_finalize)
			{
			}
			inline ~CFinalize()
			{
				_fincall();
			}
		};
	};
};


#endif // _ESB_UTILS_SAFECODING_FINALIZE_H_
