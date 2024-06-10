#include "stdafx.h"
#include "PrintConverter.h"
#include <thread>
#include "ThreadUtils.h"


PrintConverter::PrintConverter()
{
	m_pEventSubscriberThreadDone = new CEvent(TRUE, TRUE);     // signaled
	m_pEventSubscriberStopRequested = new CEvent(FALSE, TRUE); // non-signaled

	// Print thread ID
	ThreadUtils::OutputThreadId(L"PrintConverter::PrintConverter", g_fileSystem);

	m_boolKeepRunning = true;

	m_hPrinter = INVALID_HANDLE_VALUE;
	m_hEventStopRequested = INVALID_HANDLE_VALUE;
	m_hEventThreadDone = INVALID_HANDLE_VALUE;
	m_hWnd = NULL;
	m_PrintStack = NULL;

	return;
}


PrintConverter::~PrintConverter()
{
	// Print thread ID
	ThreadUtils::OutputThreadId(L"PrintConverter::~PrintConverter", g_fileSystem);

	delete m_pEventSubscriberThreadDone;
	delete m_pEventSubscriberStopRequested;
}


void PrintConverter::SetStopRequestedEvent(HANDLE hEventStopRequested)
{
	m_hEventStopRequested = hEventStopRequested;
}


void PrintConverter::SetThreadDoneEvent(HANDLE hEventThreadDone)
{
	m_hEventThreadDone = hEventThreadDone;
}


HANDLE PrintConverter::GetStopRequestedEvent(void)
{
	return m_hEventStopRequested;
}


HANDLE PrintConverter::GetThreadDoneEvent(void)
{
	return m_hEventThreadDone;
}



UINT PrintConverter::Start(LPVOID pParam)
{
	
	// Print thread ID
	ThreadUtils::OutputThreadId(L"PrintConverter::Start", g_fileSystem);
	
	// C4100 unreferenced formal parameter
	pParam;

	
	// If GetStopRequestedEvent is never set to true, the while clause
	// produces a memory leak.
	// D:\a\_work\1\s\src\vctools\VC7Libs\Ship\ATLMFC\Src\MFC\thrdcore.cpp(306) : {1357} client block at 0x0000017BE0A29E10, subtype c0, 136 bytes long.
    // D:\a\_work\1\s\src\vctools\VC7Libs\Ship\ATLMFC\Src\MFC\dumpcont.cpp(23) : atlTraceGeneral - a CWinThread object at $0000017BE0A29E10, 136 bytes long
	while (WaitForSingleObject(GetStopRequestedEvent(), 0U) != WAIT_OBJECT_0)
	{
		if (m_PrintStack->size() > 0)
		{
			int job_id = 0;
			job_id = m_PrintStack->back();

			// Print Job ID
			wchar_t buffer[100];
			int cx;
			cx = swprintf(buffer, 100, L"job_id ID: %d \n", job_id);
			OutputDebugString(buffer);
			m_PrintStack->pop_back();
		}

		Sleep(2000);

	}
	
	// Signal the event to let the primary thread know that this thread is done
	SetEvent(GetThreadDoneEvent());

	return 0;
}