#include "Water.h"
#include "GTAVInfrastructure\SHV\natives.h"
#include "LegitProject1\LInfras.h"
CWaterMgr::CWaterMgr()
{
	this->m_CurrentWater.mState = (WaterState::WaterFile)STREAMING::GET_GLOBAL_WATER_FILE();
	LAGInterface::Write("Acquired Water LVL: %s\n", GetWaterStateStr());
}

void CWaterMgr::SetWaterState(int mState)
{
	this->m_CurrentWater.mState = (WaterState::WaterFile)mState;
	STREAMING::LOAD_GLOBAL_WATER_FILE(mState);
}
