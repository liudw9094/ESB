// Test_Thread.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Thread/Thread.h>
#include <iostream>
#include <string>
#include <time.h>
#include <Windows.h>
#pragma comment(lib, "Thread")

using namespace std;
using namespace Thread;
int main()
{
	IThread* th = CreateThread();
	th->Invoke([th]()
	{
		cout << "hello!" << endl;
		th->Invoke([]()
		{
			cout << "hello 2!" << endl;
		});
	});
	IAsynTask* asa[1000];
	for (int i = 0; i < 1000; ++i)
	{
		asa[i] = th->AsynInvoke([i]()
		{
			time_t tm = time(NULL);
			//while (time(NULL) - tm < 1);
			Sleep(1000);
			cout << "hello! This is tast " << i+1 << "!" << endl;
		});

	}
	Sleep(5000);
	//time_t tm = time(NULL);
	//while (time(NULL) - tm < 5);
	th->Dispose();
	cout << "All task finished!" << endl;
	for (int i = 0; i < 1000; ++i)
		asa[i]->Dispose();
	string s;
	cin>>s;
    return 0;
}

