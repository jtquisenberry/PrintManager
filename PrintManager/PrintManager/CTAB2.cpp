// CTAB2.cpp : implementation file
//

#include "stdafx.h"
#include "PrintManager.h"
#include "afxdialogex.h"
#include "CTAB2.h"


// CTAB2 dialog

IMPLEMENT_DYNAMIC(CTAB2, CDialogEx)

CTAB2::CTAB2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TAB2, pParent)
{

}

CTAB2::~CTAB2()
{
}

void CTAB2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LC_DRIVERINFO, m_lcDrivers);
}


BEGIN_MESSAGE_MAP(CTAB2, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LC_JOBINFO2, &CTAB2::OnLvnItemchangedLcJobinfo2)
	ON_BN_CLICKED(IDC_BUTTON1, &CTAB2::OnBnClickedButton1)
END_MESSAGE_MAP()


// CTAB2 message handlers


void CTAB2::OnLvnItemchangedLcJobinfo2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;
}


void CTAB2::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	int a = 1;
	a++;
}
