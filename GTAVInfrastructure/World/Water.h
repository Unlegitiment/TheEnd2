#pragma once
class CWaterMgr {
public:
	CWaterMgr();
	void SetWaterState(int mState);
	int GetWaterState() {
		return static_cast<int>(m_CurrentWater.mState);
	}
	const char* GetWaterStateStr() { return GetWaterState() ? "LOS_SANTOS" : "CAYO_PERICO"; }
private:
	struct WaterState {
		enum WaterFile{
			WATER_STATE_LS,
			WATER_STATE_CAYO,
		}mState;
	}m_CurrentWater;
};