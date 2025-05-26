#pragma once
#include "Time.h"
#include "Timecycle.h"
#include "Weather.h"
#include <map>
class CGameAmbience {
public:
	CGameAmbience() { Init(); }
	void Init();
	void Update();
	void SetTime(CTime time);
	CTime GetTime();
	void SetWeatherState(CWeather State);
	void SetMixedState(CWeather State1, CWeather State2, float fWeatherPercentage);
	void GetState(CWeather* pOut1, CWeather* pOut2, float* pOutPercent);
private:
	CTime GetTimeFromGame();
	CTimeCycle GetTimecycleFromGame();
	void GetWeatherFromGame(CWeather* pOut, CWeather* pOut2, float* mix);

	CTimeCycleMgr* m_TimeCycleMgr = nullptr;
	CWorldTimeManager* m_TimeMgr = nullptr;
	CWeatherMgr* m_WeatherMgr = nullptr;
};