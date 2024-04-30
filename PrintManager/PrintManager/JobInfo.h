#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CJobInfo
{
public:
    CJobInfo( const int nJobId );

    //=================================================================

    int GetJobId(void) const;
    LPCTSTR GetPrinterName(void) const;
    LPCTSTR GetMachineName(void) const;
    LPCTSTR GetPortName(void) const;
    LPCTSTR GetUserName(void) const;
    LPCTSTR GetNotifyName(void) const;
    LPCTSTR GetDatatype(void) const;
    LPCTSTR GetPrintProcessor(void) const;
    LPCTSTR GetParameters(void) const;
    LPCTSTR GetDriverName(void) const;
    LPCTSTR GetDevmode(void) const;
    int GetStatusX(void) const;
    LPCTSTR GetStatusString(void) const;
    LPCTSTR GetSecurityDescriptor(void) const;
    LPCTSTR GetDocument(void) const;
    int GetPriority(void) const;
    int GetPosition(void) const;
    SYSTEMTIME GetTimeSubmitted(void) const;
    int GetStartTime(void) const;
    int GetUntilTime(void) const;
    int GetTime(void) const;
    int GetTotalPages(void) const;
    int GetPagesPrinted(void) const;
    int GetTotalBytes(void) const;
    int GetBytesPrinted(void) const;
    LPCTSTR GetStatusCustom(void) const;

    //=================================================================
    
    void UpdateInfo( const PPRINTER_NOTIFY_INFO_DATA pNotifyData );

private:
    static CMap<int, int, LPCTSTR, LPCTSTR> m_mapJobStatus;    

    int m_nJobId;

    CString m_strPrinterName;        // 0x00
    CString m_strMachineName;        // 0x01
    CString m_strPortName;           // 0x02
    CString m_strUserName;           // 0x03
    CString m_strNotifyName;         // 0x04
    CString m_strDatatype;           // 0x05
    CString m_strPrintProcessor;     // 0x06
    CString m_strParameters;         // 0x07
    CString m_strDriverName;         // 0x08
    CString m_strDevmode;            // 0x09
    int m_nStatus;                   // 0x0A
    CString m_strStatusString;       // 0x0B
    CString m_strSecurityDescripor;  // 0x0C
    CString m_strDocument;           // 0x0D
    int m_nPriority;                 // 0x0E
    int m_nPosition;                 // 0x0F
    SYSTEMTIME m_timeSubmitted;      // 0x10
    int m_nStartTime;                // 0x11
    int m_UntilTime;                 // 0x12
    int mTime;                       // 0x13
    int m_nTotalPages;               // 0x14
    int m_nPagesPrinted;             // 0x15
    int m_nTotalBytes;               // 0x16
    int m_nBytesPrinted;             // 0x17
    
};

