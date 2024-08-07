#include "stdafx.h"
#include "PrintManager.h"
#include "PrintManagerDlg.h"
#include "CTAB1.h"
#include "AboutDlg.h"
#include "LogFile.h"
#include "ThreadUtils.h"
#include "Windows.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const UINT UDM_UPDATE_JOB_LIST = RegisterWindowMessage(_T("UDM_UPDATE_JOB_LIST"));


/////////////////////////////////////////////////////////////////////////////
// CPrintManagerDlg dialog

CPrintManagerDlg::CPrintManagerDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(CPrintManagerDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPrintManagerDlg)
    //}}AFX_DATA_INIT

    // UI Elements
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_rectOrig.SetRectEmpty();

    // Thread Elements
    m_pEventSubscriberThreadDone    = NULL;
    m_pEventSubscriberStopRequested = NULL;

    // Member variables initialization
    m_nHeight = 0;
    m_nWidth = 0;
    m_pWinThreadSubscriber = NULL;
    m_pmapJobInfo = NULL;
    CPrintManagerDlg::m_ppsPrintSubscriber = new PrintSubscriber();

    // Print thread ID
    ThreadUtils::OutputThreadId(L"CPrintManagerDlg::CPrintManagerDlg", g_fileSystem);

    char* base_path;
    size_t len;
    errno_t err = _dupenv_s(&base_path, &len, "USERPROFILE");
    err;
    wchar_t* wc = new wchar_t[260] {0};
    if (base_path)
    {
        size_t pReturnValue;
        errno_t err2 = mbstowcs_s(
            &pReturnValue,
            &wc[0],
            260,
            base_path,
            260
        );
        err2;
        
        
        
        //mbstowcs(&wc[0], base_path, 260);


        HWND handle = GetSafeHwnd();

        HINSTANCE hError = ShellExecute(handle, NULL, wc, NULL, NULL, SW_SHOWMINIMIZED);
        INT_PTR pError = (INT_PTR)hError;  // If > 32, then success.
        if (pError <= 32)
        {
            // Do something if an error occurs.
            // However, it just means that the folder was not opened. So, the only 
            // actions that make sense are to log it or try again.
        }

    }

    //delete pReturnValue;
    delete[] wc;
    delete base_path;
    // delete m_ppsPrintSubscriber;

}

CPrintManagerDlg::~CPrintManagerDlg()
{
    ThreadUtils::OutputThreadId(L"CPrintManagerDlg::~CPrintManagerDlg", g_fileSystem);

    delete m_pEventSubscriberStopRequested;
    delete m_pEventSubscriberThreadDone;
    delete m_ppsPrintSubscriber;
}


void CPrintManagerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPrintManagerDlg)
    DDX_Control(pDX, IDC_BTN_STOP, m_btnStop);
    DDX_Control(pDX, IDC_BTN_START, m_btnStart);
    DDX_Control(pDX, IDC_CB_PRINTERS, m_cbPrinters);
    DDX_Control(pDX, IDCANCEL, m_btnCancel);
    DDX_Control(pDX, IDC_LC_JOBINFO, m_lcJobInfo);
    DDX_Control(pDX, IDC_TABCONTROL, m_tabcontrol);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPrintManagerDlg, CDialogEx)
    //{{AFX_MSG_MAP(CPrintManagerDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_GETMINMAXINFO()
    ON_WM_SIZE()
    ON_BN_CLICKED(IDC_BTN_START, OnStart)
    ON_BN_CLICKED(IDC_BTN_STOP, OnStop)
    ON_CBN_SELCHANGE(IDC_CB_PRINTERS, OnSelchangePrinters)
    //}}AFX_MSG_MAP
    ON_REGISTERED_MESSAGE(UDM_UPDATE_JOB_LIST, OnUpdateJobList)
    ON_BN_CLICKED(IDCANCEL, &CPrintManagerDlg::OnBnClickedCancel)
    ON_NOTIFY(TCN_SELCHANGE, IDC_TABCONTROL, &CPrintManagerDlg::OnTcnSelchangeTabcontrol)
    ON_NOTIFY(TCN_SELCHANGE, AFX_ID_PREVIEW_PREV, &CPrintManagerDlg::OnTcnSelchangeIdPreviewPrev)
