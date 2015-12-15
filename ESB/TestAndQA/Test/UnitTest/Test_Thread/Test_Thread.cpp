// Test_Thread.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Utils/Thread/Thread.h>
#include <iostream>
#include <string>
#include <time.h>


#include <iomanip>
#include <string>
#include <codecvt>
#include <iostream>
#include <fstream>
#include <string>
#include <locale>

#ifdef _DEBUG
#pragma comment(lib, "UtilsRuntimeD")
#else
#pragma comment(lib, "UtilsRuntime")
#endif
using namespace std;
using namespace Utils::Thread;
int main()
{
	{
		// UTF8 UTF16 test
		const wchar_t adfas[] = L"fdsdsf没有z\u00df\u6c34\U0001d10b";
		wstring wstr = adfas;
		std::wstring_convert<codecvt_utf8_utf16<wchar_t>, wchar_t> conv8;
		std::string s1 = conv8.to_bytes(wstr);
		wstring ws1 = conv8.from_bytes(s1);
		wcout << ws1 << endl;
	}

	string s;
	IThread* th = CreateThread();
	th->Invoke([th]()
	{
		cout << "hello!" << endl;
		th->Invoke([]()
		{
			cout << "hello 2!" << endl;
		});
	});

	{
		SREF(IAsynTask) asab[1000];
		for (int i = 0; i < 1000; ++i)
		{
			asab[i] = th->AsynInvoke([i]()
			{
				cout << "hello! iii = " << i << endl;
			});
		}
		for (int i = 0; i < 1000; ++i)
			asab[i]->Cancle();
	}
	cin >> s;
	{
		SREF(IAsynTask) asa[1000];
		for (int i = 0; i < 1000; ++i)
		{
			asa[i] = th->AsynInvoke([i]()
			{
				time_t tm = time(NULL);
				//while (time(NULL) - tm < 1);
				Sleep(1000);
				cout << "hello! This is tast " << i + 1 << "!" << endl;
			});

		}
		Sleep(5000);
		//time_t tm = time(NULL);
		//while (time(NULL) - tm < 5);
		th->Dispose();
	}
	cin >> s;
	cout << "Testing task creation and cancelation. Press <ESC> to finish."<<endl;
	IThread* th1 = CreateThread();
	for (;;)
	{
		int d = 0;
		SREF(IAsynTask) asab[1000];
		for (int i = 0; i < 1000; ++i, ++d)
		{
			asab[i] = th1->AsynInvoke([d]()
			{
				//cout << "hello! iii = " << d << endl;
			});
		}
		for (int i = 0; i < 1000; ++i)
			asab[i]->Cancle();
		if (::GetKeyState(VK_ESCAPE) & 0x8000)
			break;
	}
	th1->Dispose();
	cout << "All task finished!" << endl;
	cin >> s;
	for (;;)
	{
		SREF(IThread) th1 = CreateThread();
		if (::GetKeyState(VK_ESCAPE) & 0x8000)
			break;
	}

	cin >> s;
    return 0;
}

