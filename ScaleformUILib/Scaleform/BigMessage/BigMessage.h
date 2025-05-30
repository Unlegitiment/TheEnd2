#pragma once
#include "ScaleformUILib\Scaleform\common.h"
#include <ScaleformUILib\Scaleform\ScaleformExtensions.h>
/*
@ Todo: Simulate internal behavior via friend-classes. Have to see where it goes if there is a pool of Scaleforms or not either way its whatever. 

*/

class BigMessageHandler {
private:
	friend class CTest;
	ScaleformWideScreen* _sc = nullptr;
	int _start;
	int _duration;
	bool _transitionExecuted = false;
public:
	bool ManualDispose;
	string Transition = "TRANSITION_OUT";
	float TransitionDuration = 0.4f;
	bool TransitionAutoExpansion = false;
	BigMessageHandler() {}
	void Load() {
		if (_sc != nullptr) return;
		_sc = new ScaleformWideScreen("MP_BIG_MESSAGE_FREEMODE");
		int timeout = 1000;
		int start = MISC::GET_GAME_TIMER();
		while (!_sc->IsLoaded() && MISC::GET_GAME_TIMER() - start < timeout)  WAIT(0);
	}
	void Dispose() {
		if (_sc == nullptr) return;
		if (ManualDispose) {
			_sc->CallFunction(Transition, TransitionDuration, TransitionAutoExpansion);
			WAIT((DWORD)((TransitionDuration * .5f) * 1000));
			ManualDispose = false;
		}
		_transitionExecuted = false;
		_start = 0;
		_sc->Dispose();
		delete _sc;
		_sc = nullptr;
	}
	void ShowMissionPassed(string msg, int time = 5000, bool manualDispose = false) {
		Load();
		_start = MISC::GET_GAME_TIMER();
		ManualDispose = manualDispose;
		_sc->CallFunction("SHOW_MISSION_PASSED_MESSAGE", msg, "", 100, true, 0, true);
		_duration = time;
	}
	void ShowColoredShard(string msg, string desc, HudColour textColor, HudColour bgColor, int time = 5000, bool manualDispose = false) {
		Load();
		_start = MISC::GET_GAME_TIMER();
		_sc->CallFunction("SHOW_SHARD_CENTERED_MP_MESSAGE");
		_duration = time;
	}
	void ShowOldMessage(string msg, int time = 5000, bool manualDispose = false) {
		Load();
		_start = MISC::GET_GAME_TIMER();
		ManualDispose = manualDispose;
		_sc->CallFunction("SHOW_MISSION_PASSED_MESSAGE", msg);
		_duration = time;
	}
	void ShowSimpleShard(string title, string subtitle, int time = 5000, bool manualDispose = false) {
		Load();
		_start = MISC::GET_GAME_TIMER();
		ManualDispose = manualDispose;
		_sc->CallFunction("SHOW_SHARD_CREW_RANKUP_MP_MESSAGE", title, subtitle);
		_duration = time;
	}
	void ShowRankupMessage(string msg, string subtitle, int rank, int time = 5000, bool manualDispose = false)
	{
		Load();
		_start = MISC::GET_GAME_TIMER();
		ManualDispose = manualDispose;
		_sc->CallFunction("SHOW_BIG_MP_MESSAGE", msg, subtitle, rank, "", "");
		_duration = time;
	}
	typedef int WeaponHash;
	void ShowWeaponPurchasedMessage(string bigMessage, string weaponName, WeaponHash weapon, int time = 5000, bool manualDispose = false)
	{
		Load();
		_start = MISC::GET_GAME_TIMER();
		ManualDispose = manualDispose;
		_sc->CallFunction("SHOW_WEAPON_PURCHASED", bigMessage, weaponName, weapon, "", 100);
		_duration = time;
	}
	void ShowMpMessageLarge(string msg, int time = 5000, bool manualDispose = false)
	{
		Load();
		_start = MISC::GET_GAME_TIMER();
		ManualDispose = manualDispose;
		_sc->CallFunction("SHOW_CENTERED_MP_MESSAGE_LARGE", msg, "test", 100, true, 100);
		_sc->CallFunction("TRANSITION_IN");
		_duration = time;
	}
	void ShowMpWastedMessage(string msg, string sub, int time = 5000, bool manualDispose = false)
	{
		Load();
		_start = MISC::GET_GAME_TIMER();
		ManualDispose = manualDispose;
		_sc->CallFunction("SHOW_SHARD_WASTED_MP_MESSAGE", msg, sub);
		_duration = time;
	}
	template<typename... Args>
	void ShowCustomShard(string funcName, Args&&... paremeters)
	{
		Load();
		_sc->CallFunction(funcName, paremeters);
	}

private:
	void Update()
	{
		LAGInterface::Writeln(__FUNCTION__ " Called ! 0x%p",_sc);
		return;
		if (_sc == nullptr) {
			LAGInterface::Writeln("Failed due to _sc being null!");
			return;
		}
		_sc->Render2D();
		LAGInterface::Writeln("After sc call");

		if (ManualDispose) return;

		if (_start != 0 && (MISC::GET_GAME_TIMER() - _start) > _duration)
		{
			if (!_transitionExecuted)
			{
				_sc->CallFunction(Transition, TransitionDuration, TransitionAutoExpansion);
				_transitionExecuted = true;
				_duration += (int)((TransitionDuration * .5f) * 1000);
			}
			else
			{
				Dispose();
			}
		}
	}

};