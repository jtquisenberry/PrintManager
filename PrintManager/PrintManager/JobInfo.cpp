#include "stdafx.h"
#include "JobInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CJobInfo::CJobInfo( const int nJobId )
{
    m_nJobId              = nJobId;       
    m_timeSubmitted.wHour = USHRT_MAX; // just some invalid value
    m_nTotalPages         = 0;
    m_nPagesPrinted       = 0;
    m_nTotalBytes         = 0;
    m_nBytesPrinted       = 0;

    // since m_mapJobStatus is used by all instances, we only want to populate it once
    if (m_mapJobStatus.IsEmpty())
    {
        m_mapJobStatus.SetAt(JOB_STATUS_BLOCKED_DEVQ, _T("The driver cannot print the job."));
        m_mapJobStatus.SetAt(JOB_STATUS_COMPLETE, _T("Job is sent to the printer, but the job may not be printed yet."));
        m_mapJobStatus.SetAt(JOB_STATUS_DELETED, _T("Job has been deleted."));
        m_mapJobStatus.SetAt(JOB_STATUS_DELETING, _T("Job is being deleted."));
        m_mapJobStatus.SetAt(JOB_STATUS_ERROR, _T("An error is associated with the job."));
        m_mapJobStatus.SetAt(JOB_STATUS_OFFLINE, _T("Printer is offline."));
        m_mapJobStatus.SetAt(JOB_STATUS_PAPEROUT, _T("Printer is out of paper."));
        m_mapJobStatus.SetAt(JOB_STATUS_PAUSED, _T("Job is paused."));
        m_mapJobStatus.SetAt(JOB_STATUS_PRINTED, _T("Job has printed."));
        m_mapJobStatus.SetAt(JOB_STATUS_PRINTING, _T("Job is printing."));
        m_mapJobStatus.SetAt(JOB_STATUS_RESTART, _T("Job has been restarted."));
        m_mapJobStatus.SetAt(JOB_STATUS_SPOOLING, _T("Job is spooling."));
        m_mapJobStatus.SetAt(JOB_STATUS_USER_INTERVENTION, _T("Printer has an error that requires the user to do something."));
    }
}

CMap<int, int, LPCTSTR, LPCTSTR> CJobInfo::m_mapJobStatus;    

//=================================================================

int CJobInfo::BuildString()
{
    wchar_t buffer[1000];
    int cx;
    cx = swprintf(buffer, 1000,
        L"%- 30s %d\n ", L"aaa", m_nJobId);

    
    cx = swprintf(buffer, 1000,
        L"%- 30s %d\n "
        L"%- 30s %s\n "
        L"%- 30s %s\n "
        L"%- 30s %s\n "
        L"%- 30s %s\n "
        L"%- 30s %s\n "
        L"%- 30s %s\n "
        L"%- 30s %s\n "
        L"%- 30s %s\n "
        L"%- 30s %s\n "
        L"%- 30s %s\n "
        L"%- 30s %d\n "
        L"%- 30s %s\n "
        L"%- 30s %s\n "
        L"%- 30s %s\n "
        L"%- 30s %d\n "
        L"%- 30s %d\n "
        //L"%- 30s %d\n "
        //L"%- 30s %d\n "
        //L"%- 30s %d\n "
        //L"%- 30s %d\n "
        //L"%- 30s %d\n "
        //L"%- 30s %d\n "
        //L"%- 30s %d\n "
        L"%- 30s %d\n ",
        L"JobId", m_nJobId,
        L"PrinterName", m_strPrinterName,
        L"MachineName", m_strMachineName,
        L"PortName", m_strPortName,
        L"UserName", m_strUserName,
        L"NotifyName", m_strNotifyName,
        L"Datatype", m_strDatatype,
        L"PrintProcessor", m_strPrintProcessor,
        L"Parameters", m_strParameters,
        L"DriverName", m_strDriverName,
        L"Devmode", m_strDevmode,
        L"Status", m_nStatus,
        L"StatusString", m_strStatusString,
        L"SecurityDescriptor", m_strSecurityDescriptor,
        L"Document", m_strDocument,
        L"Priority", m_nPriority,
        L"Position", m_nPosition,
        //L"Submitted", m_timeSubmitted,
        //L"StartTime", m_nStartTime,
        //L"UntilTime", m_nUntilTime,
        //L"Time", m_nTime,
        //L"TotalPages", m_nTotalPages,
        //L"PagesPrinted", m_nPagesPrinted,
        //L"TotalBytes", m_nTotalBytes,
        L"BytesPrinted", m_nBytesPrinted
        );
        
        
        
        
        /*
        
        L"%- 30s %s\n "
        L"%- 30s %d\n "
        L"%- 30s %s\n "
        L"%- 30s %d\n "
        L"%- 30s %d\n "
        L"%- 30s %d\n "
        L"%- 30s %d\n "
        L"%- 30s %d\n "
        L"%- 30s %d\n "
        L"%- 30s %d\n "
        L"%- 30s %d\n "
        
        
        
        ,
        
                
        );
        */    


    OutputDebugString(buffer);
    return 0;
}

