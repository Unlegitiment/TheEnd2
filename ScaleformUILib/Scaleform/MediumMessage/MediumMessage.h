#pragma once
#include <ScaleformUILib\Scaleform\ScaleformExtensions.h>
#include "..\common.h"
class MediumMesageHandler {
private:
	ScaleformWideScreen* _sc = nullptr;
	int _start;
	int _timer;
	bool _hasAnimatedOut;
	HudColour outColor = HudColour::HUD_COLOUR_FREEMODE;
	float _animOutTime = 0.33f;
public:
	MediumMesageHandler() {}
	void Load()
	{
		if (_sc != nullptr) return;
		_sc = new ScaleformWideScreen("MIDSIZED_MESSAGE");

		int timeout = 1000;
		int start = MISC::GET_GAME_TIMER();
		while (!_sc->IsLoaded() && MISC::GET_GAME_TIMER()) - start < timeout) WAIT(0);
	}

	void Dispose()
	{
		_sc->Dispose();
		delete _sc;
		_sc = nullptr;
	}

	void ShowColoredShard(string msg, string desc, HudColour bgColor, HudColour outColor = HudColour::HUD_COLOUR_FREEMODE, bool useDarkerShard = false, bool useCondensedShard = false, int time = 5000, float animTime = 0.33f)
	{
		Load();
		_start = MISC::GET_GAME_TIMER();
		_sc->CallFunction("SHOW_SHARD_MIDSIZED_MESSAGE", msg, desc, (int)bgColor, useDarkerShard, useCondensedShard);
		_timer = time;
		this->outColor = outColor;
		_animOutTime = animTime;
		_hasAnimatedOut = false;
	}
private:
	void Update()
	{
		_sc->Render2D();
		if (_start != 0 && MISC::GET_GAME_TIMER()) - _start > _timer)
		{
			if (!_hasAnimatedOut)
			{
				_sc->CallFunction("SHARD_ANIM_OUT", (int)outColor, _animOutTime);
				_hasAnimatedOut = true;
				_timer += 2000;
			}
		}
		else
		{
			if (_hasAnimatedOut && MISC::GET_GAME_TIMER() - _start > (_timer + 2000))
			{
				AUDIO::PLAY_SOUND_FRONTEND(-1, "Shard_Disappear", "GTAO_FM_Events_Soundset",false);
				_start = 0;
				Dispose();
			}
		}
	}
};
