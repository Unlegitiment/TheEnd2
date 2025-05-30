#pragma once
#include "..\common.h"
#include <ScaleformUILib\Scaleform\ScaleformExtensions.h>
#include <string>

class CountdownHandler {
public:
	CountdownHandler() {}
	void Start(int number = 3,
		HudColour hudColor = HudColour::HUD_COLOUR_GREEN,
		string countdownAudioName = "321",
		string countdownAudioRef = "Car_Club_Races_Pursuit_Series_Sounds",
		string goAudioName = "Go",
		string goAudioRef = "Car_Club_Races_Pursuit_Series_Sounds") {
		Load();
		int r = 255, g = 255, b = 255, a = 255;
		HUD::GET_HUD_COLOUR(hudColor, &r, &g, &b, &a);
		int gameTime = MISC::GET_GAME_TIMER();
		while (number >= 0) {
			WAIT(0);
			if ((NETWORK::GET_NETWORK_TIME_ACCURATE() - gameTime) > 1000) { // @Warn: This uses NETWORK natives for SinglePlayer activity. Check if Okay.
				AUDIO::PLAY_SOUND_FRONTEND(-1, countdownAudioName, countdownAudioRef, true);
				gameTime = MISC::GET_GAME_TIMER();
				ShowMessage(number, r, g, b);
				number--;
			}
		}
		AUDIO::PLAY_SOUND_FRONTEND(-1, goAudioName, goAudioRef, true);
		ShowMessage("CNTDWN_GO", r, g, b);
		Dispose();
	}


private:
	void Load() {
		if (_sc != nullptr) return;
		AUDIO::REQUEST_SCRIPT_AUDIO_BANK("HUD_321_GO", false, false);
		_sc = new ScaleformWideScreen(SCALEFORM_NAME);
		int timeout = 1000;
		int start = MISC::GET_GAME_TIMER();
		while (!_sc->IsLoaded() && MISC::GET_GAME_TIMER() - start < timeout) WAIT(0);
	}
	void Dispose() {
		int gameTime = MISC::GET_GAME_TIMER();
		while (MISC::GET_GAME_TIMER() - gameTime < 1000) WAIT(0);
		_sc->Dispose();
		delete _sc;
		_sc = nullptr;
	}
	void ShowMessage(int number, int r = 255, int g = 255, int b = 255) {
		ShowMessage(std::to_string(number).c_str(), r,g,b);
	}
	void ShowMessage(string message, int r = 255, int g = 255, int b = 255)
	{
		_sc->CallFunction("SET_MESSAGE", message, r, g, b, true);
		_sc->CallFunction("FADE_MP", message, r, g, b);
	}
	void DisplayCountdown() {
		while (_sc != nullptr && _sc->IsLoaded()){}
		{
			_sc->Render2D();
			WAIT(0);
		}
	}
	string SCALEFORM_NAME = "COUNTDOWN";
	ScaleformWideScreen* _sc = nullptr;
};