#include "GameAmbience.h"
#include "GTAVInfrastructure\SHV\natives.h"
void CGameAmbience::Init()
{
	CTimeCycle cycle = GetTimecycleFromGame();
	this->m_TimeCycleMgr = new CTimeCycleMgr(&cycle); // this copies
	this->m_TimeMgr = new CWorldTimeManager();
	this->m_WeatherMgr = new CWeatherMgr();
}
void CGameAmbience::Update()
{
	this->SetTime(GetTimeFromGame());
}
void CGameAmbience::SetTime(CTime time)
{
	this->m_TimeMgr->SetTime(time);
	CLOCK::SET_CLOCK_TIME(time.GetHour(), time.GetMinute(), time.GetSecond());
}

CTime CGameAmbience::GetTime()
{
	return this->m_TimeMgr->GetDate().GetTime();
}

void CGameAmbience::SetWeatherState(CWeather State)
{
	this->m_WeatherMgr->SetWeather(State);
	MISC::SET_WEATHER_TYPE_NOW(State.GetWeather());
}

void CGameAmbience::SetMixedState(CWeather State1, CWeather State2, float fWeatherPercentage)
{
	this->m_WeatherMgr->SetWeather(State1);
	this->m_WeatherMgr->SetComboWeather(State2); // figure out this hmm? 
	this->m_WeatherMgr->SetMixPercentage(fWeatherPercentage);
	MISC::SET_CURR_WEATHER_STATE(MISC::GET_HASH_KEY(State1.GetWeather()), MISC::GET_HASH_KEY(State2.GetWeather()), fWeatherPercentage);
}

void CGameAmbience::GetState(CWeather* pOut1, CWeather* pOut2, float* pOutPercent)
{
	GetWeatherFromGame(pOut1, pOut2, pOutPercent);
	return;
}

CTime CGameAmbience::GetTimeFromGame()
{
	CTime time = CTime();
	time.SetHour(CLOCK::GET_CLOCK_HOURS());
	time.SetMinute(CLOCK::GET_CLOCK_MINUTES());
	time.SetSecond(CLOCK::GET_CLOCK_SECONDS());
	return time;
}

CTimeCycle CGameAmbience::GetTimecycleFromGame()
{
	GRAPHICS::CLEAR_TIMECYCLE_MODIFIER();
	return CTimeCycle("default");
}

void CGameAmbience::GetWeatherFromGame(CWeather* pOut, CWeather* pOut2, float* mix)
{
	Hash baseWthr = 0, mixedWeather = 0;
	float mixedPercent = 0.f;
	MISC::GET_CURR_WEATHER_STATE(&baseWthr, &mixedWeather, &mixedPercent);
	CWeather weather = CWeather();
	weather.SetWeather(m_WeatherMgr->GetWeatherFromHash(baseWthr));
	*pOut = weather;
	weather.SetWeather(m_WeatherMgr->GetWeatherFromHash(mixedWeather));
	*pOut2 = weather;
	*mix = mixedPercent;
}
