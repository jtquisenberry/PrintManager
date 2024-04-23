// PrintManagerDlg.h : header file
//

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



//#include "stdafx.h"
#include "PrintManager.h"
#include "PrintManagerDlg.h"
#include "CTAB1.h"

#include "AboutDlg.h"





#include "ThreadInfo.h"
#include "MapEx.h"
#include "JobInfo.h"


/////////////////////////////////////////////////////////////////////////////
// PrintMessenger dialog

class PrintMessenger
{
	// Construction
public:
	PrintMessenger(CWnd* pParent = NULL);	// standard constructor

	UINT ThreadFunc(void);

	// Dialog Data
		//{{AFX_DATA(PrintMessenger)
	
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PrintMessenger)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(PrintMessenger)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnStart();
	afx_msg void OnStop();
	afx_msg void OnSelchangePrinters();
	virtual void OnCancel();
	//}}AFX_MSG
	afx_msg LRESULT OnUpdateJobList(WPARAM, LPARAM);


private:
	CRect m_rectOrig;
	int m_nWidth;
	int m_nHeight;

	CEvent* m_pEventThreadDone;
	CEvent* m_pEventStopRequested;
	CWinThread* m_pWinThread;
	CThreadInfo m_ThreadInfo;

	CMapEx<int, int, CJobInfo*, CJobInfo*> m_mapJobInfo;

	enum AnchorFlags
	{
		anchorLEFT = 0x0001,
		anchorTOP = 0x0001,
		anchorRIGHT = 0x0002,
		anchorBOTTOM = 0x0004,
		resizeHORZ = 0x0010,
		resizeVERT = 0x0020,
		resizeBOTH = (resizeHORZ | resizeVERT)
	};

	void EnumeratePrinters(void);
	void EnumerateDrivers(void);
	void RepositionChildControl(CWnd* pWnd, const int dx, const int dy, const UINT uAnchor);
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnTcnSelchangeIdPreviewPrev(NMHDR* pNMHDR, LRESULT* pResult);
	CTabCtrl m_tabcontrol;
	CRect rcItem;
	CRect rcItem2;
	afx_msg void OnSelchangeTabcontrol(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTcnSelchangeTabcontrol(NMHDR* pNMHDR, LRESULT* pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.



