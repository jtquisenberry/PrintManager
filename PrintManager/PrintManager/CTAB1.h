#pragma once
#include "afxdialogex.h"
#include "MapEx.h"
#include "JobInfo.h"
#include "ThreadInfo.h"
#include "PrintSubscriber.h"

#include <thread>

// CTAB1 dialog

class CTAB1 : public CDialogEx
{
	DECLARE_DYNAMIC(CTAB1)

public:
	//CEdit m_ctab1_cedit;
	CComboBox	m_cbPrinters;
	CListCtrl  m_lcPrinters;

	// Create button variables
	CButton m_btnRedirect;
	CButton m_btnCancelRedirect;
	CButton m_btnPausePrinters;
	CButton m_btnResumePrinters;
	CButton m_btnPurgePrinters;

	HANDLE m_hPrinter;
	HANDLE m_hEventStopRequested;
	HANDLE m_hEventThreadDone;
	HWND m_hWnd;

	PrintSubscriber pPs;

	// Function declarations
	void GetSelectedPrinters();
	UINT ThreadFunc2(void);
	void StartWorkerThread();
	void StopWorkerThread();	

	CTAB1(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CTAB1();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnNMRClickLcJobinfo2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedCancelRedirect();
	afx_msg void OnBnClickedRedirect();
	afx_msg void OnBnClickedPausePrinters();
	afx_msg void OnBnClickedResumePrinters();
	afx_msg void OnBnClickedPurgePrinters();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnPrintersDebug();
	afx_msg void OnPrintersSave();
	afx_msg void OnBnClickedCancelRedirect2();

private:
	CEvent* m_pEventThreadDone;
	CEvent* m_pEventStopRequested;
	CWinThread* m_pWinThread;
	// CThreadInfo m_ThreadInfo;

	CMapEx<int, int, CJobInfo*, CJobInfo*> m_mapJobInfo;
};
