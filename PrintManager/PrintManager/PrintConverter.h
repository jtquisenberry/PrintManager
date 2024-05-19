#include "MapEx.h"
#include "JobInfo.h"
#include "ThreadInfo.h"
#include "LogFile.h"
#include <vector>

//FILE* g_fileApplication = NULL;  // Declared externally
//FILE* g_fileOutput = NULL;  // Declared externally
//int written = 0;

#pragma once
class PrintConverter
{
public:
	// Function declarations
	PrintConverter();   // standard constructor
	virtual ~PrintConverter();
	UINT Start(LPVOID pParam);
	CEvent* m_pEventThreadDone;
	CEvent* m_pEventStopRequested;
	// CThreadInfo m_ThreadInfo;
	CMapEx<int, int, CJobInfo*, CJobInfo*> m_mapJobInfo;
	std::vector<int> m_PrintStack;
	

	HANDLE m_hPrinter;
	HANDLE m_hEventStopRequested;
	HANDLE m_hEventThreadDone;
	HWND m_hWnd;

	bool m_boolKeepRunning;

};
