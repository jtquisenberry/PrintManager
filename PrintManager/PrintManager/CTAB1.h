#pragma once
#include "afxdialogex.h"
#include "MapEx.h"
#include "JobInfo.h"
#include "ThreadInfo.h"
#include "PrintSubscriber.h"
#include "PrintConverter.h"

#include <thread>

// CTAB1 dialog

class CTAB1 : public CDialogEx
{
	DECLARE_DYNAMIC(CTAB1)

public:
	CComboBox m_cbPrinters;
	CListCtrl m_lcPrinters;

	// Create button variables
	CButton m_btnRedirect;
	CButton m_btnCancelRedirect;
	CButton m_btnPausePrinters;
	CButton m_btnResumePrinters;
	CButton m_btnPurgePrinters;
	CEdit m_editTempPath;

	HANDLE m_hPrinter;
	HANDLE m_hEventSubscriberStopRequested;
	HANDLE m_hEventSubscriberThreadDone;
	HANDLE m_hEventConverterStopRequested;
	HANDLE m_hEventConverterThreadDone;
	HWND m_hWnd;

	PrintSubscriber *m_ppsPrintSubscriber;
	PrintConverter *m_ppcPrintConverter;

	// Function declarations
	void GetSelectedPrinters();
	void StopWorkerThread();
	//void OutputThreadId(CString);
	int UnsetRedirectedPrinter();
	int ReadPrinter(CString);

	std::vector<int> m_PrintStack;
	// std::vector<int>* m_PrintStack2;
	std::vector<CString> m_vectPrinterNames;
	std::vector<POSITION> m_vectPrinterPositions;
	std::vector<int> m_vectPrinterItemIndices;
	std::vector<CString> m_vectPrinterProperties;
	CString m_strPrinterName;
	CString m_strTempDirectory;
	BOOL m_boolIsRedirected;
	int m_nSelectedPrinterIndex;

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
	// afx_msg void OnBnClickedCancelRedirect();
	afx_msg void OnBnClickedRedirect();
	afx_msg void OnBnClickedPausePrinters();
	afx_msg void OnBnClickedResumePrinters();
	afx_msg void OnBnClickedPurgePrinters();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnPrintersDebug();
	afx_msg void OnPrintersSave();
	afx_msg void OnBnClickedCancelRedirect2();
	afx_msg void OnStop();





private:
	CEvent* m_pEventSubscriberThreadDone;
	CEvent* m_pEventSubscriberStopRequested;
	CEvent* m_pEventConverterThreadDone;
	CEvent* m_pEventConverterStopRequested;
	CWinThread* m_pWinThreadSubscriber;
	CWinThread* m_pWinThreadConverter;
	// CThreadInfo m_ThreadInfo;

	CMapEx<int, int, CJobInfo*, CJobInfo*> m_mapJobInfo;
};
