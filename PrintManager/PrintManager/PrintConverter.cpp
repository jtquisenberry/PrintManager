#include "stdafx.h"
#include "PrintConverter.h"
#include <thread>
#include "ThreadUtils.h"
#include <cstdio>
#include <ctime>

#ifdef __cplusplus
extern "C" {
#include "plapi.h"  /* GSAPI - gpdf version */
}
#endif


/* We hold details of each threads working in a thread_data
 * structure. */
typedef struct
{
	/* What worker number are we ? */
	int thread_num;
	/* What input file? should this worker use? */
	char* in_file;
	/* Somewhere to store the thread id */
	char* out_file;
	char* printer_name;
	HANDLE thread;
	/* exit code for the thread */
	int code;
} thread_data;

/* A list of input files to run. */
const char* in_files[] =
{
  "examples/tiger.eps",
  "examples/golfer.eps",
  "examples/escher.ps",
  "examples/snowflak.ps",
  "examples/grashopp.pcl",
  "examples/owl.pcl",
  "examples/tiger.xps"
};

#define NUM_INPUTS (sizeof(in_files)/sizeof(*in_files))
#define NUM_WORKERS (10)

static DWORD WINAPI worker(void* td_);

PrintConverter::PrintConverter()
{
	// m_pEventSubscriberThreadDone = new CEvent(TRUE, TRUE);     // signaled
	// m_pEventSubscriberStopRequested = new CEvent(FALSE, TRUE); // non-signaled

	// Print thread ID
	ThreadUtils::OutputThreadId(L"PrintConverter::PrintConverter", g_fileSystem);

	m_boolKeepRunning = true;

	m_hPrinter = INVALID_HANDLE_VALUE;
	m_hEventStopRequested = INVALID_HANDLE_VALUE;
	m_hEventThreadDone = INVALID_HANDLE_VALUE;
	m_hWnd = NULL;
	m_PrintStack = NULL;
	m_strSpoolDirectory = L"";
	m_strSpoolFile = L"";
	m_strFqSpoolFile = L"";
	m_strOutputDirectory = L"D:\\w\\prints\\";
	m_strOutputFile = L"";
	m_strFqOutputFile = L"";

	return;
}


PrintConverter::~PrintConverter()
{
	// Print thread ID
	ThreadUtils::OutputThreadId(L"PrintConverter::~PrintConverter", g_fileSystem);

	// delete m_pEventSubscriberThreadDone;
	// delete m_pEventSubscriberStopRequested;
}


void PrintConverter::SetStopRequestedEvent(HANDLE hEventStopRequested)
{
	m_hEventStopRequested = hEventStopRequested;
}


void PrintConverter::SetThreadDoneEvent(HANDLE hEventThreadDone)
{
	m_hEventThreadDone = hEventThreadDone;
}


HANDLE PrintConverter::GetStopRequestedEvent(void)
{
	return m_hEventStopRequested;
}


HANDLE PrintConverter::GetThreadDoneEvent(void)
{
	return m_hEventThreadDone;
}


UINT PrintConverter::Start(LPVOID pParam)
{
	
	// Print thread ID
	ThreadUtils::OutputThreadId(L"PrintConverter::Start", g_fileSystem);
	
	// C4100 unreferenced formal parameter
	pParam;

	// Compute spool directory
	SetSpoolDirectory();

	// If GetStopRequestedEvent is never set to true, the while clause
	// produces a memory leak.
	// D:\a\_work\1\s\src\vctools\VC7Libs\Ship\ATLMFC\Src\MFC\thrdcore.cpp(306) : {1357} client block at 0x0000017BE0A29E10, subtype c0, 136 bytes long.
    // D:\a\_work\1\s\src\vctools\VC7Libs\Ship\ATLMFC\Src\MFC\dumpcont.cpp(23) : atlTraceGeneral - a CWinThread object at $0000017BE0A29E10, 136 bytes long
	while (WaitForSingleObject(GetStopRequestedEvent(), 0U) != WAIT_OBJECT_0)
	{
		if (m_PrintStack->size() > 0)
		{
			int job_id = 0;
			job_id = m_PrintStack->back();

			// Print Job ID
			wchar_t buffer[100];
			int cx;
			cx = swprintf(buffer, 100, L"job_id ID: %d \n", job_id);
			OutputDebugString(buffer);

			// Perform conversion
			SetSpoolFileString(job_id);
			ConvertMain();

			m_PrintStack->pop_back();
		}

		Sleep(2000);

	}
	
	// Signal the event to let the primary thread know that this thread is done
	SetEvent(GetThreadDoneEvent());

	return 0;
}


