#pragma once
#include "Player.h"
#include "Markers.h"
#include "Triggers.h"
class CEndWorld {
public:
	static void Init() {
		sm_pMarkerMgr = new CMarkerMgr();
		sm_pTriggers = new CTriggers();
	}
	static CMarkerMgr* GetMarkerMgr() { return sm_pMarkerMgr; }
	static CTriggers* GetTriggers() { return sm_pTriggers; }
	static void DoTimeCycleModification();
	static void Update() {
		if (sm_pTriggers == nullptr || sm_pMarkerMgr == nullptr) { return; }
		sm_pMarkerMgr->Update();
		sm_pTriggers->Update();
	} 
	//static CPlayer* GetPlayer() { return &sm_pPlayer; }
	static void Destroy() {
		delete sm_pTriggers;
		sm_pTriggers = nullptr;
		LAGInterface::Writeln("Sm_pTriggers deleted and now equals %p", sm_pTriggers);
		delete sm_pMarkerMgr;
		sm_pMarkerMgr = nullptr;
		LAGInterface::Writeln("sm_pMarkerMgr deleted and now equals %p", sm_pMarkerMgr);
		LAGInterface::Writeln(__FUNCTION__"End Of Method");
	}
private:
	//static CPlayer sm_pPlayer;
	static CMarkerMgr* sm_pMarkerMgr;
	static CTriggers* sm_pTriggers;
};