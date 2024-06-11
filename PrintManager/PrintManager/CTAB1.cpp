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
#include "ThreadUtils.h"


// CTAB1 dialog
IMPLEMENT_DYNAMIC(CTAB1, CDialogEx)


// Non-class function definitions
//void OutputThreadId(CString);


// Constructor
CTAB1::CTAB1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TAB1, pParent)
{
	/* Initializations */
	// Thread control events
	CTAB1::m_hEventSubscriberStopRequested = INVALID_HANDLE_VALUE;
	CTAB1::m_hEventSubscriberThreadDone = INVALID_HANDLE_VALUE;
	CTAB1::m_pEventSubscriberStopRequested = NULL;
	CTAB1::m_pEventSubscriberThreadDone = NULL;
	CTAB1::m_hEventConverterStopRequested = INVALID_HANDLE_VALUE;
	CTAB1::m_hEventConverterThreadDone = INVALID_HANDLE_VALUE;
	CTAB1::m_pEventConverterStopRequested = NULL;
	CTAB1::m_pEventConverterThreadDone = NULL;	


	CTAB1::m_hPrinter = INVALID_HANDLE_VALUE;
	CTAB1::m_hWnd = 0x0;	
	// CTAB1::m_PrintStack2 = NULL;	
	CTAB1::m_pWinThreadSubscriber = NULL;
	CTAB1::m_pWinThreadConverter = NULL;
	CTAB1::m_strPrinterName = "";
	CTAB1::m_boolIsRedirected = FALSE;
	CTAB1::m_nSelectedPrinterIndex = -1;
	CTAB1::m_vectPrinterItemIndices.clear();
	CTAB1::m_vectPrinterNames.clear();
	CTAB1::m_vectPrinterPositions.clear();
	CTAB1::m_vectPrinterProperties.clear();

	// Initialize custom objects
	CTAB1::m_ppsPrintSubscriber = new PrintSubscriber();
	CTAB1::m_ppcPrintConverter = new PrintConverter();

	// Output thread ID
	ThreadUtils::OutputThreadId(L"CTAB1::CTAB1", g_fileSystem);
}


// Destructor
CTAB1::~CTAB1()
{
	// Output thread ID
	ThreadUtils::OutputThreadId(L"CTAB1::~CTAB1", g_fileSystem);

	// Delete pointers initialized with new
	delete CTAB1::m_ppsPrintSubscriber;
	delete CTAB1::m_ppcPrintConverter;
	delete m_pEventSubscriberStopRequested;
	delete m_pEventSubscriberThreadDone;
	// delete m_PrintStack2;
}


// Map UI elements to variables.
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


// Message map
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

	ThreadUtils::OutputThreadId(L"CTAB1::OnInitDialog", g_fileSystem);

	m_pEventSubscriberThreadDone = new CEvent(TRUE, TRUE);     // Signaled
	m_pEventSubscriberStopRequested = new CEvent(FALSE, TRUE); // Non-signaled
	m_pEventConverterThreadDone = new CEvent(TRUE, TRUE);      // Signaled
	m_pEventConverterStopRequested = new CEvent(FALSE, TRUE);  // Non-signaled
	
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
	m_nSelectedPrinterIndex = right_clicked_item_index;
	int nColumns = m_lcPrinters.GetHeaderCtrl()->GetItemCount();

	wchar_t buffer[5000];
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

	if (buffer_start_index < 500 && buffer_start_index >= 0)
	{
		buffer[buffer_start_index] = L'\0';
	}
	
	OutputDebugString(buffer);
	OutputDebugString(L"\n");

	*pResult = 0;
}


UINT StartPrintSubscriberThread(LPVOID pParam)
{
	ThreadUtils::OutputThreadId(L"global in CTAB1, StartPrintSubscriberThread", g_fileSystem);

	CTAB1* ctab1 = (CTAB1*)pParam;
	ctab1->m_ppsPrintSubscriber->Start(0);
	return 0;
}


