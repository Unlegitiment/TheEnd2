#pragma once
#include "Water.h"
#include "WorldState.h"
#include "Ambience\GameAmbience.h"
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
	static void Destroy();
	~CGameWorld() { Destroy(); }
private:
	static void InitWorldState();
	inline static CWaterMgr* sm_pWater = nullptr;
	inline static CWorldState* sm_pWorldState = nullptr;
	inline static CGameAmbience* sm_pAmbience = nullptr;
};