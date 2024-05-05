// CTAB1.cpp : implementation file
//


#include "stdafx.h"
#include "PrintManager.h"
#include "afxdialogex.h"
#include "CTAB1.h"
#include "LogFile.h"
#include <iostream>
#include <vector>
#include <stdio.h>
#include <windows.h>
#include <strsafe.h>


std::vector<CString> printer_names;
std::vector<POSITION> printer_positions;
std::vector<int> printer_item_indexes;
std::vector<CString> printer_properties;
CString redirected_printer_name = "";
BOOL is_redirected = FALSE;
int written2 = 0;
//void GetSelectedPrinters();



// CTAB1 dialog

IMPLEMENT_DYNAMIC(CTAB1, CDialogEx)


int selected_printer_index = -1;





CTAB1::CTAB1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TAB1, pParent)
{
	// Print thread ID
	wchar_t buffer[100];
	int cx;
	std::thread::id this_id = std::this_thread::get_id();
	cx = swprintf(buffer, 100, L"Thread ID: %d \n", this_id);
	OutputDebugString(L"\n");
	OutputDebugString(L"\n");
	OutputDebugString(L"CTAB1::CTAB1\n");
	OutputDebugString(buffer);
	OutputDebugString(L"\n");
	OutputDebugString(L"\n");
	written2 = fwprintf_s(file1, L"%- 70s %s", L"CTAB1::CTAB1: ", buffer);
	fflush(file1);

	aaa++;
	aaa++;
}

CTAB1::~CTAB1()
{
}

void CTAB1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CB_PRINTERS2, m_cbPrinters);
	DDX_Control(pDX, IDC_LC_JOBINFO2, m_lcPrinters);

	// Map UI buttons to button variables
	DDX_Control(pDX, IDC_REDIRECT, m_btnRedirect);
	DDX_Control(pDX, IDC_CANCEL_REDIRECT2, m_btnCancelRedirect);
	DDX_Control(pDX, IDC_PAUSE_PRINTERS, m_btnPausePrinters);
	DDX_Control(pDX, IDC_RESUME_PRINTERS, m_btnResumePrinters);
	DDX_Control(pDX, IDC_PURGE_PRINTERS, m_btnPurgePrinters);
}

void CTAB1::StartWorkerThread()
{


}

void CTAB1::StopWorkerThread()
{
	// signal and wait for ThreadFunc() to end 
	m_pEventStopRequested->SetEvent();
	WaitForSingleObject(m_pEventThreadDone->m_hObject, 8000U);

	// if (m_ThreadInfo.GetPrinter() != INVALID_HANDLE_VALUE)
	//	ClosePrinter(m_ThreadInfo.GetPrinter());
}





BEGIN_MESSAGE_MAP(CTAB1, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTAB1::OnBnClickedOk)
	ON_NOTIFY(NM_RCLICK, IDC_LC_JOBINFO2, &CTAB1::OnNMRClickLcJobinfo2)
	ON_BN_CLICKED(IDC_REDIRECT, &CTAB1::OnBnClickedRedirect)
	ON_BN_CLICKED(IDC_PAUSE_PRINTERS, &CTAB1::OnBnClickedPausePrinters)
	ON_BN_CLICKED(IDC_RESUME_PRINTERS, &CTAB1::OnBnClickedResumePrinters)
	ON_BN_CLICKED(IDC_PURGE_PRINTERS, &CTAB1::OnBnClickedPurgePrinters)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PRINTERS_DEBUG, &CTAB1::OnPrintersDebug)
	ON_COMMAND(ID_PRINTERS_SAVE, &CTAB1::OnPrintersSave)
	ON_BN_CLICKED(IDC_CANCEL_REDIRECT2, &CTAB1::OnBnClickedCancelRedirect2)
END_MESSAGE_MAP()