UINT StartPrintConverterThread(LPVOID pParam)
{
	ThreadUtils::OutputThreadId(L"global in CTAB1, StartPrintConverterThread", g_fileSystem);

	CTAB1* ctab1 = (CTAB1*)pParam;
	ctab1->m_ppcPrintConverter->Start(0);
	return 0;
}


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
	d;
	m_cbPrinters.GetLBText(redirected_printer_index, m_strPrinterName);

	// Destination printers are `printer_names`
	GetSelectedPrinters();

	// Pause redirected printer
	// PausePrinter(redirected_printer_name);

	m_boolIsRedirected = TRUE;

	// Remove any JobInfo objects that may be in the map.
	m_mapJobInfo.Cleanup();

	// Set the events to non-signaled
	m_pEventSubscriberStopRequested->ResetEvent();
	m_pEventSubscriberThreadDone->ResetEvent();
	m_pEventConverterStopRequested->ResetEvent();
	m_pEventConverterThreadDone->ResetEvent();

	// Open redirected printer
	HANDLE hPrinter;
	OpenPrinter((LPTSTR)(LPCTSTR)m_strPrinterName, &hPrinter, NULL);

	// Setup Print Subscriber object and thread
	m_ppsPrintSubscriber->SetPrinter(hPrinter);
	m_ppsPrintSubscriber->SetStopRequestedEvent(m_pEventSubscriberStopRequested->m_hObject);
	m_ppsPrintSubscriber->SetThreadDoneEvent(m_pEventSubscriberThreadDone->m_hObject);
	m_ppsPrintSubscriber->SetHwnd(GetSafeHwnd());
	m_ppsPrintSubscriber->m_PrintStack = &m_PrintStack;
	m_ppsPrintSubscriber->m_boolNotifyWindow = FALSE;
	m_ppsPrintSubscriber->m_boolOutputJobInfo = TRUE;
	m_ppsPrintSubscriber->m_boolSetForConversion = TRUE;

	// Setup Print Converter object and thread
	m_ppcPrintConverter->m_PrintStack = &m_PrintStack;
	m_ppcPrintConverter->SetStopRequestedEvent(m_pEventConverterStopRequested->m_hObject);
	m_ppcPrintConverter->SetThreadDoneEvent(m_pEventConverterThreadDone->m_hObject);

	// Setup Print Stack to pass information between threads
	// m_PrintStack2 = new std::vector<int>(100);

	// Start threads
	m_pWinThreadSubscriber = AfxBeginThread(::StartPrintSubscriberThread, this);
	m_pWinThreadConverter = AfxBeginThread(::StartPrintConverterThread, this);
	
	return;
}


void CTAB1::GetSelectedPrinters()
{
	m_vectPrinterNames.clear();
	m_vectPrinterPositions.clear();
	m_vectPrinterItemIndices.clear();

	int nSelectedRows = m_lcPrinters.GetSelectedCount();
	nSelectedRows;
	int nColumns = m_lcPrinters.GetHeaderCtrl()->GetItemCount();

	// pos is a 1-indexed hex value indicating the position in the list from the top.
	POSITION pos = m_lcPrinters.GetFirstSelectedItemPosition();
	m_vectPrinterPositions.push_back(pos);
	int nItem = -1;
	if (pos != NULL)
	{
		while (pos)
		{
			// nItem is a 0-indexed value indicating the position in the list from the top.
			nItem = m_lcPrinters.GetNextSelectedItem(pos);
			m_vectPrinterPositions.push_back(pos);
			m_vectPrinterItemIndices.push_back(nItem);

			for (int i = 0; i < nColumns; i++)
			{
				CString sItem = m_lcPrinters.GetItemText(nItem, i);
				if (i == 0)
				{
					m_vectPrinterNames.push_back(sItem);
				}
			}
		}
	}
	//nItem remains -1 if not selected;

	OutputDebugString(L"\n");
	OutputDebugString(L"SELECTED PRINTERS");
	OutputDebugString(L"\n");

	for (int i = 0; i < m_vectPrinterNames.size(); i++)
	{
		CString e = m_vectPrinterNames[i];
		POSITION p = m_vectPrinterPositions[i];
		int idx = m_vectPrinterItemIndices[i];
		wchar_t buffer[100];
		int cx;

		cx = swprintf(buffer, 100, L"%- 40s %4lu %4d", (LPCWSTR)e, *(unsigned long*)&p, idx);
		OutputDebugString(buffer);
		OutputDebugString(L"\n");
	}

	OutputDebugString(L"\n");

	for (CString element : m_vectPrinterNames)
	{
		OutputDebugString(element);
		OutputDebugString(L"\n");
	}

	return;
}


