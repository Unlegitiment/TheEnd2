#pragma once
#include "ScaleformUILib/Scaleform/common.h" 
#include "ScaleformUILib/Scaleform/ScaleformExtensions.h" 
#include <LegitProject1\LInfras.h>
enum ChatScope
{
	Global = 0,
	Team = 1,
	All = 2,
	Clan = 3,
};

enum ChatVisibility
{
	Hidden = 0,
	Default = 1,
	Typing = 2,
};
class MultiplayerChatHandler
{
private:
	const string SCALEFORM_NAME = "MULTIPLAYER_CHAT";
	ScaleformWideScreen* _sc = nullptr;

	bool _isTyping;
	int _start = 0;
	int _duration = 5000;
public:
	MultiplayerChatHandler() {}

	void Load()
	{
		if (_sc != nullptr) return;
		_sc = new ScaleformWideScreen(SCALEFORM_NAME);
		int timeout = 1000;
		int start = MISC::GET_GAME_TIMER();
		while (!_sc->IsLoaded() && MISC::GET_GAME_TIMER() - start < timeout)
		{
			LAGInterface::Writeln("%s : loading", SCALEFORM_NAME);
			WAIT(0);
		}
	}

private: void Dispose()
{
	_sc->Dispose();
	delete _sc;
	_sc = nullptr;
}
public:
	void SetFocus(ChatVisibility visibility, ChatScope scope = ChatScope::Global, string scopeText = "All", string playerName = "", HudColour color = HudColour::HUD_COLOUR_PURE_WHITE)
	{
		if (visibility == ChatVisibility::Typing)
			_isTyping = true;
		else if (visibility == ChatVisibility::Hidden)
			_isTyping = false;
		else if (visibility == ChatVisibility::Default)
			_start = MISC::GET_GAME_TIMER();
		_sc->CallFunction("SET_FOCUS", (int)visibility, (int)scope, scopeText, playerName, (int)color);
	}

	void AddMessage(string playerName, string message, ChatScope scope, bool teamOnly, HudColour color)
	{
		_sc->CallFunction("ADD_MESSAGE", playerName, message, (int)scope, teamOnly, (int)color);
	}

	void Close()
	{
		SetFocus(ChatVisibility::Hidden);
		_start = 0;
		_isTyping = false;
	}

	void Show() { SetFocus(ChatVisibility::Default); }
	void StartTyping() { SetFocus(ChatVisibility::Typing); }
	void Hide() { SetFocus(ChatVisibility::Hidden);}
	 void Reset() { _sc->CallFunction("RESET"); }
	 void PageUp()
	{
		if (!_isTyping) return;
		_sc->CallFunction("PAGE_UP");
	}

	 void PageDown()
	{
		if (!_isTyping) return;
		_sc->CallFunction("PAGE_DOWN");
	}

	 void DeleteText()
	{
		if (!_isTyping) return;
		_sc->CallFunction("DELETE_TEXT");
	}

	 void AddText(string text)
	{
		if (!_isTyping) return;
		_sc->CallFunction("ADD_TEXT", text);
	}

	 void CompleteText()
	{
		if (!_isTyping) return;
		_sc->CallFunction("COMPLETE_TEXT");
	}

	 void AbortText()
	{
		if (!_isTyping) return;
		_sc->CallFunction("ABORT_TEXT");
	}

	 bool IsTyping() { return _isTyping; }
	 void SetDuration(int duration) { _duration = duration; }

	 void Update()
	{
		if (_sc == nullptr|| !_sc->IsLoaded()) return;
		_sc->Render2D();

		if (!_isTyping && MISC::GET_GAME_TIMER() - _start > _duration)
			Close();
	}
};

