#include <Windows.h>
#include "Tool.h"

static double mvFreq;
static double mvBaseTime;

class WinMvTimeStarter
{
public:
	WinMvTimeStarter()
	{
		LARGE_INTEGER c, f;
		QueryPerformanceFrequency(&f);
		mvFreq = (double)f.QuadPart / 1000000;
		QueryPerformanceCounter(&c);
		mvBaseTime = (double)c.QuadPart / mvFreq;
	}
}mv_starter;

CTool::CTool()
{
	m_mutexHandle = CreateMutex(NULL, false, NULL);
}

CTool::~CTool()
{
	if (m_mutexHandle)
	{
		CloseHandle(m_mutexHandle);
		m_mutexHandle = NULL;
	}
}

bool CTool::enter()
{
	if (m_mutexHandle)
	{
		return (WaitForSingleObject(m_mutexHandle, INFINITE) == WAIT_OBJECT_0);
	}

	return false;
}

bool CTool::tryEnter()
{
	if (m_mutexHandle)
	{
		return (WaitForSingleObject(m_mutexHandle, 0) == WAIT_OBJECT_0);
	}

	return false;
}

bool CTool::leave()
{
	if (m_mutexHandle)
	{
		return (ReleaseMutex(m_mutexHandle) != FALSE);
	}

	return false;
}

uint64_t CTool::getCurTimeMilliSecond()
{
	static uint64_t s_lastMillSeconds = 0;
	LARGE_INTEGER c;
	QueryPerformanceCounter(&c);
	int64_t tmp = (int64_t)((((double)c.QuadPart / mvFreq) - mvBaseTime) / 1000);
	s_lastMillSeconds = (uint64_t)(tmp > (int64_t)s_lastMillSeconds ? tmp : s_lastMillSeconds);

	return s_lastMillSeconds;
}

uint64_t CTool::getCurTimeMicroSecond()
{
	LARGE_INTEGER c;
	QueryPerformanceCounter(&c);
	return (int64_t)(((double)c.QuadPart / mvFreq) - mvBaseTime);
}