// CTAB1 message handlers
BOOL CTAB1::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Print thread ID
	wchar_t buffer[100];
	int cx;
	std::thread::id this_id = std::this_thread::get_id();
	cx = swprintf(buffer, 100, L"Thread ID: %d \n", this_id);
	OutputDebugString(L"\n");
	OutputDebugString(L"\n");
	OutputDebugString(L"CTAB1::OnInitDialog\n");
	OutputDebugString(buffer);
	OutputDebugString(L"\n");
	OutputDebugString(L"\n");
	written2 = fwprintf_s(file1, L"%- 70s %s", L"CTAB1::OnInitDialog: ", buffer);
	fflush(file1);

	m_pEventThreadDone = new CEvent(TRUE, TRUE);     // signaled
	m_pEventStopRequested = new CEvent(FALSE, TRUE); // non-signaled

	// CThreadInfo
	// m_ThreadInfo.SetStopRequestedEvent(m_pEventStopRequested->m_hObject);
	// m_ThreadInfo.SetThreadDoneEvent(m_pEventThreadDone->m_hObject);
	// m_ThreadInfo.SetHwnd(GetSafeHwnd());
	
	return TRUE;
}





void CTAB1::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void CTAB1::OnNMRClickLcJobinfo2(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Right-click m_lcPrinters	
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int right_clicked_item_index = pNMItemActivate->iItem;
	selected_printer_index = right_clicked_item_index;
	int nColumns = m_lcPrinters.GetHeaderCtrl()->GetItemCount();

	wchar_t buffer[500];
	int cx;

	OutputDebugString(L"\n");
	OutputDebugString(L"RIGHT-CLICKED PRINTER");
	OutputDebugString(L"\n");
		
	int buffer_start_index = 0;
	for (int column_number = 0; column_number < nColumns; column_number++)
	{
		CString sItem = m_lcPrinters.GetItemText(right_clicked_item_index, column_number);
		cx = swprintf(&buffer[buffer_start_index], 500, L"%40s ", (LPCWSTR)sItem);
		buffer_start_index += cx;
	}

	if (buffer_start_index < 500)
	{
		buffer[buffer_start_index] = L'\0';
	}
	
	OutputDebugString(buffer);
	OutputDebugString(L"\n");

	*pResult = 0;
}


void CTAB1::OnBnClickedCancelRedirect()
{
	// TODO: Add your control notification handler code here
}


UINT StartPrintSubscriber(LPVOID pParam)
{
	/*
	int* pInt = (int*)pParam;
	int k = *pInt;
	CTAB1* pDlg = (CTAB1*)pParam;
	return pDlg->ThreadFunc2();
	*/
	CTAB1* ctab1 = (CTAB1*)pParam;
	ctab1->pPs.Start(0);
	//PrintSubscriber* pPs = (PrintSubscriber*)pParam;
	//return pPs->Start(0);
	return 0;
}




UINT ThreadFunc2(LPVOID pParam)
{
	/*
	int* pInt = (int*)pParam;
	int k = *pInt;
	CTAB1* pDlg = (CTAB1*)pParam;
	return pDlg->ThreadFunc2();
	*/

	PrintSubscriber* pPs = (PrintSubscriber*)pParam;
	return pPs->Start(0);
}

/*
UINT CTAB1::ThreadFunc2(void)
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
*/


