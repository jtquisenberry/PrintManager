#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CJobInfo
{
public:
    CJobInfo( const int nJobId );

    //=================================================================

    int GetJobId(void) const;
    LPCTSTR GetPrinterName(void) const;            // 0x00
    LPCTSTR GetMachineName(void) const;            // 0x01
    LPCTSTR GetPortName(void) const;               // 0x02
    LPCTSTR GetUserName(void) const;               // 0x03
    LPCTSTR GetNotifyName(void) const;             // 0x04
    LPCTSTR GetDatatype(void) const;               // 0x05
    LPCTSTR GetPrintProcessor(void) const;         // 0x06
    LPCTSTR GetParameters(void) const;             // 0x07
    LPCTSTR GetDriverName(void) const;             // 0x08
    LPCTSTR GetDevmode(void) const;                // 0x09
    int GetStatusX(void) const;                    // 0x0A
    LPCTSTR GetStatusString(void) const;           // 0x0B
    LPCTSTR GetSecurityDescriptor(void) const;     // 0x0C
    LPCTSTR GetDocument(void) const;               // 0x0D
    int GetPriority(void) const;                   // 0x0E
    int GetPosition(void) const;                   // 0x0F
    SYSTEMTIME GetTimeSubmitted(void) const;       // 0x10
    int GetStartTime(void) const;                  // 0x11
    int GetUntilTime(void) const;                  // 0x12
    int GetTime(void) const;                       // 0x13
    int GetTotalPages(void) const;                 // 0x14
    int GetPagesPrinted(void) const;               // 0x15
    int GetTotalBytes(void) const;                 // 0x16
    int GetBytesPrinted(void) const;               // 0x17
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
    int m_nUntilTime;                // 0x12
    int m_nTime;                     // 0x13
    int m_nTotalPages;               // 0x14
    int m_nPagesPrinted;             // 0x15
    int m_nTotalBytes;               // 0x16
    int m_nBytesPrinted;             // 0x17
    
};

