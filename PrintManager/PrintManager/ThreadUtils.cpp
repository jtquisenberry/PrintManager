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
	error;
	fflush(log_file);
}


void ThreadUtils::OutputAddress(void* variable, CString text)
{
	wchar_t buffer[150];
	int cx = 0;
	cx = swprintf(buffer, 150, L"%s: %p \n", (LPCWSTR)text, variable);
	OutputDebugString(buffer);
}


void ThreadUtils::ErrorMessage(LPCTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process
	wchar_t* error_buffer = new wchar_t[300] {0};
	int cx;
	cx = swprintf(error_buffer, 300, L"%s failed with error %d: %s", lpszFunction, dw, (LPCTSTR)lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)error_buffer, TEXT("Error"), MB_OK);

	delete[] error_buffer;

	ExitProcess(dw);
}
