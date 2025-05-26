#pragma once
class CTimeCycle {
public:
	static constexpr int MAX_TIME_CYCLE = 64;
	CTimeCycle(const char* timeCycle);
	void SetIntensity(float intensity) { this->m_fIntensity = intensity; }
	float GetIntensity() const { return m_fIntensity; }
	char* GetName() { return m_aTimeCycle; }
private:
	char m_aTimeCycle[MAX_TIME_CYCLE];
	float m_fIntensity;
};
#include "GTAVInfrastructure\SHV\natives.h"
class CTimeCycleMgr {
public:
	CTimeCycleMgr(CTimeCycle* curTimeCycle) { m_CurrentCycle = new CTimeCycle(""); *m_CurrentCycle = *curTimeCycle; } // Add assert that curTimeCycle != nullptr;
	void SetTimeCycle(const CTimeCycle& timeCycle) {
		if (m_CurrentCycle != nullptr) delete m_CurrentCycle; // why do this instruction. @TODO: Vibe check allocation stuff
		m_CurrentCycle = new CTimeCycle(timeCycle);
	}
	void SetStrength(float fStrength) {
		if (this->m_CurrentCycle == nullptr) return;
		this->m_fStrength = fStrength;
	}
	void DoTransition(CTimeCycle* nextCycle, float fTransition) { // im gonna assume thats like the progression of the timecycle?
		SetTimeCycle(*nextCycle);
	}
	void ClearTC() {
		if (m_CurrentCycle != nullptr) { delete m_CurrentCycle; }
		this->m_fStrength = 0.0f;
	}
private:
	CTimeCycle* m_CurrentCycle = nullptr;
	float m_fStrength = 1.f;
};