END_MESSAGE_MAP()




/////////////////////////////////////////////////////////////////////////////
// CPrintManagerDlg message handlers

BOOL CPrintManagerDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();


    // Print thread ID
    wchar_t buffer[100];
    int cx;
    std::thread::id this_id = std::this_thread::get_id();
    cx = swprintf(buffer, 100, L"Thread ID: %d \n", *(int*)&this_id);
    OutputDebugString(L"\n");
    OutputDebugString(L"\n");
    OutputDebugString(L"CPrintManagerDlg::OnInitDialog\n");
    OutputDebugString(buffer);
    OutputDebugString(L"\n");
    OutputDebugString(L"\n");
    
    int written2 = 0;
    written2 = fwprintf_s(g_fileSystem, L"%- 70s %s", L"CPrintManagerDlg::OnInitDialog: ", buffer);

    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        CString strAboutMenu;
        int ret_val = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ret_val++;
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }


    // Set the icon for this dialog.  The framework does this automatically
    // when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon
 
    GetClientRect(m_rectOrig);
    m_nWidth  = m_rectOrig.Width();
    m_nHeight = m_rectOrig.Height();

    m_lcJobInfo.InsertColumn(0, _T("Job Id"), LVCFMT_RIGHT, 45);
    m_lcJobInfo.InsertColumn(1, _T("User"), LVCFMT_LEFT, 75);
    m_lcJobInfo.InsertColumn(2, _T("From"), LVCFMT_LEFT, 80);
    m_lcJobInfo.InsertColumn(3, _T("Document"), LVCFMT_LEFT, 125);
    m_lcJobInfo.InsertColumn(4, _T("Submitted @"), LVCFMT_LEFT, 80);
    m_lcJobInfo.InsertColumn(5, _T("Pages printed"), LVCFMT_RIGHT, 80);
    m_lcJobInfo.InsertColumn(6, _T("Bytes printed"), LVCFMT_RIGHT, 110);
    m_lcJobInfo.InsertColumn(7, _T("Status"), LVCFMT_LEFT, 150);

    

    m_pEventSubscriberThreadDone = new CEvent(TRUE, TRUE);     // signaled
    m_pEventSubscriberStopRequested = new CEvent(FALSE, TRUE); // non-signaled

    // Print thread ID
    ThreadUtils::OutputThreadId(L"CPrintManagerDlg::OnInitDialog", g_fileSystem);
    
    m_ppsPrintSubscriber->SetStopRequestedEvent(m_pEventSubscriberStopRequested->m_hObject);
    m_ppsPrintSubscriber->SetThreadDoneEvent(m_pEventSubscriberThreadDone->m_hObject);
    m_ppsPrintSubscriber->SetHwnd(GetSafeHwnd());
    m_ppsPrintSubscriber->SetWindowsMessage(UDM_UPDATE_JOB_LIST);
        
    /*Setup tabcontrol and tabs*/
    CTabCtrl* pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TABCONTROL);
    m_tab1.Create(IDD_TAB1, pTabCtrl);

    CTabCtrl* pTabCtrl2 = (CTabCtrl*)GetDlgItem(IDC_TABCONTROL);
    m_tab2.Create(IDD_TAB2, pTabCtrl2);

    TCITEM item1, item2;
    item1.mask = TCIF_TEXT | TCIF_PARAM;
    item1.lParam = (LPARAM)&m_tab1;
    item1.pszText = _T("Printers");
    pTabCtrl->InsertItem(0, &item1);

    item2.mask = TCIF_TEXT | TCIF_PARAM;
    item2.lParam = (LPARAM)&m_tab2;
    item2.pszText = _T("Drivers");
    pTabCtrl2->InsertItem(1, &item2);

    // CRect rcItem;  // moved to header file.
    pTabCtrl->GetItemRect(0, &rcItem);
    m_tab1.SetWindowPos(NULL, rcItem.left, rcItem.bottom + 1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    // CRect rcItem2;  // moved to header file.
    pTabCtrl2->GetItemRect(0, &rcItem2);
    m_tab2.SetWindowPos(NULL, rcItem2.left, rcItem2.bottom + 1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    m_tab1.ShowWindow(SW_SHOW);
    m_tab2.ShowWindow(SW_HIDE);

    // Tab 1
    m_tab1.m_lcPrinters.SetExtendedStyle(LVS_EX_FULLROWSELECT);
    m_tab1.m_lcPrinters.InsertColumn(0, _T("Printer Name"), LVCFMT_RIGHT, 230);
    m_tab1.m_lcPrinters.InsertColumn(1, _T("User"), LVCFMT_LEFT, 190);
    m_tab1.m_lcPrinters.InsertColumn(2, _T("From"), LVCFMT_LEFT, 190);
    m_tab1.m_lcPrinters.InsertColumn(3, _T("Document"), LVCFMT_LEFT, 180);
    m_tab1.m_btnCancelRedirect.EnableWindow(FALSE);
    
    // Tab 2
    m_tab2.m_lcDrivers.InsertColumn(0, _T("Driver Name"), LVCFMT_RIGHT, 200);
    m_tab2.m_lcDrivers.InsertColumn(1, _T("Path"), LVCFMT_LEFT, 150);
    m_tab2.m_lcDrivers.InsertColumn(2, _T("Data File"), LVCFMT_LEFT, 150);
    m_tab2.m_lcDrivers.InsertColumn(3, _T("Monitor"), LVCFMT_LEFT, 200);

    // Do work on application startup.
    EnumeratePrinters();
    EnumerateDrivers();
    
    return TRUE;  // return TRUE  unless you set the focus to a control
}


void CPrintManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}


