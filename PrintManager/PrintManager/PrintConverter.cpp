#include "stdafx.h"
#include "PrintConverter.h"
#include "windows.h"


PrintConverter::PrintConverter()
{
	m_pEventThreadDone = new CEvent(TRUE, TRUE);     // signaled
	m_pEventStopRequested = new CEvent(FALSE, TRUE); // non-signaled
	m_boolKeepRunning = true;

	m_hPrinter = INVALID_HANDLE_VALUE;
	m_hEventStopRequested = INVALID_HANDLE_VALUE;
	m_hEventThreadDone = INVALID_HANDLE_VALUE;
	m_hWnd = NULL;


	return;
}


PrintConverter::~PrintConverter()
{

}

UINT PrintConverter::Start(LPVOID pParam)
{
	while (m_boolKeepRunning)
	{
		if (m_PrintStack->size() > 0)
		{
			int job_id = 0;
			job_id = m_PrintStack->back();

			// Print thread ID
			wchar_t buffer[100];
			int cx;
			cx = swprintf(buffer, 100, L"job_id ID: %d \n", job_id);
			OutputDebugString(buffer);
			m_PrintStack->pop_back();
			int aaa = 0;
			aaa++;
		}

		Sleep(2000);
	}

	return 0;
}