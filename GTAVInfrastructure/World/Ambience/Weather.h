#pragma once
class CWeather {
public:
	static constexpr int MAX_WEATHER_NAME = 32;
	const char* GetWeather() { return this->m_WeatherState; }
	void SetWeather(const char* weather);
private:
	char m_WeatherState[MAX_WEATHER_NAME] = { "" };
};
#include <map>
class CWeatherMgr {
public:
	CWeatherMgr();
	void SetWeather(const CWeather& weather) { this->m_pWeather = weather; }
	void SetComboWeather(CWeather& weather) {
		if (m_pComboWeather == nullptr) return; 
		*this->m_pComboWeather = weather; // copy
	}
	CWeather& GetWeather() {
		return this->m_pWeather;
	}
	CWeather* GetComboWeather() {
		return this->m_pComboWeather;
	}
	void SetMixPercentage(float fMix) { this->m_fMixPercentage = fMix; }
	float GetMixPercentage() { return this->m_fMixPercentage; }
	const char* GetWeatherFromHash(unsigned long hsh);

private:
	std::map<unsigned long, const char*> WeatherTypes;
	CWeather m_pWeather, *m_pComboWeather;
	float m_fMixPercentage = 0.0f;
};