void PrintConverter::SetSpoolDirectory()
{
	// _dupenv_s is safer than getenv
	char* base_path;
	size_t len;
	errno_t err = _dupenv_s(&base_path, &len, "SYSTEMROOT");
	if (err)
	{
		m_strSpoolDirectory = L"";
	}
	else
	{
		m_strSpoolDirectory = (CString)base_path + L"\\System32\\Spool\\Printers\\";
		int xxx = 0;
		xxx++;
	}
}


void PrintConverter::SetConverterFiles(int JobId)
{
	wchar_t buffer[100];
	swprintf(buffer, 100, L"%05d.SPL", JobId);
	m_strSpoolFile = (CString)buffer;
	m_strFqSpoolFile = m_strSpoolDirectory + m_strSpoolFile;
	swprintf(buffer, 100, L"%05d.pdf", JobId);
	m_strFqOutputFile = m_strOutputDirectory + m_strFqOutputFile;
}


void PrintConverter::SetOutputPrinters(std::vector<CString> vectOutputPrinters)
{
	m_vectOutputPrinters = vectOutputPrinters;
}


CString PrintConverter::TimeToString()
{
	std::time_t rawtime;
	std::tm* timeinfo;
	char buffer[80];
	std::time(&rawtime);
	timeinfo = std::localtime(&rawtime);
	std::strftime(buffer, 80, "%Y-%m-%d-%H-%M-%S", timeinfo);
	CString strTime = (CString)buffer;
}


int PrintConverter::ConvertMain()
{
	// Convert CString to char*
	CT2A asciiFqSpoolFile(m_strFqSpoolFile);
	CT2A asciiFqOutputFile(m_strFqOutputFile);

	int failed = 0;
	int code = 0;

	thread_data td;
	td.in_file = asciiFqSpoolFile;
	td.out_file = asciiFqOutputFile;
	td.thread_num = 0;
	td.thread = CreateThread(NULL, 0, worker, &td, 0, NULL);
	
	WaitForSingleObject(td.thread, INFINITE);
	fprintf(stderr, "Thread %d finished with %d\n", td.thread_num, td.code);

	if (td.code != 0)
		failed += 1;
	
	OutputDebugString(L"CONVERSION TO PDF DONE");
	
	int nCountPrinters = m_vectOutputPrinters.size();

	int thread_counter = 0;
	for (CString printer : m_vectOutputPrinters)
	{
		thread_counter++;
		td.in_file = asciiFqOutputFile;
		td.thread_num = thread_counter;
		td.thread = CreateThread(NULL, 0, worker, &td, 0, NULL);
		WaitForSingleObject(td.thread, INFINITE);
		if (td.code != 0)
			failed += 1;
	}

	return failed;
}


/* The function to perform the work of the thread.
 * Starts a gs instance, runs a file, shuts it down.
 */
static DWORD WINAPI worker(void* td_)
{
	thread_data* td = (thread_data*)td_;
	int code;
	void* instance = NULL;
	char out[32];

	/* Construct the argc/argv to pass to ghostscript. */
	int argc = 0;
	const char* argv[10];

	argv[argc++] = "gpdl";
	argv[argc++] = "-sDEVICE=pdfwrite";
	argv[argc++] = "-o";
	argv[argc++] = td->out_file;
	argv[argc++] = "-r100";
	argv[argc++] = td->in_file;

	/* Create a GS instance. */
	code = gsapi_new_instance(&instance, NULL);
	if (code < 0) {
		printf("Error %d in gsapi_new_instance\n", code);
		goto failearly;
	}

	/* Run our test. */
	code = gsapi_init_with_args(instance, argc, (char**)argv);
	if (code < 0) {
		printf("Error %d in gsapi_init_with_args\n", code);
		goto fail;
	}

	/* Close the interpreter down (important, or we will leak!) */
	code = gsapi_exit(instance);
	if (code < 0) {
		printf("Error %d in gsapi_exit\n", code);
		goto fail;
	}

fail:
	/* Delete the gs instance. */
	gsapi_delete_instance(instance);

failearly:
	td->code = code;

	return 0;
}

