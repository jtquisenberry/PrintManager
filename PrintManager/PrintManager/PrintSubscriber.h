#include "MapEx.h"
#include "JobInfo.h"
#include "ThreadInfo.h"
#include "LogFile.h"
#include <vector>

//FILE* g_fileApplication = NULL;  // Declared externally
//FILE* g_fileOutput = NULL;  // Declared externally
//int written = 0;

#pragma once
class PrintSubscriber
{
public:
	// Function declarations
	PrintSubscriber();   // standard constructor
	virtual ~PrintSubscriber();
	UINT Start(LPVOID pParam);
	CEvent* m_pEventThreadDone;
	CEvent* m_pEventStopRequested;
	// CThreadInfo m_ThreadInfo;
	CMapEx<int, int, CJobInfo*, CJobInfo*> m_mapJobInfo;

	HANDLE GetPrinter(void);
	HANDLE GetStopRequestedEvent(void);
	HANDLE PrintSubscriber::GetThreadDoneEvent(void);
	HWND PrintSubscriber::GetHwnd(void);
	void PrintSubscriber::SetPrinter(HANDLE hPrinter);
	void PrintSubscriber::SetStopRequestedEvent(HANDLE hEventStopRequested);
	void PrintSubscriber::SetThreadDoneEvent(HANDLE hEventThreadDone);
	void PrintSubscriber::SetHwnd(HWND hWnd);

	
    HANDLE m_hPrinter;
    HANDLE m_hEventStopRequested;
    HANDLE m_hEventThreadDone;
    HWND m_hWnd;

	std::vector<int>* m_PrintStack;

};