void CTAB1::OnBnClickedRedirect()
{
	// Clicked Redirect Button
	
	// Disable Redirect button
	m_btnRedirect.EnableWindow(FALSE);
	
	// Enable Cancel button
	m_btnCancelRedirect.EnableWindow(TRUE);
	
	int redirected_printer_index = m_cbPrinters.GetCurSel();

	if (redirected_printer_index < 0)
	{
		return;
	}

	// Source printer is `redirected_printer_name`
	DWORD_PTR d = m_cbPrinters.GetItemData(redirected_printer_index);
	m_cbPrinters.GetLBText(redirected_printer_index, redirected_printer_name);

	// Destination printers are `printer_names`
	GetSelectedPrinters();


	// PausePrinter(redirected_printer_name);

	is_redirected = TRUE;
	



	/* Subscribe to printer status messages.*/
	m_mapJobInfo.Cleanup();

	// set the events to non-signaled
	m_pEventStopRequested->ResetEvent();
	m_pEventThreadDone->ResetEvent();

	// Open printer
	HANDLE hPrinter;
	OpenPrinter((LPTSTR)(LPCTSTR)redirected_printer_name, &hPrinter, NULL);

	// Setup thread
	// m_ThreadInfo.SetPrinter(hPrinter);


	//PrintSubscriber* ps = new PrintSubscriber();
	pPs.SetPrinter(hPrinter);
	pPs.SetStopRequestedEvent(m_pEventStopRequested->m_hObject);
	pPs.SetThreadDoneEvent(m_pEventThreadDone->m_hObject);
	pPs.SetHwnd(GetSafeHwnd());

	int eee = 0;


	// int xxx = 3;

	// Monitor thread
	m_pWinThread = AfxBeginThread(::StartPrintSubscriber, this);
	//m_pWinThread = AfxBeginThread(::ThreadFunc2, this);
	// m_pWinThread = AfxBeginThread(::ThreadFunc2, &xxx);
	// m_pWinThread = AfxBeginThread(::ThreadFunc2, ps);
	
	
	return;
}

void CTAB1::GetSelectedPrinters()
{
	printer_names.clear();
	printer_positions.clear();
	printer_item_indexes.clear();

	int nSelectedRows = m_lcPrinters.GetSelectedCount();
	int nColumns = m_lcPrinters.GetHeaderCtrl()->GetItemCount();

	// pos is a 1-indexed hex value indicating the position in the list from the top.
	POSITION pos = m_lcPrinters.GetFirstSelectedItemPosition();
	printer_positions.push_back(pos);
	int nItem = -1;
	if (pos != NULL)
	{
		while (pos)
		{
			// nItem is a 0-indexed value indicating the position in the list from the top.
			nItem = m_lcPrinters.GetNextSelectedItem(pos);
			printer_positions.push_back(pos);
			printer_item_indexes.push_back(nItem);

			for (int i = 0; i < nColumns; i++)
			{
				CString sItem = m_lcPrinters.GetItemText(nItem, i);
				if (i == 0)
				{
					printer_names.push_back(sItem);
				}
			}
		}
	}
	//nItem remains -1 if not selected;

	OutputDebugString(L"\n");
	OutputDebugString(L"SELECTED PRINTERS");
	OutputDebugString(L"\n");

	for (int i = 0; i < printer_names.size(); i++)
	{
		CString e = printer_names[i];
		POSITION p = printer_positions[i];
		int idx = printer_item_indexes[i];

		wchar_t buffer[100];
		int cx;

		cx = swprintf(buffer, 100, L"%40s %4d %4d", (LPCWSTR)e, (int)p, idx);
		OutputDebugString(buffer);
		OutputDebugString(L"\n");
	}

	OutputDebugString(L"\n");

	for (CString element : printer_names)
	{
		OutputDebugString(element);
		OutputDebugString(L"\n");
	}

}


void GetClickedPrinter()
{



}




void ErrorMessage(LPCTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
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

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	//LocalFree(lpMsgBuf);
	//LocalFree(lpDisplayBuf);
	//ExitProcess(dw);
}


