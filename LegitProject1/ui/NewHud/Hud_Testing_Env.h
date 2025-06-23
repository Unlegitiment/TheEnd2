#pragma once
#include "LegitProject1\ui\Scaleform\ScaleformMgr.h"
#include <TheEnd2\keyboard.h>
enum eWeirdHudComponents :int {
	NEW_HUD,
	WANTED_STARS,
	WEAPON_ICON,
	CASH,
	MP_CASH,
	MP_MESSAGE,
	VEHICLE_NAME,
	AREA_NAME,
	STREET_NAME,
	HELP_TEXT,
	FLOATING_HELP_TEXT_1,
	FLOATING_HELP_TEXT_2,
	CASH_CHANGE,
	RETICLE,
	SUBTITLE_TEXT,
	RADIO_STATIONS,
	SAVING_GAME,
	GAME_STREAM,
	WEAPON_WHEEL,
	WEAPON_WHEEL_STATS,
};
class CNewHudTest {
public:
	void Init() {
		//scaleform = CScaleformManager::Get()->GetScaleform("HUD");
	}
	void Update() {
		//logf("Fuck you!");
		if (IsKeyJustUp(VK_F15)) {
			bRequiresHudRestart = !bRequiresHudRestart;
			logf("Requires Restart Flipped: %d", bRequiresHudRestart);
		}
		if (bRequiresHudRestart) {
			
		}
	}
	void Shutdown() {
		CScaleformManager::Get()->Release("HUD");
	}
public:
	bool bRequiresHudRestart = true;
	CScaleform* scaleform = nullptr;
};