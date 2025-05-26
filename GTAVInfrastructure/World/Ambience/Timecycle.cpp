#include "Timecycle.h"
#include <string>
CTimeCycle::CTimeCycle(const char* timeCycle) : m_aTimeCycle("")
{
	strncpy_s(m_aTimeCycle, timeCycle, MAX_TIME_CYCLE);
}
