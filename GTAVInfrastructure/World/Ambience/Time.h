#pragma once
class CTime {
public:
	int GetHour() { return this->hour; }
	int GetMinute() { return this->minute; }
	int GetSecond() { return this->second; }
	void SetHour(int hrs) { hour = hrs; }
	void SetMinute(int mins) { minute = mins; }
	void SetSecond(int seconds) { second = seconds; }
private:
	int hour, minute, second;
};

enum eWeekday {
	SUNDAY,
	MONDAY,
	TUESDAY,
	WEDNESDAY,
	THURSDAY,
	FRIDAY,
	SATURDAY
};
class CDate {
public:
	eWeekday GetWeekDay() {
		return m_DoTW;
	}
	void SetWeekDay(eWeekday wk) {
		this->m_DoTW = wk;
	}
	void SetTime(CTime time) {
		this->m_Time = time;
	}
	CTime& GetTime() { return this->m_Time; }
private:
	CTime m_Time;
	eWeekday m_DoTW;
};
class CWorldTimeManager {
public:
	void SetTime(CTime time);
	void SetWeekday(eWeekday wkd);
	CTime& GetTime();
	CDate& GetDate();
private:
	CDate m_Date;
};