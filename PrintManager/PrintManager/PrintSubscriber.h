#include "MapEx.h"
#include "JobInfo.h"
#include "ThreadInfo.h"

#pragma once
class PrintSubscriber
{
public:
	// Function declarations
	UINT Start(LPVOID pParam);
	PrintSubscriber();   // standard constructor
	virtual ~PrintSubscriber();
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



};
