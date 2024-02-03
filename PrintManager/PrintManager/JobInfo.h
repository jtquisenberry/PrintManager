#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CJobInfo
{
public:
    CJobInfo( const int nJobId );

    //=================================================================

    int GetJobId( void ) const;
    LPCTSTR GetUser( void ) const;
    LPCTSTR GetMachine( void ) const;
    LPCTSTR GetPort( void ) const;
    LPCTSTR GetDocument( void ) const;
    SYSTEMTIME GetTimeSubmitted( void ) const;
    int GetTotalPages( void ) const;
    int GetPagesPrinted( void ) const;
    int GetTotalBytes( void ) const;
    int GetBytesPrinted( void ) const;
    LPCTSTR GetStatus( void ) const;

    //=================================================================
    
    void UpdateInfo( const PPRINTER_NOTIFY_INFO_DATA pNotifyData );

private:
    static CMap<int, int, LPCTSTR, LPCTSTR> m_mapJobStatus;    

    int m_nJobId;
    CString m_strUser;
    CString m_strMachine;
    CString m_strPort;
    CString m_strDocument;
    SYSTEMTIME m_timeSubmitted;
    int m_nTotalPages;
    int m_nPagesPrinted;
    int m_nTotalBytes;
    int m_nBytesPrinted;
    int m_nStatus;
};

