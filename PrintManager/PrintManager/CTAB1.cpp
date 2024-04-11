// CTAB1.cpp : implementation file
//


#include "stdafx.h"
#include "PrintManager.h"
#include "afxdialogex.h"
#include "CTAB1.h"
#include <iostream>
#include <vector>



std::vector<CString> s;



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
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);



	



	int xxx = m_lcPrinters.GetSelectedCount();

	int nColumns = m_lcPrinters.GetHeaderCtrl()->GetItemCount();
	POSITION pos = m_lcPrinters.GetFirstSelectedItemPosition();
	int selected = -1;
	if (pos != NULL)
	{
		while (pos)
		{
			int nItem = m_lcPrinters.GetNextSelectedItem(pos);
			selected = nItem + 1;

			

			for (int i = 0; i < nColumns; i++)
			{
				CString sItem = m_lcPrinters.GetItemText(nItem, i);
				// TO DO: do stuff with item text here

				//std::wcout << "aaa";

				if (i == 0)
				{
					s.push_back(sItem);
				}


			}

		}
	}
	//returns -1 if not selected;
	
	OutputDebugString(L"\n");
	OutputDebugString(L"SELECTED PRINTERS");
	OutputDebugString(L"\n");



	for (CString element : s)
	{
		OutputDebugString(element);
		OutputDebugString(L"\n");
	}

	OutputDebugString(L"\n");

	*pResult = 0;
}


void CTAB1::OnBnClickedCancelRedirect()
{
	// TODO: Add your control notification handler code here
}


void CTAB1::OnBnClickedRedirect()
{
	int xxx = m_cbPrinters.GetCurSel();
	DWORD_PTR d = m_cbPrinters.GetItemData(xxx);
	CString cstring;
	m_cbPrinters.GetLBText(xxx, cstring);
}