void CPrintManagerDlg::EnumeratePrinters( void )
{
    DWORD dwNeeded;
    DWORD dwReturned;
    
    EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, NULL, 2, NULL, 0, &dwNeeded, &dwReturned);
    
    LPBYTE lpBuffer = new BYTE[dwNeeded];
    EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, NULL, 2, lpBuffer, dwNeeded, &dwNeeded, &dwReturned);

    PPRINTER_INFO_2 p1 = (PPRINTER_INFO_2) lpBuffer;


    char* base_path;
    size_t len;
    errno_t err = _dupenv_s(&base_path, &len, "USERPROFILE");
    err;
    wchar_t* wc = new wchar_t[260] {0};
    if (base_path)
    {
        size_t pReturnValue;
        errno_t err2 = mbstowcs_s(
            &pReturnValue,
            &wc[0],
            260,
            base_path,
            260
        );
        err2;
    }
    else
    {
        wc = L"c:";
    }

    m_tab1.m_editTempPath.SetWindowTextW(wc);
    
    for (DWORD x = 0; x < dwReturned; x++)
    {
        m_cbPrinters.AddString(p1->pPrinterName);
        m_tab1.m_cbPrinters.AddString(p1->pPrinterName);

        
        CString strText;
        strText.Format(_T("%s"), p1->pPrinterName);
        int nItem = m_tab1.m_lcPrinters.InsertItem(m_tab1.m_lcPrinters.GetItemCount(), strText);

        m_tab1.m_lcPrinters.SetItemText(nItem, 1, p1->pDriverName);

        strText.Format(_T("%d on %s"), p1->Status, p1->pPortName);
        m_tab1.m_lcPrinters.SetItemText(nItem, 2, strText);

        strText.Format(_T("%d"), p1->cJobs);
        m_tab1.m_lcPrinters.SetItemText(nItem, 3, strText);

        p1++;
    }
    
    delete[] lpBuffer;
}


