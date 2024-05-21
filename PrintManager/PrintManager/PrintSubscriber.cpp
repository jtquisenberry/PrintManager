#include "stdafx.h"
#include "PrintSubscriber.h"
#include "JobInfo.h"


PrintSubscriber::PrintSubscriber()
{
	m_pEventThreadDone = new CEvent(TRUE, TRUE);     // signaled
	m_pEventStopRequested = new CEvent(FALSE, TRUE); // non-signaled

	m_hPrinter = INVALID_HANDLE_VALUE;
	m_hEventStopRequested = INVALID_HANDLE_VALUE;
	m_hEventThreadDone = INVALID_HANDLE_VALUE;
	m_hWnd = NULL;

	
	return;
}


PrintSubscriber::~PrintSubscriber()
{

}

HANDLE PrintSubscriber::GetPrinter(void)
{
	return m_hPrinter;
}

HANDLE PrintSubscriber::GetStopRequestedEvent(void)
{
	return m_hEventStopRequested;
}

HANDLE PrintSubscriber::GetThreadDoneEvent(void)
{
	return m_hEventThreadDone;
}

HWND PrintSubscriber::GetHwnd(void)
{
	return m_hWnd;
}

//================================================================

void PrintSubscriber::SetPrinter(HANDLE hPrinter)
{
	m_hPrinter = hPrinter;
}

void PrintSubscriber::SetStopRequestedEvent(HANDLE hEventStopRequested)
{
	m_hEventStopRequested = hEventStopRequested;
}

void PrintSubscriber::SetThreadDoneEvent(HANDLE hEventThreadDone)
{
	m_hEventThreadDone = hEventThreadDone;
}

void PrintSubscriber::SetHwnd(HWND hWnd)
{
	m_hWnd = hWnd;
}

