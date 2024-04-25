#pragma once
class PrintSubscriber
{
public:
	// Function declarations
	UINT Worker();
	PrintSubscriber();   // standard constructor
	virtual ~PrintSubscriber();
	CThreadInfo m_ThreadInfo;
	CMapEx<int, int, CJobInfo*, CJobInfo*> m_mapJobInfo;

};
