// CTAB1.cpp : implementation file
//


#include "stdafx.h"
#include "PrintManager.h"
#include "afxdialogex.h"
#include "CTAB1.h"
#include <iostream>
#include <vector>
#include <stdio.h>

#include <windows.h>
#include <strsafe.h>

std::vector<CString> printer_names;
std::vector<POSITION> printer_positions;
std::vector<int> printer_item_indexes;


//void GetSelectedPrinters();



// CTAB1 dialog

IMPLEMENT_DYNAMIC(CTAB1, CDialogEx)

CTAB1::CTAB1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TAB1, pParent)
{

}

CTAB1::~CTAB1()
{
}

void CTAB1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CB_PRINTERS2, m_cbPrinters);
	DDX_Control(pDX, IDC_LC_JOBINFO2, m_lcPrinters);
}


BEGIN_MESSAGE_MAP(CTAB1, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTAB1::OnBnClickedOk)
	ON_NOTIFY(NM_RCLICK, IDC_LC_JOBINFO2, &CTAB1::OnNMRClickLcJobinfo2)
	//ON_BN_CLICKED(IDC_CANCEL_REDIRECT, &CTAB1::OnBnClickedCancelRedirect)
	ON_BN_CLICKED(IDC_REDIRECT, &CTAB1::OnBnClickedRedirect)
	ON_BN_CLICKED(IDC_PAUSE_PRINTERS, &CTAB1::OnBnClickedPausePrinters)
END_MESSAGE_MAP()


// CTAB1 message handlers


void CTAB1::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void CTAB1::OnNMRClickLcJobinfo2(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Right-click m_lcPrinters	
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
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

	for (int i=0; i < printer_names.size(); i++)
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

	*pResult = 0;
}


void CTAB1::OnBnClickedCancelRedirect()
{
	// TODO: Add your control notification handler code here
}


void CTAB1::OnBnClickedRedirect()
{
	int redirected_printer_index = m_cbPrinters.GetCurSel();

	if (redirected_printer_index < 0)
	{
		return;
	}

	DWORD_PTR d = m_cbPrinters.GetItemData(redirected_printer_index);
	CString redirected_printer_name;
	m_cbPrinters.GetLBText(redirected_printer_index, redirected_printer_name);

	GetSelectedPrinters();
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



void ErrorExit(LPCTSTR lpszFunction)
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

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(dw);
}




void CTAB1::OnBnClickedPausePrinters()
{
	CTAB1::GetSelectedPrinters();
	for (CString printer_name : printer_names)
	{
		HANDLE pHandle;
		int status = -999;
		status = OpenPrinter((WCHAR*)(LPCWSTR)printer_name, &pHandle, NULL);


		DWORD dwNeeded = 0L;
		DWORD dwReturned;
		PPRINTER_INFO_2 pInfo = NULL;

		BOOL ret = GetPrinter(pHandle, 2, NULL, 0, &dwNeeded);

		if (!dwNeeded)
		{
			fprintf(stderr, "[FATAL] GetPrinter() failed: %lu\n", GetLastError());
			//ClosePrinter(pHandle);
			// return EXIT_FAILURE;
		}

		pInfo = (PRINTER_INFO_2*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwNeeded);

		if (pInfo == NULL)
		{
			fprintf(stderr, "[FATAL] HeapAlloc() failed: %lu\n", GetLastError());
			//ClosePrinter(pHandle);
			// return EXIT_FAILURE;
		}

		ret = GetPrinter(pHandle, 2, (LPBYTE)pInfo, dwNeeded, &dwNeeded);

		int c = PRINTER_STATUS_PAUSED;

		BOOL yyy = SetPrinter(pHandle, 0, NULL, PRINTER_CONTROL_PAUSE);
		ErrorExit(TEXT("GetProcessId"));
		int a = 1;

		ret = GetPrinter(pHandle, 2, (LPBYTE)pInfo, dwNeeded, &dwNeeded);
		int b = 1;

	}
}
