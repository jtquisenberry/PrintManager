#pragma once

#include <thread>


class ThreadUtils
{
public:
	static void OutputThreadId(CString, FILE*);
	static void OutputAddress(void*);

};