void CJobInfo::UpdateInfo( const PPRINTER_NOTIFY_INFO_DATA pNotifyData )
{
    if (pNotifyData->Field == JOB_NOTIFY_FIELD_PRINTER_NAME)                     // 0x00
        // m_strPrinterName = (LPCTSTR)pNotifyData->NotifyData.Data.pBuf;
        SetPrinterName(pNotifyData);
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_MACHINE_NAME)                // 0x01
        // m_strMachineName = (LPCTSTR)pNotifyData->NotifyData.Data.pBuf;
        SetMachineName(pNotifyData);
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_PORT_NAME)                   // 0x02
        // m_strPortName = (LPCTSTR)pNotifyData->NotifyData.Data.pBuf;
        SetPortName(pNotifyData);
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_USER_NAME)                   // 0x03
        // m_strUserName = (LPCTSTR) pNotifyData->NotifyData.Data.pBuf;
        SetUserName(pNotifyData);
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_NOTIFY_NAME)                 // 0x04
        // m_strNotifyName = (LPCTSTR)pNotifyData->NotifyData.Data.pBuf;
        SetNotifyName(pNotifyData);
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_DATATYPE)                    // 0x05
        // m_strDatatype = (LPCTSTR)pNotifyData->NotifyData.Data.pBuf;
        SetDatatype(pNotifyData);
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_PRINT_PROCESSOR)             // 0x06
        // m_strPrintProcessor = (LPCTSTR)pNotifyData->NotifyData.Data.pBuf;
        SetPrintProcessor(pNotifyData);
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_PARAMETERS)                  // 0x07
        // m_strParameters = (LPCTSTR)pNotifyData->NotifyData.Data.pBuf;
        SetParameters(pNotifyData);
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_DRIVER_NAME)                 // 0x08
        // m_strDriverName = (LPCTSTR)pNotifyData->NotifyData.Data.pBuf;
        SetDriverName(pNotifyData);
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_DEVMODE)                     // 0x09
        // m_strDevmode = (LPCTSTR)pNotifyData->NotifyData.Data.pBuf;
        SetDevmode(pNotifyData);
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_STATUS)                      // 0x0A
        // m_nStatus = (int)pNotifyData->NotifyData.Data.pBuf;
        SetStatus(pNotifyData);
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_STATUS_STRING)               // 0x0B
        // m_strStatusString = (LPCTSTR)pNotifyData->NotifyData.Data.pBuf;
        SetStatusString(pNotifyData);
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_SECURITY_DESCRIPTOR)         // 0x0C
        // m_strSecurityDescriptor = (LPCTSTR)pNotifyData->NotifyData.Data.pBuf;
        SetSecurityDescriptor(pNotifyData);
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_DOCUMENT)                    // 0x0D
        // m_strDocument = (LPCTSTR) pNotifyData->NotifyData.Data.pBuf;
        SetDocument(pNotifyData);
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_PRIORITY)                    // 0x0E
        // m_nPriority = (int)pNotifyData->NotifyData.Data.pBuf;
        SetPriority(pNotifyData);
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_POSITION)                    // 0x0F
        // m_nPosition = (int)pNotifyData->NotifyData.Data.pBuf;
        SetPosition(pNotifyData);
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_SUBMITTED)                   // 0x10
        // m_timeSubmitted = *((SYSTEMTIME *) pNotifyData->NotifyData.Data.pBuf);
        SetSubmitted(pNotifyData);
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_START_TIME)                  // 0x11
        // m_nStartTime = (int)pNotifyData->NotifyData.Data.pBuf;
        SetStartTime(pNotifyData);
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_UNTIL_TIME)                  // 0x12
        // m_nUntilTime = (int)pNotifyData->NotifyData.Data.pBuf;
        SetUntilTime(pNotifyData);
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_TIME)                        // 0x13
        // m_nTime = (int)pNotifyData->NotifyData.Data.pBuf;
        SetTime(pNotifyData);
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_TOTAL_PAGES)                 // 0x14
        // m_nTotalPages = pNotifyData->NotifyData.adwData[0];
        SetTotalPages(pNotifyData);
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_PAGES_PRINTED)               // 0x15
        // m_nPagesPrinted = pNotifyData->NotifyData.adwData[0];
        SetPagesPrinted(pNotifyData);
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_TOTAL_BYTES)                 // 0x16
        // m_nTotalBytes = pNotifyData->NotifyData.adwData[0];
        SetTotalBytes(pNotifyData);
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_BYTES_PRINTED)               // 0x17
        // m_nBytesPrinted = pNotifyData->NotifyData.adwData[0];
        SetBytesPrinted(pNotifyData);
    else
        TRACE(_T("Unhandled job field: %x\n"), pNotifyData->Field);
}