void GetClickedPrinter()
{



}


int SetPrinterStatus(CString printer_name, int Command)
{
	// Initialize for output
	wchar_t buffer[100];
	int cx;

	// Initialize for printer access.
	HANDLE hPrinter;
	PRINTER_DEFAULTS* pAll = new PRINTER_DEFAULTS();
	pAll->DesiredAccess = PRINTER_ALL_ACCESS;
	BOOL result = 0;

	// OpenPrinter in all-access mode
	result = OpenPrinter((LPWSTR)(LPCWSTR)printer_name, &hPrinter, pAll);

	DWORD dwNeeded = 0L;
	DWORD dwReturned = 0L;
	dwReturned;
	PPRINTER_INFO_2 pInfo = NULL;

	result = GetPrinter(hPrinter, 2, NULL, 0, &dwNeeded);
	if (!dwNeeded)
	{
		fprintf(stderr, "[FATAL] GetPrinter() failed: %lu\n", GetLastError());
		ClosePrinter(hPrinter);
		return EXIT_FAILURE;
	}

	pInfo = (PRINTER_INFO_2*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwNeeded);
	if (pInfo == NULL)
	{
		fprintf(stderr, "[FATAL] HeapAlloc() failed: %lu\n", GetLastError());
		ClosePrinter(hPrinter);
		return EXIT_FAILURE;
	}

	result = GetPrinter(hPrinter, 2, (LPBYTE)pInfo, dwNeeded, &dwNeeded);
	cx = swprintf(buffer, 100, L"%40s Status: %4d\n", (LPCWSTR)printer_name, pInfo->Status);
	OutputDebugString(buffer);

	// BOOL SetPrinter(
	//  _In_ HANDLE hPrinter,
	//	_In_ DWORD  Level,
	//	_In_ LPBYTE pPrinter,
	//	_In_ DWORD  Command
	//	);
	// 
	// hPrinter: A handle to the printer.
	// 
	// Level: If the Command parameter is not equal to zero, the Level parameter must be zero.
	// 
	// pPrinter: If Level is 0, but the Command parameter is not PRINTER_CONTROL_SET_STATUS, 
	// pPrinter must be NULL.
	// 
	// Command possibilities
	// PRINTER_CONTROL_PAUSE;        // 1
	// PRINTER_CONTROL_PURGE;        // 3
	// PRINTER_CONTROL_RESUME;       // 2
	// PRINTER_CONTROL_SET_STATUS;   // 4
	// https://learn.microsoft.com/en-us/windows/win32/printdocs/setprinter
	result = SetPrinter(hPrinter, 0, NULL, Command);
	if (!result)
	{
		ThreadUtils::ErrorMessage(TEXT("SetPrinter"));
	}

	result = GetPrinter(hPrinter, 2, (LPBYTE)pInfo, dwNeeded, &dwNeeded);
	cx = swprintf(buffer, 100, L"%40s Status: %4d\n", (LPCWSTR)printer_name, pInfo->Status);
	OutputDebugString(buffer);

	delete pAll;

	return 0;
}


int CTAB1::ReadPrinter(CString printer_name)
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
	dwReturned;
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
	m_vectPrinterProperties.push_back(s);
	cx = swprintf(buffer, 100, L"Printer Name: %40s", (LPCWSTR)pInfo->pDriverName);
	s = (CString)buffer;
	m_vectPrinterProperties.push_back(s);
	cx = swprintf(buffer, 100, L"Printer Name: %40s", (LPCWSTR)pInfo->pPortName);
	s = (CString)buffer;
	m_vectPrinterProperties.push_back(s);
	cx = swprintf(buffer, 100, L"Printer Name: %40s", (LPCWSTR)pInfo->pPrintProcessor);
	s = (CString)buffer;
	m_vectPrinterProperties.push_back(s);
	cx = swprintf(buffer, 100, L"Printer Name: %40s", (LPCWSTR)pInfo->pServerName);
	s = (CString)buffer;
	m_vectPrinterProperties.push_back(s);
	cx = swprintf(buffer, 100, L"Printer Name: %40s", (LPCWSTR)pInfo->pShareName);
	s = (CString)buffer;
	m_vectPrinterProperties.push_back(s);
	cx = swprintf(buffer, 100, L"Printer Name: %40d", pInfo->AveragePPM);
	s = (CString)buffer;
	m_vectPrinterProperties.push_back(s);
	
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
	for (CString printer_name : m_vectPrinterNames)
	{
		BOOL result = 0;
		OutputDebugString(L"\n");
		OutputDebugString(L"PRINTER STATUS");
		OutputDebugString(L"\n");
		result = PausePrinter(printer_name);
	}

	return;
}


