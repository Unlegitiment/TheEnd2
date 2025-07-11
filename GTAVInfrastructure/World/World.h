#pragma once
#include "Water.h"
#include "WorldState.h"
#include "Ambience\GameAmbience.h"
#include "GTAVInfrastructure\streaming\streamingMgr.h"
class CGameWorld {
public:
	CGameWorld() { Init(); }
	static void Init();
	static void Update();
	static void SetWorldState(bool Cayo);
	static int GetWorldState(); 
	static void SetWater(int wtrFile);
	static int GetWaterFile();
	static CWaterMgr* GetWtrMgr() { return sm_pWater; }
	static CGameAmbience* GetAmbience() { return sm_pAmbience; }
	static CStreamingMgr* GetStreamingMgr() { return sm_pStreaming; }
	static CWorldState* GetWorldStateMgr() { return sm_pWorldState; }
	static void Destroy();
	~CGameWorld() { Destroy(); }
private:
	static void InitWorldState();
	inline static CWaterMgr* sm_pWater = nullptr;
	inline static CWorldState* sm_pWorldState = nullptr;
	inline static CGameAmbience* sm_pAmbience = nullptr;
	inline static CStreamingMgr* sm_pStreaming = nullptr;
};