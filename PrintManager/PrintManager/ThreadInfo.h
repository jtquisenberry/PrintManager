#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CThreadInfo
{
    HANDLE m_hPrinter;
    HANDLE m_hEventStopRequested;
    HANDLE m_hEventThreadDone;
    HWND m_hWnd;

public:
    CThreadInfo()
    {
        m_hPrinter            = INVALID_HANDLE_VALUE;
        m_hEventStopRequested = INVALID_HANDLE_VALUE;
        m_hEventThreadDone    = INVALID_HANDLE_VALUE;
        m_hWnd                = NULL;
    }

    //================================================================

    HANDLE GetPrinter( void ) const
    {
        return m_hPrinter;
    }

    HANDLE GetStopRequestedEvent( void ) const
    {
        return m_hEventStopRequested;
    }

    HANDLE GetThreadDoneEvent( void ) const
    {
        return m_hEventThreadDone;
    }

    HWND GetHwnd( void ) const
    {
        return m_hWnd;
    }

    //================================================================

    void SetPrinter( const HANDLE hPrinter )
    {
        m_hPrinter = hPrinter;
    }

    void SetStopRequestedEvent( const HANDLE hEventStopRequested )
    {
        m_hEventStopRequested = hEventStopRequested;
    }

    void SetThreadDoneEvent( const HANDLE hEventThreadDone )
    {
        m_hEventThreadDone = hEventThreadDone;
    }

    void SetHwnd( const HWND hWnd )
    {
        m_hWnd = hWnd;
    }
};

