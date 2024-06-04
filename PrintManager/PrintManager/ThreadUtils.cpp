#include "stdafx.h"
#include "ThreadUtils.h"

void ThreadUtils::OutputThreadId(CString strFunctionName, FILE* log_file)
{
	// Print thread ID
	wchar_t buffer[100];
	int cx = 0;
	std::thread::id this_id = std::this_thread::get_id();
	cx = swprintf(buffer, 100, L"Thread ID: %d \n", *(int*)&this_id);
	OutputDebugString(L"\n\n");
	OutputDebugString(strFunctionName + "\n");
	OutputDebugString(buffer);
	OutputDebugString(L"\n\n");
	cx = fwprintf_s(log_file, L"%- 70s %s", (LPCWSTR)strFunctionName, buffer);
	int error = GetLastError();
	fflush(log_file);
}
