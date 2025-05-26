#pragma once
struct sScript {
public:
	char ScriptName[32];
	int id;
private:

};
#include <vector>
enum eStackSize : int{
    MICRO = 128,
    MINI = 512,
    DEFAULT = 1424,
    SPECIAL_ABILITY = 1828,
    FRIEND = 2050,
    SHOP = 2324,
    CELLPHONE = 2552,
    VEHICLE_SPAWN = 3568,
    CAR_MOD_SHOP = 3750,
    PAUSE_MENU_SCRIPT = 3076,
    APP_INTERNET = 4592,
    MULTIPLAYER_MISSION = 5050,
    CONTACTS_APP = 4000,
    INTERACTION_MENU = 9800,
    SCRIPT_XML = 8344,
    PROPERTY_INT = 19400,
    ACTIVITY_CREATOR_INT = 15900,
    SMPL_INTERIOR = 2512,
    WAREHOUSE = 14100,
    IE_DELIVERY = 2324,
    SHOP_CONTROLLER = 3800,
    AM_MP_YACHT = 5000,
    INGAMEHUD = 4600,
    TRANSITION = 8032,
    FMMC_LAUNCHER = 27000,
    MULTIPLAYER_FREEMODE = 85000,
    MISSION = 62500,
    MP_LAUNCH_SCRIPT = 34750
};
class CGameScriptMgr {
public:
	void LoadScripts();
    void LoadGameScripts(); // no shv
	sScript* LoadScript(const char* scrName, int stackSize);
	sScript* FindScript(const char* str);
	void TerminateAllNonSHVThreads();
	void UnloadScript(int id);
	void UnloadScript(sScript* sc);
	void UnloadScript(const char* scrName);
	void Destroy();
	~CGameScriptMgr() { Destroy(); }
private:
	std::vector<sScript*> m_Scripts;
};