#include "Time.h"

void CWorldTimeManager::SetTime(CTime time)
{
    this->m_Date.SetTime(time);
}

void CWorldTimeManager::SetWeekday(eWeekday wkd)
{
    this->m_Date.SetWeekDay(wkd);
}

CTime& CWorldTimeManager::GetTime()
{
    return this->m_Date.GetTime();
}

CDate& CWorldTimeManager::GetDate()
{
    return this->m_Date;
}