//=================================================================

int CJobInfo::GetJobId( void ) const
{
    return m_nJobId;
}

LPCTSTR CJobInfo::GetPrinterName(void) const
{
    return m_strPrinterName;
}

void CJobInfo::SetPrinterName(const PPRINTER_NOTIFY_INFO_DATA pNotifyData)
{
    m_strPrinterName = (LPCTSTR)pNotifyData->NotifyData.Data.pBuf;
    return;
}

LPCTSTR CJobInfo::GetMachineName(void) const
{
    return m_strMachineName;
}

void CJobInfo::SetMachineName(const PPRINTER_NOTIFY_INFO_DATA pNotifyData)
{
    m_strMachineName = (LPCTSTR)pNotifyData->NotifyData.Data.pBuf;
    return;
}

LPCTSTR CJobInfo::GetPortName(void) const
{
    return m_strPortName;
}

void CJobInfo::SetPortName(const PPRINTER_NOTIFY_INFO_DATA pNotifyData)
{
    m_strPortName = (LPCTSTR)pNotifyData->NotifyData.Data.pBuf;
    return;
}

LPCTSTR CJobInfo::GetUserName( void ) const
{
    return m_strUserName;
}

void CJobInfo::SetUserName(const PPRINTER_NOTIFY_INFO_DATA pNotifyData)
{
    m_strUserName = (LPCTSTR)pNotifyData->NotifyData.Data.pBuf;
    return;
}

LPCTSTR CJobInfo::GetNotifyName(void) const
{
    return m_strNotifyName;
}

void CJobInfo::SetNotifyName(const PPRINTER_NOTIFY_INFO_DATA pNotifyData)
{
    m_strNotifyName = (LPCTSTR)pNotifyData->NotifyData.Data.pBuf;
    return;
}

LPCTSTR CJobInfo::GetDatatype(void) const
{
    return m_strDatatype;
}

void CJobInfo::SetDatatype(const PPRINTER_NOTIFY_INFO_DATA pNotifyData)
{
    m_strDatatype = (LPCTSTR)pNotifyData->NotifyData.Data.pBuf;
    return;
}

LPCTSTR CJobInfo::GetPrintProcessor(void) const
{
    return m_strPrintProcessor;
}

void CJobInfo::SetPrintProcessor(const PPRINTER_NOTIFY_INFO_DATA pNotifyData)
{
    m_strPrintProcessor = (LPCTSTR)pNotifyData->NotifyData.Data.pBuf;
    return;
}

LPCTSTR CJobInfo::GetParameters(void) const
{
    return m_strParameters;
}

void CJobInfo::SetParameters(const PPRINTER_NOTIFY_INFO_DATA pNotifyData)
{
    m_strParameters = (LPCTSTR)pNotifyData->NotifyData.Data.pBuf;
    return;
}

LPCTSTR CJobInfo::GetDriverName( void ) const
{
    return m_strDriverName;
}

void CJobInfo::SetDriverName(const PPRINTER_NOTIFY_INFO_DATA pNotifyData)
{
    m_strDriverName = (LPCTSTR)pNotifyData->NotifyData.Data.pBuf;
    return;
}

LPCTSTR CJobInfo::GetDevmode(void) const
{
    return m_strDevmode;
}

void CJobInfo::SetDevmode(const PPRINTER_NOTIFY_INFO_DATA pNotifyData)
{
    m_devDevmode = (DEVMODE*)pNotifyData->NotifyData.Data.pBuf;
    m_strDevmode = L"DEVMODE PLACEHOLDER";
    return;
}

int CJobInfo::GetStatus(void) const
{
    return m_nStatus;
}

void CJobInfo::SetStatus(const PPRINTER_NOTIFY_INFO_DATA pNotifyData)
{
    m_nStatus = (int)pNotifyData->NotifyData.Data.pBuf;
    return;
}

