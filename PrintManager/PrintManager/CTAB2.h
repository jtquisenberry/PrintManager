#pragma once
#include "afxdialogex.h"


// CTAB2 dialog

class CTAB2 : public CDialogEx
{
	DECLARE_DYNAMIC(CTAB2)

public:
	CTAB2(CWnd* pParent = nullptr);   // standard constructor
	CListCtrl  m_lcDrivers;
	virtual ~CTAB2();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnItemchangedLcJobinfo2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButton1();
};
