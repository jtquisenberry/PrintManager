#pragma once
#include "afxdialogex.h"


// CTAB1 dialog

class CTAB1 : public CDialogEx
{
	DECLARE_DYNAMIC(CTAB1)

public:
	//CEdit m_ctab1_cedit;
	CComboBox	m_cbPrinters;
	CListCtrl  m_lcJobinfo2;
	CTAB1(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CTAB1();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
