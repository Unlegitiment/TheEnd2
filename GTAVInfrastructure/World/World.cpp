#include "World.h"
#include "GTAVInfrastructure\Objects\Ped.h"
void CGameWorld::Init()
{
    sm_pWater = new CWaterMgr();
    sm_pWorldState = new CWorldState();
    sm_pAmbience = new CGameAmbience();
    sm_pStreaming = new CStreamingMgr();
    InitWorldState();
}

void CGameWorld::Update()
{
    sm_pAmbience->Update(); // Allows time to be fixed.
}

void CGameWorld::SetWorldState(bool Cayo)
{
    if (Cayo) {
        sm_pWorldState->SetWorldState(CAYO);
        sm_pWater->SetWaterState(CAYO);
    }
    else {
        sm_pWorldState->SetWorldState(LOS_SANTOS);
        sm_pWater->SetWaterState(LOS_SANTOS);
    }
}

int CGameWorld::GetWorldState()
{
    return sm_pWorldState->GetState();
}

void CGameWorld::SetWater(int wtrFile)
{
    sm_pWater->SetWaterState(wtrFile);
}

int CGameWorld::GetWaterFile()
{
    return sm_pWater->GetWaterState();
}

void CGameWorld::Destroy()
{
    delete sm_pWater;
    delete sm_pWorldState;
    delete sm_pAmbience;
    delete sm_pStreaming;
}

void CGameWorld::InitWorldState()
{
    if (GetWaterFile() == CAYO) {
        sm_pWorldState->SetWorldState(CAYO);
        sm_pWater->SetWaterState(CAYO);
    }
}
