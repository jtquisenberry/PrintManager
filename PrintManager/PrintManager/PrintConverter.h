#include "MapEx.h"
#include "JobInfo.h"
#include "ThreadInfo.h"
#include "ThreadUtils.h"
#include "LogFile.h"
#include <vector>


#pragma once
class PrintConverter
{
public:
	// Function declarations
	PrintConverter();   // standard constructor
	virtual ~PrintConverter();
	UINT Start(LPVOID pParam);
	void PrintConverter::SetStopRequestedEvent(HANDLE hEventStopRequested);
	void PrintConverter::SetThreadDoneEvent(HANDLE hEventThreadDone);
	HANDLE PrintConverter::GetStopRequestedEvent(void);
	HANDLE PrintConverter::GetThreadDoneEvent(void);
	
	// Thread control
	//CEvent* m_pEventSubscriberThreadDone;
	//CEvent* m_pEventSubscriberStopRequested;
	HANDLE m_hEventStopRequested;
	HANDLE m_hEventThreadDone;

	// Other objects
	CMapEx<int, int, CJobInfo*, CJobInfo*> m_mapJobInfo;
	std::vector<int>* m_PrintStack;
	HANDLE m_hPrinter;
	HWND m_hWnd;
	bool m_boolKeepRunning;

};
