#pragma once
#include <ScaleformUILib\Scaleform\ScaleformExtensions.h>
#include <algorithm>
#define USE_LIST
#include "..\common.h"
#include <string>
enum ScoreDisplayType
{
	NUMBER_ONLY = 0,
	ICON = 1,
	NONE = 2
};
enum ScoreRightIconType
{
	NONE = 0,
	INACTIVE_HEADSET = 48,
	MUTED_HEADSET = 49,
	ACTIVE_HEADSET = 47,
	RANK_FREEMODE = 65,
	KICK = 64,
	LOBBY_DRIVER = 79,
	LOBBY_CODRIVER = 80,
	SPECTATOR = 66,
	BOUNTY = 115,
	DEAD = 116,
	DPAD_GANG_CEO = 121,
	DPAD_GANG_BIKER = 122,
	DPAD_DOWN_TARGET = 123
};
struct PlayerRow {
	int ServerId;
	string Name;
	string RightText;
	int Color;
	string IconOverlayText;
	string JobPointsText;
	string CrewLabelText;
	ScoreDisplayType JobPointsDisplayType;
	ScoreRightIconType RightIcon;
	string TextureString;
	char FriendType;
};
/*
	I'm not gonna lie to you. I don't have faith in this specific one just because of std::vector alone. 
	Also heap allocations are expensive but we do them here anyway. 
	Probably bad lmao most things are Heap Allocs because C# is a Garbage Collected Language. 
*/
class PlayerListHandler {
public:
	void Load() {
		if (_sc != nullptr) return;
		_sc = new ScaleformWideScreen("MP_MM_CARD_FREEMODE");
		int timeout = 1000;
		int start = MISC::GET_GAME_TIMER();
		while (!_sc->IsLoaded() && MISC::GET_GAME_TIMER() - start < timeout) WAIT(0);
	}
	void Dispose() {
		if (_sc == nullptr) return;
		Enabled = false;
		Index = 0;
		MaxPages = 1;
		CurrentPage = 0;
		TitleLeftText = "";
		TitleRightText = "";
		TitleIcon = 0;
		_sc->CallFunction("SET_DATA_SLOT_EMPTY");
		_sc->Dispose();
		delete _sc;
		_sc = nullptr;
		for (int i = 0; i < 1024; i++) {
			PED::UNREGISTER_PEDHEADSHOT(i);
		}
	}
	~PlayerListHandler() {
		delete PlayerRows; // if we kill PlayerListHandler this has to go too. Else we'd leak. 
	}
	void SetTitle(string left, string right, int icon) {
		TitleLeftText = left;
		TitleRightText = right;
		TitleIcon = icon;
	}
	void AddRow(PlayerRow* row) {
		PlayerRows->push_back(row);
	}
	void RemoveRow(PlayerRow* row) {
		auto it = std::find(PlayerRows->begin(), PlayerRows->end(), row);
		if(it != PlayerRows->end()) PlayerRows->erase(it);
		std::sort(PlayerRows->begin(), PlayerRows->end(), [](PlayerRow* a, PlayerRow* b) {
			return a->ServerId < b->ServerId;
			});
	}
	void RemoveRow(int serverId) {
		auto it = std::find_if(PlayerRows->begin(), PlayerRows->end(), [serverId](PlayerRow* row) {return row->ServerId == serverId; });
		if (it != PlayerRows->end()) {
			PlayerRows->erase(it);
			std::sort(PlayerRows->begin(), PlayerRows->end(), [](PlayerRow* a, PlayerRow* b) {
				return a->ServerId < b->ServerId;
			});
		}
	}
	void NextPage() {
		UpdateMaxPages();
		_start = MISC::GET_GAME_TIMER();
		_timer = 8000;
		BuildMenu();
		if (CurrentPage > MaxPages) {
			CurrentPage = 0;
			Enabled = false;
			_start = 0;
			Dispose();
			return;
		}
	}
	void SetHighlight(int idx) {
		_sc->CallFunction("SET_HIGHLIGHT", idx);
	}
	void DisplayMic(int idx, int unk) {
		_sc->CallFunction("DISPLAY_MIC", idx, unk);
	}
	void UpdateSlot(PlayerRow* row) {
		auto r = std::find(PlayerRows->begin(), PlayerRows->end(), row);
		if (r != PlayerRows->end()) {
			*r = row;
			if (row->CrewLabelText != "") {
				_sc->CallFunction("UPDATE_SLOT", std::distance(PlayerRows->begin(), r), row->RightText, row->Name, row->Color, (int)row->RightIcon, row->IconOverlayText, row->JobPointsText, std::string("..+")+row->CrewLabelText, (int)row->JobPointsDisplayType, row->TextureString, row->TextureString, row->FriendType);
			}
			else {
				_sc->CallFunction("UPDATE_SLOT", std::distance(PlayerRows->begin(), r), row->RightText, row->Name, row->Color, (int)row->RightIcon, row->IconOverlayText, row->JobPointsText, "", (int)row->JobPointsDisplayType, row->TextureString, row->TextureString, row->FriendType);
			}
		}
	}
	void SetIcon(int index, ScoreRightIconType icon, string txt) {
		PlayerRow* row = PlayerRows->at(index);
		if (row != nullptr) {
			_sc->CallFunction("SET_ICON", index, (int)icon, txt);
		}
	}
	static inline bool IsNullOrWhiteSpace(const std::string& str) {
		return str.empty() || std::all_of(str.begin(), str.end(), isspace);
	}
	void BuildMenu() {
		Load();
		List<PlayerRow*> rows = List<PlayerRow*>();
		_sc->CallFunction("SET_DATA_SLOT_EMPTY");
		_sc->CallFunction("SET_TITLE", TitleLeftText, TitleRightText, TitleIcon);
		for (auto it = PlayerRows->begin(); it < PlayerRows->end(); it++) {
			if (IsRowSupposedToShow(std::distance(PlayerRows->begin(), it))) {
				rows.push_back(*it);
			}
			Index++;
		}
		Index = 0;
		for (PlayerRow* row : rows) {
			if (IsNullOrWhiteSpace(row->CrewLabelText)) {
				_sc->CallFunction("SET_DATA_SLOT", Index, row->RightText, row->Name, row->Color, (int)row->RightIcon, row->IconOverlayText, row->JobPointsText, "", (int)row->JobPointsDisplayType, row->TextureString, row->TextureString, row->FriendType);
			}
			else {
				_sc->CallFunction("SET_DATA_SLOT", Index, row->RightText, row->Name, row->Color, (int)row->RightIcon, row->IconOverlayText, row->JobPointsText, std::string("..+") + row->CrewLabelText, (int)row->JobPointsDisplayType, row->TextureString, row->TextureString, row->FriendType);
			}
			Index++;
		}
		_sc->CallFunction("DISPLAY_VIEW");
	}
private:
	bool IsRowSupposedToShow(int row) {
		if (CurrentPage > 0)
		{
			int max = CurrentPage * 16;
			int min = CurrentPage * 16 - 16;

			if (row >= min && row < max) return true;
		}
		return false;
	}
	void UpdateMaxPages()
	{
		MaxPages = (int)ceil(PlayerRows->size() / 16.f);
	}
	void Update() {
		GRAPHICS::DRAW_SCALEFORM_MOVIE(_sc->GetHandle(), Position.X(), Position.Y(), 0.28f, 0.6f, 255, 255, 255, 255, 0);
		if (_start != 0 && MISC::GET_GAME_TIMER() - _start > _timer)
		{
			CurrentPage = 0;
			Enabled = false;
			_start = 0;
			Dispose();
			return;
		}
	}
	void SetCurrentPage(int value) {
		if (PlayerRows->empty())
		{
			CurrentPage = 0;
			return;
		}
		CurrentPage = value;
		if (CurrentPage > 0)
		{
			Enabled = true;
			NextPage();
		}
	}
	int GetCurrentPage() {
		return CurrentPage;
	}
public:
	string TitleLeftText;
	string TitleRightText;
	int TitleIcon;
	bool Enabled;
	int MaxPages = 1;
	PointF Position;
	List<PlayerRow*>* PlayerRows = new List<PlayerRow*>(); // Lads I do hate this. 
private:
	int _start;
	int _timer;
	ScaleformWideScreen* _sc;
	int Index = 0;
	int CurrentPage = 0;
	PointF _position = PointF(0.112f, 0.3f);
};