void CPrintManagerDlg::EnumerateDrivers(void)
{
    DWORD dwNeeded;
    DWORD dwReturned;

    EnumPrinterDrivers(NULL, _T("all"), 4, NULL, 0, &dwNeeded, &dwReturned);
    LPBYTE lpBuffer = new BYTE[dwNeeded];
    EnumPrinterDrivers(NULL, _T("all"), 4, lpBuffer, dwNeeded, &dwNeeded, &dwReturned);
    PDRIVER_INFO_4 di = (PDRIVER_INFO_4)lpBuffer;

    for (DWORD x = 0; x < dwReturned; x++)
    {
        CString strText;
        strText.Format(_T("%s"), di->pName);
        int nItem = m_tab2.m_lcDrivers.InsertItem(m_tab2.m_lcDrivers.GetItemCount(), strText);
        nItem = m_tab2.m_lcDrivers.InsertItem(m_tab2.m_lcDrivers.GetItemCount(), strText);
        nItem = m_tab2.m_lcDrivers.InsertItem(m_tab2.m_lcDrivers.GetItemCount(), strText);
        nItem = m_tab2.m_lcDrivers.InsertItem(m_tab2.m_lcDrivers.GetItemCount(), strText);
        nItem = m_tab2.m_lcDrivers.InsertItem(m_tab2.m_lcDrivers.GetItemCount(), strText);

        m_tab2.m_lcDrivers.SetItemText(nItem, 1, di->pDriverPath);

        strText.Format(_T("%s"), di->pDataFile);
        m_tab2.m_lcDrivers.SetItemText(nItem, 2, strText);

        strText.Format(_T("%s"), di->pMonitorName);
        m_tab2.m_lcDrivers.SetItemText(nItem, 3, strText);
        
        di++;
    }

    delete[] lpBuffer;
}


UINT ThreadFunc( LPVOID pParam )
{
    // Print thread ID
    wchar_t buffer[100];
    int cx = 0;
    std::thread::id this_id = std::this_thread::get_id();
    cx = swprintf(buffer, 100, L"Thread ID: %d \n", *(int*)&this_id);
    OutputDebugString(L"\n\n");
    OutputDebugString(L"CPrintManagerDlg, UINT ThreadFunc(LPVOID pParam)\n");
    OutputDebugString(buffer);
    OutputDebugString(L"\n\n");
    cx = fwprintf_s(g_fileSystem, L"%- 70s %s", L"CPrintManagerDlg, UINT ThreadFunc(LPVOID pParam): ", buffer);
    
    // CPrintManagerDlg *pDlg = (CPrintManagerDlg *) pParam;
    // return pDlg->ThreadFunc();

    CPrintManagerDlg* print_manager = (CPrintManagerDlg*)pParam;
    return print_manager->m_ppsPrintSubscriber->Start(0);
}


void CPrintManagerDlg::OnSelchangePrinters() 
{
    m_btnStart.EnableWindow(TRUE);
}


void CPrintManagerDlg::OnStart()
{
    // Clicked Start button
    
    m_btnStart.EnableWindow(FALSE);
    m_btnStop.EnableWindow(TRUE);
    m_cbPrinters.EnableWindow(FALSE);

    m_mapJobInfo.Cleanup();
    
    // set the events to non-signaled
    m_pEventSubscriberStopRequested->ResetEvent();
    m_pEventSubscriberThreadDone->ResetEvent();


    HANDLE hPrinter;
    CString strPrinter;

    m_cbPrinters.GetWindowText(strPrinter);
    OpenPrinter((LPTSTR) (LPCTSTR) strPrinter, &hPrinter, NULL);
    
    m_ppsPrintSubscriber->SetPrinter(hPrinter);
    m_ppsPrintSubscriber->m_boolNotifyWindow = TRUE;
    m_ppsPrintSubscriber->m_boolOutputJobInfo = FALSE;
    m_ppsPrintSubscriber->m_boolSetForConversion = FALSE;

    m_pmapJobInfo = &(m_ppsPrintSubscriber->m_mapJobInfo);
    

    // Remember that ::Func means the global version of the function.
    m_pWinThreadSubscriber = AfxBeginThread(::ThreadFunc, this);
}


void CPrintManagerDlg::OnStop() 
{
    // signal and wait for ThreadFunc() to end 
    m_pEventSubscriberStopRequested->SetEvent();
    WaitForSingleObject(m_pEventSubscriberThreadDone->m_hObject, 8000U);

    if (m_ppsPrintSubscriber->GetPrinter() != INVALID_HANDLE_VALUE)
        ClosePrinter(m_ppsPrintSubscriber->GetPrinter());

    m_btnStart.EnableWindow(TRUE);
    m_btnStop.EnableWindow(FALSE);
    m_cbPrinters.EnableWindow(TRUE);
}


