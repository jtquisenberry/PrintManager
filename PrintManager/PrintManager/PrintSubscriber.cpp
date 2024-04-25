#include "stdafx.h"
#include "PrintSubscriber.h"
#include "JobInfo.h"

PrintSubscriber::PrintSubscriber()
{

}

PrintSubscriber::~PrintSubscriber()
{

}

UINT PrintSubscriber::Worker()
{
	PPRINTER_NOTIFY_INFO pNotification = NULL;

	WORD JobFields[] =
	{
		JOB_NOTIFY_FIELD_PRINTER_NAME,
		JOB_NOTIFY_FIELD_MACHINE_NAME,
		JOB_NOTIFY_FIELD_PORT_NAME,
		JOB_NOTIFY_FIELD_USER_NAME,
		JOB_NOTIFY_FIELD_NOTIFY_NAME,
		JOB_NOTIFY_FIELD_DATATYPE,
		JOB_NOTIFY_FIELD_PRINT_PROCESSOR,
		JOB_NOTIFY_FIELD_PARAMETERS,
		JOB_NOTIFY_FIELD_DRIVER_NAME,
		JOB_NOTIFY_FIELD_DEVMODE,
		JOB_NOTIFY_FIELD_STATUS,
		JOB_NOTIFY_FIELD_STATUS_STRING,
		JOB_NOTIFY_FIELD_DOCUMENT,
		JOB_NOTIFY_FIELD_PRIORITY,
		JOB_NOTIFY_FIELD_POSITION,
		JOB_NOTIFY_FIELD_SUBMITTED,
		JOB_NOTIFY_FIELD_START_TIME,
		JOB_NOTIFY_FIELD_UNTIL_TIME,
		JOB_NOTIFY_FIELD_TIME,
		JOB_NOTIFY_FIELD_TOTAL_PAGES,
		JOB_NOTIFY_FIELD_PAGES_PRINTED,
		JOB_NOTIFY_FIELD_TOTAL_BYTES,
		JOB_NOTIFY_FIELD_BYTES_PRINTED
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

	// get a handle to a printer change notification object.
	HANDLE hChange = FindFirstPrinterChangeNotification(m_ThreadInfo.GetPrinter(),
		PRINTER_CHANGE_ALL,
		0,
		&NotificationOptions);

	DWORD dwChange;
	HANDLE aHandles[2];
	aHandles[0] = hChange;
	aHandles[1] = m_ThreadInfo.GetStopRequestedEvent();

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












					// ::PostMessage(m_ThreadInfo.GetHwnd(), UDM_UPDATE_JOB_LIST, 0, 0);
				}
			}

			FreePrinterNotifyInfo(pNotification);
			pNotification = NULL;
		}
		else if (WaitForSingleObject(m_ThreadInfo.GetStopRequestedEvent(), 0U) == WAIT_OBJECT_0)
		{
			FindClosePrinterChangeNotification(hChange);
			hChange = INVALID_HANDLE_VALUE;
		}
	}

	// Signal the event to let the primary thread know that this thread is done
	SetEvent(m_ThreadInfo.GetThreadDoneEvent());

	return 0;
}
