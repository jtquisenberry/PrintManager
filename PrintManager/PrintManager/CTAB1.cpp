// CTAB1.cpp : implementation file
//


#include "stdafx.h"
#include "PrintManager.h"
#include "afxdialogex.h"
#include "CTAB1.h"
#include <iostream>
#include <vector>
#include <stdio.h>


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
	ON_BN_CLICKED(IDC_CANCEL_REDIRECT, &CTAB1::OnBnClickedCancelRedirect)
	ON_BN_CLICKED(IDC_REDIRECT, &CTAB1::OnBnClickedRedirect)
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
