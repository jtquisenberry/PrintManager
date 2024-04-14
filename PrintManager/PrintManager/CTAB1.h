#pragma once
#include "afxdialogex.h"


// CTAB1 dialog

class CTAB1 : public CDialogEx
{
	DECLARE_DYNAMIC(CTAB1)

public:
	//CEdit m_ctab1_cedit;
	CComboBox	m_cbPrinters;
	CListCtrl  m_lcPrinters;
	void GetSelectedPrinters();
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
	afx_msg void OnNMRClickLcJobinfo2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedCancelRedirect();
	afx_msg void OnBnClickedRedirect();
	afx_msg void OnBnClickedPausePrinters();
	afx_msg void OnBnClickedResumePrinters();
	afx_msg void OnBnClickedPurgePrinters();
};