LPCTSTR CJobInfo::GetStatusString(void) const
{
    return m_strStatusString;
}

void CJobInfo::SetStatusString(const PPRINTER_NOTIFY_INFO_DATA pNotifyData)
{
    m_strStatusString = (LPCTSTR)pNotifyData->NotifyData.Data.pBuf;
    return;
}

LPCTSTR CJobInfo::GetSecurityDescriptor(void) const
{
    return m_strSecurityDescriptor;
}

void CJobInfo::SetSecurityDescriptor(const PPRINTER_NOTIFY_INFO_DATA pNotifyData)
{
    //m_strSecurityDescriptor = (LPCTSTR)pNotifyData->NotifyData.Data.pBuf;
    m_strSecurityDescriptor = L"UNSUPPORTED";
    return;
}

LPCTSTR CJobInfo::GetDocument(void) const
{
    return m_strDocument;
}

void CJobInfo::SetDocument(const PPRINTER_NOTIFY_INFO_DATA pNotifyData)
{
    m_strDocument = (LPCTSTR)pNotifyData->NotifyData.Data.pBuf;
    return;
}

int CJobInfo::GetPriority(void) const
{
    return m_nPriority;
}

void CJobInfo::SetPriority(const PPRINTER_NOTIFY_INFO_DATA pNotifyData)
{
    m_nPriority = (int)pNotifyData->NotifyData.Data.pBuf;
    return;
}

int CJobInfo::GetPosition(void) const
{
    return m_nPosition;
}

void CJobInfo::SetPosition(const PPRINTER_NOTIFY_INFO_DATA pNotifyData)
{
    m_nPosition = (int)pNotifyData->NotifyData.Data.pBuf;
    return;
}

SYSTEMTIME CJobInfo::GetSubmitted( void ) const
{
    return m_timeSubmitted;
}

void CJobInfo::SetSubmitted(const PPRINTER_NOTIFY_INFO_DATA pNotifyData)
{
    m_timeSubmitted = *((SYSTEMTIME*)pNotifyData->NotifyData.Data.pBuf);
    return;
}

int CJobInfo::GetStartTime(void) const
{
    return m_nStartTime;
}

void CJobInfo::SetStartTime(const PPRINTER_NOTIFY_INFO_DATA pNotifyData)
{
    m_nStartTime = (int)pNotifyData->NotifyData.Data.pBuf;
    return;
}

int CJobInfo::GetUntilTime(void) const
{
    return m_nUntilTime;
}

void CJobInfo::SetUntilTime(const PPRINTER_NOTIFY_INFO_DATA pNotifyData)
{
    m_nUntilTime = (int)pNotifyData->NotifyData.Data.pBuf;
    return;
}

int CJobInfo::GetTime(void) const
{
    return m_nTime;
}

void CJobInfo::SetTime(const PPRINTER_NOTIFY_INFO_DATA pNotifyData)
{
    m_nTime = (int)pNotifyData->NotifyData.Data.pBuf;
    return;
}

int CJobInfo::GetTotalPages( void ) const
{
    return m_nTotalPages;
}

void CJobInfo::SetTotalPages(const PPRINTER_NOTIFY_INFO_DATA pNotifyData)
{
    m_nTotalPages = (int)pNotifyData->NotifyData.Data.pBuf;
    return;
}

int CJobInfo::GetPagesPrinted( void ) const
{
    return m_nPagesPrinted;
}

void CJobInfo::SetPagesPrinted(const PPRINTER_NOTIFY_INFO_DATA pNotifyData)
{
    m_nPagesPrinted = (int)pNotifyData->NotifyData.Data.pBuf;
    return;
}

int CJobInfo::GetTotalBytes( void ) const
{                
    return m_nTotalBytes;
}

void CJobInfo::SetTotalBytes(const PPRINTER_NOTIFY_INFO_DATA pNotifyData)
{
    m_nTotalBytes = (int)pNotifyData->NotifyData.Data.pBuf;
    return;
}

int CJobInfo::GetBytesPrinted( void ) const
{
    return m_nBytesPrinted;
}

void CJobInfo::SetBytesPrinted(const PPRINTER_NOTIFY_INFO_DATA pNotifyData)
{
    m_nBytesPrinted = (int)pNotifyData->NotifyData.Data.pBuf;
    return;
}

LPCTSTR CJobInfo::GetStatusCustom( void ) const
{
    LPCTSTR lpszStatus = NULL;

    m_mapJobStatus.Lookup(m_nStatus, lpszStatus);

    return lpszStatus;
}
