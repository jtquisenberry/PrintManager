#include "stdafx.h"
#include "PrintManager.h"
#include "PrintManagerDlg.h"
#include "LogFile.h"
#include "ThreadUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

FILE* g_fileSystem = NULL;  // Declared externally
FILE* g_fileObjects = NULL;  // Declared externally


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

	// Debug memory leak on normal block # value in parentheses
	// _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// _CrtSetBreakAlloc(1546);
	// _CrtSetBreakAlloc(3551);

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	OpenLogs();
	StartLogs();
	ThreadUtils::OutputThreadId(L"CPrintManagerApp::CPrintManagerApp", g_fileSystem);
}

CPrintManagerApp::~CPrintManagerApp()
{
	// Destructor
	ThreadUtils::OutputThreadId(L"CPrintManagerApp::~CPrintManagerApp", g_fileSystem);
	CloseLogs();
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPrintManagerApp object

CPrintManagerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPrintManagerApp initialization

BOOL CPrintManagerApp::InitInstance()
{
	ThreadUtils::OutputThreadId(L"CPrintManagerApp::InitInstance", g_fileSystem);	
	
	CPrintManagerDlg dlg;
	m_pMainWnd = &dlg;
    dlg.DoModal();

	// This code is not reached until the dialog is closed.
	// Since the dialog has been closed, return FALSE so that the application
	// exits, rather than starting the application's message pump.
	return FALSE;
}

BOOL CPrintManagerApp::OpenLogs()
{
	// _dupenv_s is safer than getenv
	char* base_path;
	size_t len;
	errno_t err = _dupenv_s(&base_path, &len, "USERPROFILE");
	if (err) return FALSE;

	// Build log paths
	char str_SystemPath[260];
	char str_ObjectsPath[260];
	memset(&str_SystemPath[0], 0, sizeof(str_SystemPath));
	memset(&str_ObjectsPath[0], 0, sizeof(str_ObjectsPath));
	if (base_path)
	{
		strcat_s(str_SystemPath, 260, base_path);
		strcat_s(str_SystemPath, 260, "\\print_manager_system.txt");
		strcat_s(str_ObjectsPath, 260, base_path);
		strcat_s(str_ObjectsPath, 260, "\\print_manager_objects.txt");
	}
	else
	{
		return FALSE;
	}

	// Open files
	fopen_s(&g_fileSystem, str_SystemPath, "a+");
	fopen_s(&g_fileObjects, str_ObjectsPath, "a+");

	delete base_path;

	return TRUE;
}

BOOL CPrintManagerApp::StartLogs()
{
	fwprintf_s(g_fileSystem, L"\nSTART\n");
	fwprintf_s(g_fileSystem, L"---------------------------------------\n\n");
	ThreadUtils::OutputThreadId(L"CPrintManagerApp::StartLogs", g_fileSystem);
	fwprintf_s(g_fileObjects, L"\nSTART\n");
	fwprintf_s(g_fileObjects, L"---------------------------------------\n\n");
	fflush(g_fileSystem);
	fflush(g_fileObjects);
	return TRUE;
}


BOOL CPrintManagerApp::CloseLogs()
{
	ThreadUtils::OutputThreadId(L"CPrintManagerApp::CloseLogs", g_fileSystem);
	fclose(g_fileSystem);
	fclose(g_fileObjects);
	return TRUE;
}