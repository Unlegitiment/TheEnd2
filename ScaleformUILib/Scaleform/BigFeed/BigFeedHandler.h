#pragma once
#include "ScaleformUILib/Scaleform/ScaleformExtensions.h"
class BigFeedHandler {
public:
	friend class CTest;
	BigFeedHandler() : _sc(nullptr) {
		Load();
	}
	bool RightAligned;
	bool DisabledNotifications;
	bool IsEnabled() { return this->enabled; }
	void SetEnabled(bool value) {
		this->enabled = value;
		if (value) {
			_sc->CallFunction("SETUP_BIGFEED", RightAligned);
			_sc->CallFunction("HIDE_ONLINE_LOGO");
			_sc->CallFunction("FADE_IN_BIGFEED");
			if (DisabledNotifications)
				HUD::THEFEED_AUTO_POST_GAMETIPS_OFF(); // Same native signature as ThefeedCommentTeleportPoolOff
			UpdateInfo();
		}
		else {
			_sc->CallFunction("END_BIGFEED");
			if (DisabledNotifications) HUD::THEFEED_AUTO_POST_GAMETIPS_OFF();
		}
	}
	const char* GetTitle() { return this->title; }
	void SetTitle(const char* value) {
		strncpy_s(this->title, value, sizeof title);
		if (enabled) {
			UpdateInfo();
		}
	}
	const char* GetSubtitle() { return this->subtitle; }
	const char* GetTextureDictionary() { return this->textureDictionary; }
	const char* GetTextureName() { return this->textureName; }
	void SetSubtitle(const char* value) { 
		strncpy_s(this->subtitle, value, sizeof subtitle);


		if (enabled) {
			UpdateInfo();
		}
	}
	void SetBodyText(const char* value) {
		strncpy_s(this->bodyText, value, sizeof bodyText);
		if (enabled) {
			UpdateInfo();
		}
	}
	void UpdatePicture(const char* txd, const char* txn) {
		strncpy_s(this->textureDictionary, txd, sizeof textureDictionary);
		strncpy_s(this->textureName, txn, sizeof textureName);
		if (!enabled) { return; }
		_sc->CallFunction("SET_BIGFEED_IMAGE", txd, txn);
	}

private:
	void UpdateInfo() {
		if (!enabled) return;
		//AddTextEntry("scaleform_ui_bigfeed", bodyText); -- Not a valid call in base Game. No CFX natives :(
		int handle = _sc->GetHandle();
		GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(handle, "SET_BIGFEED_INFO");
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING("");
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING(this->bodyText);
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(0);
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_TEXTURE_NAME_STRING(textureDictionary);
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_TEXTURE_NAME_STRING(textureName);
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING(subtitle);
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING("");
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING(title);
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING(0);
		GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
	}
	
	void Load() {
		if (_sc != nullptr) return;
		_sc = new ScaleformWideScreen("GTAV_ONLINE");
		int timeout = 1000;
		int start = MISC::GET_GAME_TIMER();
		while (!_sc->IsLoaded() && MISC::GET_GAME_TIMER() - start < timeout) WAIT(0);
		_sc->CallFunction("HIDE_ONLINE_LOGO");
	}
	void Dispose() {
		_sc->Dispose();
		delete _sc;
		_sc = nullptr;
	}
	/*internal*/ void Update() { //  C# is a weird language. 
		if (enabled) _sc->Render2D();
	}
	ScaleformWideScreen* _sc = nullptr;
	bool enabled;
	char title[256] = { "\0" };
	char subtitle[256] = { "\0" };
	char bodyText[256] = { "\0" };
	char textureDictionary[256] = { "\0" };
	char textureName[256] = { "\0" };
};