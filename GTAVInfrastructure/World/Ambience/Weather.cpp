#include "Weather.h"
#include <string>
void CWeather::SetWeather(const char* weather)
{
	strncpy_s(this->m_WeatherState, weather, MAX_WEATHER_NAME);
	
}

CWeatherMgr::CWeatherMgr()
{
	this->WeatherTypes.insert({ 2544503417, "EXTRASUNNY" });
	this->WeatherTypes.insert({ 3496890190, "CLEAR " });
	this->WeatherTypes.insert({ 3021313653, "CLOUDS " });
	this->WeatherTypes.insert({ 1875423685, "SMOG " });
	this->WeatherTypes.insert({ 1510666486, "FOGGY " });
	this->WeatherTypes.insert({ 3042581451, "OVERCAST " });
	this->WeatherTypes.insert({ 669135361, "RAIN " });
	this->WeatherTypes.insert({ 2939984370, "THUNDER " });
	this->WeatherTypes.insert({ 3929078362, "CLEARING " });
	this->WeatherTypes.insert({ 374931295, "NEUTRAL " });
	this->WeatherTypes.insert({ 3788945765, "SNOW " });
	this->WeatherTypes.insert({ 4231556963, "BLIZZARD " });
	this->WeatherTypes.insert({ 3228026158, "SNOWLIGHT " });
	this->WeatherTypes.insert({ 4086529054, "XMAS " });
	this->WeatherTypes.insert({ 3606517134, "HALLOWEEN " });
	this->WeatherTypes.insert({ 385726482, "RAIN_HALLOWEEN " });
	this->WeatherTypes.insert({ 2144126041,"SNOW_HALLOWEEN " });
	this->m_pComboWeather = new CWeather(); 
}

const char* CWeatherMgr::GetWeatherFromHash(unsigned long hsh)
{
	return this->WeatherTypes.find(hsh)->second;
}