void CPrintManagerDlg::OnCancel() 
{
    OnStop();

    delete m_pEventSubscriberStopRequested;
    delete m_pEventSubscriberThreadDone;

    m_mapJobInfo.Cleanup();

    CDialogEx::OnCancel();
}


LRESULT CPrintManagerDlg::OnUpdateJobList( WPARAM, LPARAM )
{
    m_lcJobInfo.DeleteAllItems();

    POSITION pos = m_pmapJobInfo->GetStartPosition();
    while (pos != NULL)
    {
        int nKey;
        CJobInfo *pJobInfo;
        m_pmapJobInfo->GetNextAssoc(pos, nKey, pJobInfo);

        // ASSERT(pJobInfo != NULL);

        CString strText;
        if (pJobInfo)
        {
            strText.Format(_T("%03d"), pJobInfo->GetJobId());
        }
        else
        {
            return 999;
        }

        int nItem = m_lcJobInfo.InsertItem(m_lcJobInfo.GetItemCount(), strText);

        m_lcJobInfo.SetItemText(nItem, 1, pJobInfo->GetUserName());
        
        strText.Format(_T("%s on %s"), pJobInfo->GetPortName(), pJobInfo->GetMachineName());
        m_lcJobInfo.SetItemText(nItem, 2, strText);
        
        m_lcJobInfo.SetItemText(nItem, 3, pJobInfo->GetDocument());

        if (pJobInfo->GetSubmitted().wHour != USHRT_MAX)
        {
            SYSTEMTIME st1 = pJobInfo->GetSubmitted(),
                       st2;

            SystemTimeToTzSpecificLocalTime(NULL, &st1, &st2);
            strText.Format(_T("%02u:%02u:%02u"), st2.wHour, st2.wMinute, st2.wSecond);
            m_lcJobInfo.SetItemText(nItem, 4, strText);
        }

        strText.Format(_T("%d of %d"), pJobInfo->GetPagesPrinted(), pJobInfo->GetTotalPages());
        m_lcJobInfo.SetItemText(nItem, 5, strText);

        strText.Format(_T("%d of %d"), pJobInfo->GetBytesPrinted(), pJobInfo->GetTotalBytes());
        m_lcJobInfo.SetItemText(nItem, 6, strText);

        m_lcJobInfo.SetItemText(nItem, 7, pJobInfo->GetStatusCustom());
    }

    return 0;
}


void CPrintManagerDlg::RepositionChildControl( CWnd *pWnd, const int dx, const int dy, const UINT uAnchor )
{
    // make sure the dialog exists
    if (NULL != pWnd->m_hWnd)
    {
        CRect   rect;

        pWnd->GetWindowRect(rect);
        ScreenToClient(rect);

        if (uAnchor & resizeVERT)        // 0x0020
            rect.bottom += dy;
        else if (uAnchor & anchorBOTTOM) // 0x0004
            rect.OffsetRect(0, dy);
        
        if (uAnchor & resizeHORZ)        // 0x0010
            rect.right += dx;
        else if (uAnchor & anchorRIGHT)  // 0x0002
            rect.OffsetRect(dx, 0);

        pWnd->MoveWindow(rect);
    }
}


void CPrintManagerDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
    CDialogEx::OnGetMinMaxInfo(lpMMI);

    // since OnGetMinMaxInfo() gets called very early, make sure we have valid numbers
    if (! m_rectOrig.IsRectEmpty())
    {
        // establish a minimum size
        lpMMI->ptMinTrackSize.x = m_rectOrig.Width() + (GetSystemMetrics(SM_CXFRAME) * 2);
        lpMMI->ptMinTrackSize.y = m_rectOrig.Height() + GetSystemMetrics(SM_CYCAPTION) + (GetSystemMetrics(SM_CXFRAME) * 2);
    }
}