int SetPrinterStatus(CString printer_name, int status)
{
	// Initialize for output
	wchar_t buffer[100];
	int cx;

	// Initialize for printer access.
	HANDLE pHandle;
	PRINTER_DEFAULTS* pAll = new PRINTER_DEFAULTS();
	pAll->DesiredAccess = PRINTER_ALL_ACCESS;
	BOOL result = 0;

	// OpenPrinter in all-access mode
	result = OpenPrinter((LPWSTR)(LPCWSTR)printer_name, &pHandle, pAll);

	DWORD dwNeeded = 0L;
	DWORD dwReturned;
	PPRINTER_INFO_2 pInfo = NULL;

	result = GetPrinter(pHandle, 2, NULL, 0, &dwNeeded);
	if (!dwNeeded)
	{
		fprintf(stderr, "[FATAL] GetPrinter() failed: %lu\n", GetLastError());
		ClosePrinter(pHandle);
		return EXIT_FAILURE;
	}

	pInfo = (PRINTER_INFO_2*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwNeeded);
	if (pInfo == NULL)
	{
		fprintf(stderr, "[FATAL] HeapAlloc() failed: %lu\n", GetLastError());
		ClosePrinter(pHandle);
		return EXIT_FAILURE;
	}

	result = GetPrinter(pHandle, 2, (LPBYTE)pInfo, dwNeeded, &dwNeeded);
	cx = swprintf(buffer, 100, L"%40s Status: %4d\n", printer_name, pInfo->Status);
	OutputDebugString(buffer);

	result = SetPrinter(pHandle, 0, NULL, status);
	if (!result)
	{
		ErrorMessage(TEXT("SetPrinter"));
	}

	result = GetPrinter(pHandle, 2, (LPBYTE)pInfo, dwNeeded, &dwNeeded);
	cx = swprintf(buffer, 100, L"%40s Status: %4d\n", printer_name, pInfo->Status);
	OutputDebugString(buffer);
	return 0;
}



int ReadPrinter(CString printer_name)
{
	// Initialize for printer access.
	HANDLE pHandle;
	PRINTER_DEFAULTS* pAll = new PRINTER_DEFAULTS();
	pAll->DesiredAccess = PRINTER_ALL_ACCESS;
	BOOL result = 0;

	// OpenPrinter in read-only mode.
	result = OpenPrinter((LPWSTR)(LPCWSTR)printer_name, &pHandle, NULL);

	DWORD dwNeeded = 0L;
	DWORD dwReturned;
	PPRINTER_INFO_2 pInfo = NULL;

	result = GetPrinter(pHandle, 2, NULL, 0, &dwNeeded);
	if (!dwNeeded)
	{
		fprintf(stderr, "[FATAL] GetPrinter() failed: %lu\n", GetLastError());
		ClosePrinter(pHandle);
		return EXIT_FAILURE;
	}

	pInfo = (PRINTER_INFO_2*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwNeeded);
	if (pInfo == NULL)
	{
		fprintf(stderr, "[FATAL] HeapAlloc() failed: %lu\n", GetLastError());
		ClosePrinter(pHandle);
		return EXIT_FAILURE;
	}

	result = GetPrinter(pHandle, 2, (LPBYTE)pInfo, dwNeeded, &dwNeeded);

	CString s;
	wchar_t buffer[100];
	int cx;
	cx = swprintf(buffer, 100, L"Printer Name: %40s", (LPCWSTR)pInfo->pPrinterName);
	s = (CString)buffer;
	printer_properties.push_back(s);
	cx = swprintf(buffer, 100, L"Printer Name: %40s", (LPCWSTR)pInfo->pDriverName);
	s = (CString)buffer;
	printer_properties.push_back(s);
	cx = swprintf(buffer, 100, L"Printer Name: %40s", (LPCWSTR)pInfo->pPortName);
	s = (CString)buffer;
	printer_properties.push_back(s);
	cx = swprintf(buffer, 100, L"Printer Name: %40s", (LPCWSTR)pInfo->pPrintProcessor);
	s = (CString)buffer;
	printer_properties.push_back(s);
	cx = swprintf(buffer, 100, L"Printer Name: %40s", (LPCWSTR)pInfo->pServerName);
	s = (CString)buffer;
	printer_properties.push_back(s);
	cx = swprintf(buffer, 100, L"Printer Name: %40s", (LPCWSTR)pInfo->pShareName);
	s = (CString)buffer;
	printer_properties.push_back(s);
	cx = swprintf(buffer, 100, L"Printer Name: %40d", pInfo->AveragePPM);
	s = (CString)buffer;
	printer_properties.push_back(s);
	
	return 0;
}








int PausePrinter(CString printer_name)
{
	BOOL result = 0;
	result = SetPrinterStatus(printer_name, PRINTER_CONTROL_PAUSE);
	return result;
}


int ResumePrinter(CString printer_name)
{
	BOOL result = 0;
	result = SetPrinterStatus(printer_name, PRINTER_CONTROL_RESUME);
	return result;
}