void CTAB1::OnBnClickedResumePrinters()
{
	CTAB1::GetSelectedPrinters();
	for (CString printer_name : m_vectPrinterNames)
	{
		BOOL result = 0;
		OutputDebugString(L"\n");
		OutputDebugString(L"PRINTER STATUS");
		OutputDebugString(L"\n");
		result = ResumePrinter(printer_name);
	}

	return;
}


void CTAB1::OnBnClickedPurgePrinters()
{
	CTAB1::GetSelectedPrinters();
	for (CString printer_name : m_vectPrinterNames)
	{
		BOOL result = 0;
		OutputDebugString(L"\n");
		OutputDebugString(L"PRINTER STATUS");
		OutputDebugString(L"\n");
		result = PurgePrinter(printer_name);
	}

	return;
}


void CTAB1::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// C4100 unreferenced formal parameter
	pWnd;

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
	if (m_nSelectedPrinterIndex < 0)
	{
		return;
	}

	CString printer_name = m_lcPrinters.GetItemText(m_nSelectedPrinterIndex, 0);
	CFileDialog FileDlg(FALSE, CString(".txt"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, CString(L"*.*"));
	ReadPrinter(printer_name);

	if (FileDlg.DoModal() == IDOK)
	{
		CString filename = FileDlg.GetPathName();
		FILE* log_file;
        _wfopen_s(&log_file, filename, L"a+");
		if (log_file)
		{
			fwprintf(log_file, L"lpString: %s\n", L"aaaa");
			fflush(log_file);
			fclose(log_file);
		}
	}
}


void CTAB1::OnStop()
{
	// Signal and wait for Subscriber to end 
	m_pEventSubscriberStopRequested->SetEvent();
	WaitForSingleObject(m_pEventSubscriberThreadDone->m_hObject, 8000U);

	if (m_ppsPrintSubscriber->GetPrinter() != INVALID_HANDLE_VALUE)
		ClosePrinter(m_ppsPrintSubscriber->GetPrinter());

	// Signal and wait for Converter to end 
	m_pEventConverterStopRequested->SetEvent();
	WaitForSingleObject(m_pEventConverterThreadDone->m_hObject, 8000U);

	// Enable Redirect button
	m_btnRedirect.EnableWindow(TRUE);

	// Disable Cancel Redirect button
	m_btnCancelRedirect.EnableWindow(FALSE);
}


int CTAB1::UnsetRedirectedPrinter()
{

	// Unset redirected printer.
	int result = 0;
	if (m_strPrinterName.GetLength() > 0)
	{
		result = ResumePrinter(m_strPrinterName);
	}

	// Reset redirected printer to zero-length string
	m_strPrinterName = "";
	m_boolIsRedirected = FALSE;

	return result;
}


void CTAB1::StopWorkerThread()
{
	// signal and wait for ThreadFunc() to end 
	m_pEventSubscriberStopRequested->SetEvent();
	WaitForSingleObject(m_pEventSubscriberThreadDone->m_hObject, 8000U);

	// if (m_ThreadInfo.GetPrinter() != INVALID_HANDLE_VALUE)
	//	ClosePrinter(m_ThreadInfo.GetPrinter());
}


void CTAB1::OnBnClickedCancelRedirect2()
{
	OnStop();
	UnsetRedirectedPrinter();

	// Stop thread
	// TODO: Add Stop PrintConverter thread.
	StopWorkerThread();
	
	m_mapJobInfo.Cleanup();

	// This line closes the tab dialog.
	// CDialogEx::OnCancel();
}