void CPrintManagerDlg::OnSize(UINT nType, int cx, int cy) 
{
    CDialogEx::OnSize(nType, cx, cy);
    
    // how much did the dialog grow/shrink by?
    int nWidthOffset  = cx - m_nWidth;
    int nHeightOffset = cy - m_nHeight;

    RepositionChildControl(&m_lcJobInfo, nWidthOffset, nHeightOffset, anchorTOP | anchorRIGHT | resizeBOTH);
    // RepositionChildControl(&m_btnStart, nWidthOffset, nHeightOffset, anchorTOP | anchorRIGHT);
    // RepositionChildControl(&m_btnStop, nWidthOffset, nHeightOffset, anchorTOP | anchorRIGHT);
    // RepositionChildControl(&m_btnCancel, nWidthOffset, nHeightOffset, anchorTOP | anchorRIGHT);

    // Save dialog's new dimensions
    m_nWidth  = cx;
    m_nHeight = cy;

    CRect rect;
    CRect rect2;
    CRect rect3;
    CRect rect4;
    CWnd* pWnd = GetDlgItem(IDC_LC_JOBINFO);
    if (pWnd != 0x0)
    {
        pWnd->GetWindowRect(&rect);
        ScreenToClient(&rect);

        m_tabcontrol.GetWindowRect(&rect2);
        ScreenToClient(&rect2);

        m_tab1.GetWindowRect(&rect3);
        ScreenToClient(&rect3);

        m_tab1.m_lcPrinters.GetWindowRect(&rect4);
        ScreenToClient(&rect4);

        m_tabcontrol.SetWindowPos(NULL, rect2.left, rect2.top, 
            cx - 21, rect2.bottom - rect2.top + nHeightOffset,
            SWP_NOZORDER);
        
        m_tab1.SetWindowPos(NULL, rcItem.left, rcItem.bottom + 1,
            cx, rect3.bottom - rect3.top + nHeightOffset,
            SWP_NOZORDER);

        m_tab1.m_lcPrinters.SetWindowPos(NULL, 0, 62,
            cx - 21, rect4.bottom - rect4.top + nHeightOffset,
            SWP_NOZORDER);
        
        printf("%d %d", cx, cy);
        
    }

}


void CPrintManagerDlg::OnBnClickedCancel()
{
    CDialogEx::OnCancel();
}


void CPrintManagerDlg::OnTcnSelchangeTabcontrol(NMHDR* pNMHDR, LRESULT* pResult)
{
    // C4100, unreferenced formal parameter 
    pNMHDR;   // Add a reference like this
    
    *pResult = 0;

    int iSel = m_tabcontrol.GetCurSel();

    if (iSel == 0)
    {
        m_tab1.ShowWindow(SW_SHOW);
        m_tab2.ShowWindow(SW_HIDE);
    }
    else if (iSel == 1)
    {
        m_tab2.ShowWindow(SW_SHOW);
        m_tab1.ShowWindow(SW_HIDE);
    }
    else
    {
        m_tab1.ShowWindow(SW_SHOW);
        m_tab2.ShowWindow(SW_HIDE);
    }
}


void CPrintManagerDlg::OnTcnSelchangeIdPreviewPrev(NMHDR* pNMHDR, LRESULT* pResult)
{
    // C4100, unreferenced formal parameter 
    pNMHDR;   // Add a reference like this

    *pResult = 0;
}


void CPrintManagerDlg::OnSelchangeTabcontrol(NMHDR* pNMHDR, LRESULT* pResult)
{
    // C4100, unreferenced formal parameter 
    pNMHDR;   // Add a reference like this
    
    *pResult = 0;

    int iSel = m_tabcontrol.GetCurSel();

    if (iSel == 0)
    {
        m_tab1.ShowWindow(SW_SHOW);
        m_tab2.ShowWindow(SW_HIDE);
    }
    else if (iSel == 1)
    {
        m_tab2.ShowWindow(SW_SHOW);
        m_tab1.ShowWindow(SW_HIDE);
    }
    else
    {
        m_tab1.ShowWindow(SW_SHOW);
        m_tab2.ShowWindow(SW_HIDE);
    }
}
