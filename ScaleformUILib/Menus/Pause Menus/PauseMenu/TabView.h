#pragma once
#include "..\PauseMenuBase.h"
#include "..\..\..\Scaleform\common.h"
class TabView;
class BaseTab;
using PauseMenuOpenEvent = void(*)(TabView* menu);
using PauseMenuCloseEvent = void(*)(TabView*);
using PauseMenuTabChange = void(*)(TabView*, BaseTab* tab, int i);
using PauseMenuFocusChange = void(*)(TabView* menu, BaseTab* tab, int focusLevel); // 
using PauseMenuOpenEvent = void(*)(TabView* menu, BaseTab* tab); // @IMPORTANT: ***NOT FINISHED ***
class TabView : public PauseMenuBase{
public:
	inline static string AUDIO_LIBRARY = "HUD_FRONTEND_DEFAULT_SOUNDSET";
	inline static string AUDIO_UPDOWN = "NAV_UP_DOWN";
	inline static string AUDIO_LEFTRIGHT = "NAV_LEFT_RIGHT";
	inline static string AUDIO_SELECT = "SELECT";
	inline static string AUDIO_BACK = "BACK";
	inline static string AUDIO_ERROR = "ERROR";
	string Title;
	string SubTitle;
	string SideStringTop;
	string SideStringMiddle;
	string SideStringBottom;
	bool ShowStoreBackground;
	int StoreBackgroundAnimationSpeed = 240; // internal get public set
	HudColour TabsColor = HudColour::HUD_COLOUR_PAUSE_BG;


private:
	bool isBuilding = false;
	

};