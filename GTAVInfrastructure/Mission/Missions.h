#pragma once
class fwMission {
public:
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Cleanup() = 0; 
	virtual int GetMissionStatus() { return m_iMissionStatus; }
protected:
	int m_iMissionStatus;
};