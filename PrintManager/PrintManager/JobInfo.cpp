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

void CJobInfo::UpdateInfo( const PPRINTER_NOTIFY_INFO_DATA pNotifyData )
{
    if (pNotifyData->Field == JOB_NOTIFY_FIELD_PRINTER_NAME)                     // 0x00
        m_strPrinterName = (LPCTSTR)pNotifyData->NotifyData.Data.pBuf;
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_MACHINE_NAME)                // 0x01
        m_strMachineName = (LPCTSTR)pNotifyData->NotifyData.Data.pBuf;
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_PORT_NAME)                   // 0x02
        m_strPortName = (LPCTSTR)pNotifyData->NotifyData.Data.pBuf;
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_USER_NAME)                   // 0x03
        m_strUserName = (LPCTSTR) pNotifyData->NotifyData.Data.pBuf;
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_NOTIFY_NAME)                 // 0x04
        m_strNotifyName = (LPCTSTR)pNotifyData->NotifyData.Data.pBuf;
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_DATATYPE)                    // 0x05
        m_strDatatype = (LPCTSTR)pNotifyData->NotifyData.Data.pBuf;
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_PRINT_PROCESSOR)             // 0x06
        m_strPrintProcessor = (LPCTSTR)pNotifyData->NotifyData.Data.pBuf;
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_PARAMETERS)                  // 0x07
        m_strParameters = (LPCTSTR)pNotifyData->NotifyData.Data.pBuf;
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_DRIVER_NAME)                 // 0x08
        m_strDriverName = (LPCTSTR)pNotifyData->NotifyData.Data.pBuf;
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_DEVMODE)                     // 0x09
        m_strDevmode = (LPCTSTR)pNotifyData->NotifyData.Data.pBuf;
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_STATUS)                      // 0x0A
        m_nStatus = (int)pNotifyData->NotifyData.Data.pBuf;
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_STATUS_STRING)               // 0x0B
        m_strStatusString = (LPCTSTR)pNotifyData->NotifyData.Data.pBuf;
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_SECURITY_DESCRIPTOR)         // 0x0C
        m_strSecurityDescripor = (LPCTSTR)pNotifyData->NotifyData.Data.pBuf;
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_DOCUMENT)                    // 0x0D
        m_strDocument = (LPCTSTR) pNotifyData->NotifyData.Data.pBuf;
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_PRIORITY)                    // 0x0E
        m_nPriority = (int)pNotifyData->NotifyData.Data.pBuf;
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_POSITION)                    // 0x0F
        m_nPosition = (int)pNotifyData->NotifyData.Data.pBuf;
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_SUBMITTED)                   // 0x10
        m_timeSubmitted = *((SYSTEMTIME *) pNotifyData->NotifyData.Data.pBuf);
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_START_TIME)                  // 0x11
        m_nStartTime = (int)pNotifyData->NotifyData.Data.pBuf;
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_UNTIL_TIME)                  // 0x12
        m_nUntilTime = (int)pNotifyData->NotifyData.Data.pBuf;
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_TIME)                        // 0x13
        m_nTime = (int)pNotifyData->NotifyData.Data.pBuf;
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_TOTAL_PAGES)                 // 0x14
        m_nTotalPages = pNotifyData->NotifyData.adwData[0];
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_PAGES_PRINTED)               // 0x15
        m_nPagesPrinted = pNotifyData->NotifyData.adwData[0];
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_TOTAL_BYTES)                 // 0x16
        m_nTotalBytes = pNotifyData->NotifyData.adwData[0];
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_BYTES_PRINTED)               // 0x17
        m_nBytesPrinted = pNotifyData->NotifyData.adwData[0];
    else
        TRACE(_T("Unhandled job field: %x\n"), pNotifyData->Field);
}

//=================================================================

int CJobInfo::GetJobId( void ) const
{
    return m_nJobId;
}

LPCTSTR CJobInfo::GetUserName( void ) const
{
    return m_strUserName;
}

LPCTSTR CJobInfo::GetMachineName( void ) const
{
    return m_strMachineName;
}

LPCTSTR CJobInfo::GetPortName( void ) const
{
    return m_strPortName;
}

LPCTSTR CJobInfo::GetDocument( void ) const
{
    return m_strDocument;
}

SYSTEMTIME CJobInfo::GetTimeSubmitted( void ) const
{
    return m_timeSubmitted;
}

int CJobInfo::GetTotalPages( void ) const
{
    return m_nTotalPages;
}

int CJobInfo::GetPagesPrinted( void ) const
{
    return m_nPagesPrinted;
}

int CJobInfo::GetTotalBytes( void ) const
{                
    return m_nTotalBytes;
}

int CJobInfo::GetBytesPrinted( void ) const
{
    return m_nBytesPrinted;
}

LPCTSTR CJobInfo::GetStatusCustom( void ) const
{
    LPCTSTR lpszStatus = NULL;

    m_mapJobStatus.Lookup(m_nStatus, lpszStatus);

    return lpszStatus;
}
