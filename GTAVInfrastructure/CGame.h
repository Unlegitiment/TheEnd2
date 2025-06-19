#pragma once
#include "Handlers.h"
#include "Hud\HudHandler.h"
#include "GameScripts.h"
#include "Cheats\CheatMgr.h"
#include "World\World.h"
#include "ScriptEnv\ScriptProc.h"
#include "Camera\Camera.h"
struct DeathState { // All for now but its fine.
	int PLAYER_PED_ID;
};
class CGame {
public:
	static void Init();
	static CEntityHandler& GetEntityHandler() { return *sm_pEntityHandler; };
	static CGameWorld& GetWorld() { return *sm_pWorld; }
	static CWaterMgr& GetWater() { return *sm_pWorld->GetWtrMgr(); }
	static CHudHandler& GetHudHandler() { return *sm_pHudHandler; }
	static CScaleformHandler& GetScaleformHandler() { return *sm_pHudHandler->GetScaleformHandler(); }
	static CGameScriptMgr& GetScriptMgr() { return *sm_pGameScriptMgr; }
	static CPedHeadshotMgr& GetHeadshotMgr() { return sm_pEntityHandler->GetHeadshotMgr(); }
	static CCheatMgr& GetCheatMgr() { return *sm_pCheats; }
	static CPed* GetLocalPlayer() { return GetEntityHandler().GetLocalPlayer(); }
	static CCameraManager& GetCameraMgr() { return *sm_pCameraManager; }
	static void Update();
	static void Destroy();
private:
	static void DeathCheck();
	static void LoadCheats();
	inline static CEntityHandler* sm_pEntityHandler = nullptr;
	inline static CGameWorld* sm_pWorld = nullptr;
	inline static CHudHandler* sm_pHudHandler = nullptr;
	inline static CGameScriptMgr* sm_pGameScriptMgr = nullptr;
	inline static CCheatMgr* sm_pCheats = nullptr;
	inline static CCameraManager* sm_pCameraManager = nullptr;
private:
	using DeathScript = std::function<void(const DeathState*)>;
	inline static DeathScript sm_pfnDeathScript = nullptr;
	static void DefaultDeathHandler(const DeathState* pDeathState);
public:
	static void SetDeathHandler(DeathScript pFn);
};