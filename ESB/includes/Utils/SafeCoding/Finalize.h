#ifndef _ESB_UTILS_SAFECODING_FINALIZE_H_
#define _ESB_UTILS_SAFECODING_FINALIZE_H_

template <class TCALL>
class CFinalize
{
private:
	TCALL _fincall;
public:
	CFinalize (TCALL _finalize) : _fincall(_finalize)
	{
	}
	~CFinalize()
	{
		_fincall();
	}
};

#endif // _ESB_UTILS_SAFECODING_FINALIZE_H_
