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
	int PrintConverter::ConvertMain();
	void PrintConverter::SetSpoolDirectory();
	void PrintConverter::SetTempDirectory(CString);
	void PrintConverter::SetConverterFiles(int);
	CString PrintConverter::TimeToString();
	void PrintConverter::SetOutputPrinters(std::vector<CString>);
	
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
	CString m_strSpoolDirectory;
	CString m_strSpoolFile;
	CString m_strFqSpoolFile;
	CString m_strOutputDirectory;
	CString m_strOutputFile;
	CString m_strFqOutputFile;
	std::vector<CString> m_vectOutputPrinters;
};