int PurgePrinter(CString printer_name)
{
	BOOL result = 0;
	result = SetPrinterStatus(printer_name, PRINTER_CONTROL_PURGE);
	return result;
}


void CTAB1::OnBnClickedPausePrinters()
{
	CTAB1::GetSelectedPrinters();
	for (CString printer_name : printer_names)
	{
		BOOL result = 0;
		OutputDebugString(L"\n");
		OutputDebugString(L"PRINTER STATUS");
		OutputDebugString(L"\n");
		result = PausePrinter(printer_name);
		return;
	}
}


void CTAB1::OnBnClickedResumePrinters()
{
	CTAB1::GetSelectedPrinters();
	for (CString printer_name : printer_names)
	{
		BOOL result = 0;
		OutputDebugString(L"\n");
		OutputDebugString(L"PRINTER STATUS");
		OutputDebugString(L"\n");
		result = ResumePrinter(printer_name);
		return;
	}
}


void CTAB1::OnBnClickedPurgePrinters()
{
	CTAB1::GetSelectedPrinters();
	for (CString printer_name : printer_names)
	{
		BOOL result = 0;
		OutputDebugString(L"\n");
		OutputDebugString(L"PRINTER STATUS");
		OutputDebugString(L"\n");
		result = PurgePrinter(printer_name);
		return;
	}
}


void CTAB1::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: Add your message handler code here
	// Load the desired menu
	CMenu mnuPopupSubmit;
	mnuPopupSubmit.LoadMenu(IDR_POPUP_PRINTERS);

	// Get a pointer to the button
	// CButton* pButton;
	// pButton = reinterpret_cast<CButton*>(GetDlgItem(IDC_LC_JOBINFO2));
	CListCtrl* pListCtrl;
	pListCtrl = reinterpret_cast<CListCtrl*>(GetDlgItem(IDC_LC_JOBINFO2));

	// Find the rectangle around the button
	CRect rectSubmitButton;
	pListCtrl->GetWindowRect(&rectSubmitButton);

	// Get a pointer to the first item of the menu
	CMenu* mnuPopupMenu = mnuPopupSubmit.GetSubMenu(0);
	ASSERT(mnuPopupMenu);

	// Find out if the user right-clicked the button
	// because we are interested only in the button
	if (rectSubmitButton.PtInRect(point)) // Since the user right-clicked the button, display the context menu
		mnuPopupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}


void CTAB1::OnPrintersDebug()
{
	// TODO: Add your command handler code here
	MessageBox(L"You selected the DEBUG menu item");
}


void CTAB1::OnPrintersSave()
{
	if (selected_printer_index < 0)
	{
		return;
	}

	CString printer_name = m_lcPrinters.GetItemText(selected_printer_index, 0);


	CFileDialog FileDlg(FALSE, CString(".txt"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, CString(L"*.*"));

	ReadPrinter(printer_name);


	if (FileDlg.DoModal() == IDOK)
	{
		CString filename = FileDlg.GetPathName();
		FILE *log_file = _wfopen(filename, L"a+");
		fwprintf(log_file, L"lpString: %s\n", L"aaaa");
		fclose(log_file);
		int a = 1;
	}
}


void CTAB1::OnBnClickedCancelRedirect2()
{
	// Return if redirected printer has not been set.
	if (redirected_printer_name.GetLength() <= 0)
	{
		return;
	}

	int result = 0;

	// Set printer to resume (un-pause)
	result = ResumePrinter(redirected_printer_name);
	if (result <= 0)
	{
		return;
	}

	// Clear application data.


	// Reset redirected printer to zero-length string
	redirected_printer_name = "";

	// Enable Redirect button
	m_btnRedirect.EnableWindow(TRUE);

	// Disable Cancel button
	m_btnCancelRedirect.EnableWindow(FALSE);

	is_redirected = FALSE;


	// Stop thread
	StopWorkerThread();

	delete m_pEventStopRequested;
	delete m_pEventThreadDone;

	m_mapJobInfo.Cleanup();

	CDialogEx::OnCancel();


}
