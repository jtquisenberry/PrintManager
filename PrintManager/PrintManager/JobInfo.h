#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CJobInfo
{
public:
    CJobInfo( const int nJobId );

    //=================================================================

    int GetJobId(void) const;
    LPCTSTR GetPrinterName(void) const;                             // 0x00
    void SetPrinterName(const PPRINTER_NOTIFY_INFO_DATA);
    LPCTSTR GetMachineName(void) const;                             // 0x01
    void SetMachineName(const PPRINTER_NOTIFY_INFO_DATA);
    LPCTSTR GetPortName(void) const;                                // 0x02
    void SetPortName(const PPRINTER_NOTIFY_INFO_DATA);
    LPCTSTR GetUserName(void) const;                                // 0x03
    void SetUserName(const PPRINTER_NOTIFY_INFO_DATA);
    LPCTSTR GetNotifyName(void) const;                              // 0x04
    void SetNotifyName(const PPRINTER_NOTIFY_INFO_DATA);
    LPCTSTR GetDatatype(void) const;                                // 0x05
    void SetDatatype(const PPRINTER_NOTIFY_INFO_DATA);
    LPCTSTR GetPrintProcessor(void) const;                          // 0x06
    void SetPrintProcessor(const PPRINTER_NOTIFY_INFO_DATA);
    LPCTSTR GetParameters(void) const;                              // 0x07
    void SetParameters(const PPRINTER_NOTIFY_INFO_DATA);
    LPCTSTR GetDriverName(void) const;                              // 0x08
    void SetDriverName(const PPRINTER_NOTIFY_INFO_DATA);
    LPCTSTR GetDevmode(void) const;                                 // 0x09
    void SetDevmode(const PPRINTER_NOTIFY_INFO_DATA);
    int GetStatus(void) const;                                     // 0x0A
    void SetStatus(const PPRINTER_NOTIFY_INFO_DATA);
    LPCTSTR GetStatusString(void) const;                            // 0x0B
    void SetStatusString(const PPRINTER_NOTIFY_INFO_DATA);
    LPCTSTR GetSecurityDescriptor(void) const;                      // 0x0C
    void SetSecurityDescriptor(const PPRINTER_NOTIFY_INFO_DATA);
    LPCTSTR GetDocument(void) const;                                // 0x0D
    void SetDocument(const PPRINTER_NOTIFY_INFO_DATA);
    int GetPriority(void) const;                                    // 0x0E
    void SetPriority(const PPRINTER_NOTIFY_INFO_DATA);
    int GetPosition(void) const;                                    // 0x0F
    void SetPosition(const PPRINTER_NOTIFY_INFO_DATA);
    SYSTEMTIME GetSubmitted(void) const;                        // 0x10
    void SetSubmitted(const PPRINTER_NOTIFY_INFO_DATA);
    int GetStartTime(void) const;                                   // 0x11
    void SetStartTime(const PPRINTER_NOTIFY_INFO_DATA);
    int GetUntilTime(void) const;                                   // 0x12
    void SetUntilTime(const PPRINTER_NOTIFY_INFO_DATA);
    int GetTime(void) const;                                        // 0x13
    void SetTime(const PPRINTER_NOTIFY_INFO_DATA);
    int GetTotalPages(void) const;                                  // 0x14
    void SetTotalPages(const PPRINTER_NOTIFY_INFO_DATA);
    int GetPagesPrinted(void) const;                                // 0x15
    void SetPagesPrinted(const PPRINTER_NOTIFY_INFO_DATA);
    int GetTotalBytes(void) const;                                  // 0x16
    void SetTotalBytes(const PPRINTER_NOTIFY_INFO_DATA);
    int GetBytesPrinted(void) const;                                // 0x17
    void SetBytesPrinted(const PPRINTER_NOTIFY_INFO_DATA);
    LPCTSTR GetStatusCustom(void) const;

    int BuildString(void);
    CString GetString(void);

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
    DEVMODE* m_devDevmode;           // 0x09
    int m_nStatus;                   // 0x0A
    CString m_strStatusAsText;
    CString m_strStatusString;       // 0x0B
    CString m_strSecurityDescriptor; // 0x0C
    CString m_strDocument;           // 0x0D
    int m_nPriority;                 // 0x0E
    int m_nPosition;                 // 0x0F
    SYSTEMTIME m_timeSubmitted;      // 0x10
    CString m_strSubmitted;
    int m_nStartTime;                // 0x11
    int m_nUntilTime;                // 0x12
    int m_nTime;                     // 0x13
    int m_nTotalPages;               // 0x14
    int m_nPagesPrinted;             // 0x15
    int m_nTotalBytes;               // 0x16
    int m_nBytesPrinted;             // 0x17
    
    CString m_strAllProperties;

};

