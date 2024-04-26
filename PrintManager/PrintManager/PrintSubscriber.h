#include "MapEx.h"
#include "JobInfo.h"
#include "ThreadInfo.h"

#pragma once
class PrintSubscriber
{
public:
	// Function declarations
	UINT Worker();
	PrintSubscriber();   // standard constructor
	virtual ~PrintSubscriber();
	CEvent* m_pEventThreadDone;
	CEvent* m_pEventStopRequested;
	CThreadInfo m_ThreadInfo;
	CMapEx<int, int, CJobInfo*, CJobInfo*> m_mapJobInfo;

};
