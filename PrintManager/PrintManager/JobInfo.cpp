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
    if (pNotifyData->Field == JOB_NOTIFY_FIELD_USER_NAME)
        m_strUser = (LPCTSTR) pNotifyData->NotifyData.Data.pBuf;
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_MACHINE_NAME)
        m_strMachine = (LPCTSTR) pNotifyData->NotifyData.Data.pBuf;
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_PORT_NAME)
        m_strPort = (LPCTSTR) pNotifyData->NotifyData.Data.pBuf;
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_DOCUMENT)
        m_strDocument = (LPCTSTR) pNotifyData->NotifyData.Data.pBuf;
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_SUBMITTED)
        m_timeSubmitted = *((SYSTEMTIME *) pNotifyData->NotifyData.Data.pBuf);
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_PAGES_PRINTED)
        m_nPagesPrinted = pNotifyData->NotifyData.adwData[0];
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_TOTAL_PAGES)
        m_nTotalPages = pNotifyData->NotifyData.adwData[0];
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_BYTES_PRINTED)
        m_nBytesPrinted = pNotifyData->NotifyData.adwData[0];
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_TOTAL_BYTES)
        m_nTotalBytes = pNotifyData->NotifyData.adwData[0];
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_STATUS)
        m_nStatus = pNotifyData->NotifyData.adwData[0]; 
    else if (pNotifyData->Field == JOB_NOTIFY_FIELD_STATUS_STRING)
        ;
    else
        TRACE(_T("Unhandled job field: %x\n"), pNotifyData->Field);
}

//=================================================================

int CJobInfo::GetJobId( void ) const
{
    return m_nJobId;
}

LPCTSTR CJobInfo::GetUser( void ) const
{
    return m_strUser;
}

LPCTSTR CJobInfo::GetMachine( void ) const
{
    return m_strMachine;
}

LPCTSTR CJobInfo::GetPort( void ) const
{
    return m_strPort;
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

LPCTSTR CJobInfo::GetStatus( void ) const
{
    LPCTSTR lpszStatus = NULL;

    m_mapJobStatus.Lookup(m_nStatus, lpszStatus);

    return lpszStatus;
}
