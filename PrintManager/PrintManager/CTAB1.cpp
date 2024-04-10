// CTAB1.cpp : implementation file
//

#include "stdafx.h"
#include "PrintManager.h"
#include "afxdialogex.h"
#include "CTAB1.h"


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
	// TODO: Add your control notification handler code here
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