UINT PrintSubscriber::Start(LPVOID pParam)
{
	PPRINTER_NOTIFY_INFO pNotification = NULL;

	WORD JobFields[] =
	{
		JOB_NOTIFY_FIELD_PRINTER_NAME,         // 0x00
		JOB_NOTIFY_FIELD_MACHINE_NAME,         // 0x01
		JOB_NOTIFY_FIELD_PORT_NAME,            // 0x02
		JOB_NOTIFY_FIELD_USER_NAME,            // 0x03
		JOB_NOTIFY_FIELD_NOTIFY_NAME,          // 0x04
		JOB_NOTIFY_FIELD_DATATYPE,             // 0x05
		JOB_NOTIFY_FIELD_PRINT_PROCESSOR,      // 0x06
		JOB_NOTIFY_FIELD_PARAMETERS,           // 0x07
		JOB_NOTIFY_FIELD_DRIVER_NAME,          // 0x08
		JOB_NOTIFY_FIELD_DEVMODE,              // 0x09
		JOB_NOTIFY_FIELD_STATUS,               // 0x0A
		JOB_NOTIFY_FIELD_STATUS_STRING,        // 0x0B
		JOB_NOTIFY_FIELD_DOCUMENT,             // 0x0D
		JOB_NOTIFY_FIELD_PRIORITY,             // 0x0E
		JOB_NOTIFY_FIELD_POSITION,             // 0x0F
		JOB_NOTIFY_FIELD_SUBMITTED,            // 0x10
		JOB_NOTIFY_FIELD_START_TIME,           // 0x11
		JOB_NOTIFY_FIELD_UNTIL_TIME,           // 0x12
		JOB_NOTIFY_FIELD_TIME,                 // 0x13
		JOB_NOTIFY_FIELD_TOTAL_PAGES,          // 0x14
		JOB_NOTIFY_FIELD_PAGES_PRINTED,        // 0x15
		JOB_NOTIFY_FIELD_TOTAL_BYTES,          // 0x16
		JOB_NOTIFY_FIELD_BYTES_PRINTED         // 0x17
	};
	PRINTER_NOTIFY_OPTIONS_TYPE	Notifications[1] =
	{
		{
			JOB_NOTIFY_TYPE,
			0,
			0,
			0,
			sizeof(JobFields) / sizeof(JobFields[0]),
			JobFields
		},
	};
	PRINTER_NOTIFY_OPTIONS NotificationOptions =
	{
		2,
		PRINTER_NOTIFY_OPTIONS_REFRESH,
		sizeof(Notifications) / sizeof(Notifications[0]),
		Notifications
	};


	HANDLE hPrinter = GetPrinter();


	// get a handle to a printer change notification object.


	/* Set which notifications*/
	
	HANDLE hChange = FindFirstPrinterChangeNotification(GetPrinter(),
		PRINTER_CHANGE_ALL,
		0,
		&NotificationOptions);
	/*
	HANDLE hChange = FindFirstPrinterChangeNotification(GetPrinter(),
		PRINTER_CHANGE_WRITE_JOB,
		0,
		&NotificationOptions);
	*/

	DWORD dwChange;
	HANDLE aHandles[2];
	aHandles[0] = hChange;
	aHandles[1] = GetStopRequestedEvent();

	while (hChange != INVALID_HANDLE_VALUE)
	{
		// sleep until a printer change notification wakes this thread or the
		// event becomes set indicating it's time for the thread to end.
		WaitForMultipleObjects(2, aHandles, FALSE, INFINITE);

		if (WaitForSingleObject(hChange, 0U) == WAIT_OBJECT_0)
		{
			FindNextPrinterChangeNotification(hChange, &dwChange, &NotificationOptions, (LPVOID*)&pNotification);

			if (pNotification != NULL)
			{
				// if a notification overflow occurred,
				if (pNotification->Flags & PRINTER_NOTIFY_INFO_DISCARDED)
				{
					DWORD dwOldFlags = NotificationOptions.Flags;


					// PRINTER_CHANGE_SET_JOB
					int a = PRINTER_CHANGE_ADD_JOB;



					// we must refresh to continue
					NotificationOptions.Flags = PRINTER_NOTIFY_OPTIONS_REFRESH;

					FreePrinterNotifyInfo(pNotification);

					FindNextPrinterChangeNotification(hChange, &dwChange, &NotificationOptions, (LPVOID*)&pNotification);

					NotificationOptions.Flags = dwOldFlags;
				}

				// iterate through each notification
				for (DWORD x = 0; x < pNotification->Count; x++)
				{
					ASSERT(pNotification->aData[x].Type == JOB_NOTIFY_TYPE);

					CJobInfo* pJobInfo = NULL;

					// if the job info item does not exist, create a new one
					if (!m_mapJobInfo.Lookup(pNotification->aData[x].Id, pJobInfo))
					{
						pJobInfo = new CJobInfo(pNotification->aData[x].Id);

						m_mapJobInfo.SetAt(pNotification->aData[x].Id, pJobInfo);
					}

					ASSERT(pJobInfo != NULL);
					pJobInfo->UpdateInfo(&pNotification->aData[x]);

					int zzz = 0;

					
					// ::PostMessage(m_ThreadInfo.GetHwnd(), UDM_UPDATE_JOB_LIST, 0, 0);
				}

				int ddd = 0;
				
				POSITION pos = m_mapJobInfo.GetStartPosition();
				while (pos != NULL)
				{
					int nKey;
					CJobInfo* pJobInfo = new CJobInfo(NULL);

					//auto vvv = pJobInfo->BuildString();


					m_mapJobInfo.GetNextAssoc(pos, nKey, pJobInfo);
					int cx2 = pJobInfo->BuildString();

					OutputDebugString(L"\n\n");
					OutputDebugStringW(pJobInfo->GetString());
					OutputDebugString(L"\n\n");

					int written2 = 0;
					written2 = fwprintf_s(g_fileApplication, L"\n\n%s\n\n", pJobInfo->GetString());


					// Put JobId in stack

					int job_id = 0;
					job_id = pJobInfo->GetJobId();
					m_PrintStack->push_back(pJobInfo->GetJobId());
					int bbb = 0;
					bbb++;



					ASSERT(pJobInfo != NULL);

				}

			}

			FreePrinterNotifyInfo(pNotification);
			pNotification = NULL;
		}
		else if (WaitForSingleObject(GetStopRequestedEvent(), 0U) == WAIT_OBJECT_0)
		{
			FindClosePrinterChangeNotification(hChange);
			hChange = INVALID_HANDLE_VALUE;
		}
	}

	// Signal the event to let the primary thread know that this thread is done
	SetEvent(GetThreadDoneEvent());

	return 0;
}
