#include "stdafx.h"
#include "PrintManager.h"
#include "PrintManagerDlg.h"
#include "LogFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

FILE* file1 = NULL;  // Declared externally
FILE* file2 = NULL;  // Declared externally



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
	char* base_path = getenv("USERPROFILE");
	char path1[260];
	char path2[260];
	memset(&path1[0], 0, sizeof(path1));
	memset(&path2[0], 0, sizeof(path1));
	strcat(path1, base_path);
	strcat(path1, "\\print_manager_system.txt");
	strcat(path2, base_path);
	strcat(path2, "\\print_manager_objects.txt");

	file1 = fopen(path1, "a+");
	file2 = fopen(path2, "a+");

	fwprintf_s(file1, L"\nSTART\n");
	fwprintf_s(file1, L"---------------------------------------\n\n");
	fwprintf_s(file2, L"\nSTART\n");
	fwprintf_s(file2, L"---------------------------------------\n\n");
	fflush(file1);
	fflush(file2);
	
	CPrintManagerDlg dlg;

	m_pMainWnd = &dlg;
	
    dlg.DoModal();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
