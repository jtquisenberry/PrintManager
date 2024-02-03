#include "stdafx.h"
#include "PrintManager.h"
#include "PrintManagerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrintManagerApp

BEGIN_MESSAGE_MAP(CPrintManagerApp, CWinApp)
	//{{AFX_MSG_MAP(CPrintManagerApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrintManagerApp construction

CPrintManagerApp::CPrintManagerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPrintManagerApp object

CPrintManagerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPrintManagerApp initialization

BOOL CPrintManagerApp::InitInstance()
{
	CPrintManagerDlg dlg;

	m_pMainWnd = &dlg;
	
    dlg.DoModal();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
