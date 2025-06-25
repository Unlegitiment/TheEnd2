#pragma once
#include "GTAVInfrastructure\ScriptEnv\Script.h"
#include <GTAVInfrastructure\SHV\natives.h>
#include "keyboard.h"
#include <LegitProject1\LInfras.h>
#include <GTAVInfrastructure\macros\WHILE.h>
#include <unordered_map>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <array>
#include <LegitProject1\ui\Hud.h>
#include <LegitProject1\ui\Scaleform\ScaleformMgr.h>
#include "LegitProject1\Markers\MarkerCommon.h"
void SHVDrawText(const char* string, float x, float y, float scale);
class CDeveloperFreecam {
public:
	std::string Stringify(const CVector3& vec) {
		return std::to_string(vec.GetX()) + " " + std::to_string(vec.GetY()) + " " + std::to_string(vec.GetZ());
	}
	CDeveloperFreecam() : m_CamPos(CAM::GET_GAMEPLAY_CAM_COORD()), m_CamRotation(0,0,0) {
		if (!CAM::DOES_CAM_EXIST(this->cCam)) {
			cCam = CAM::CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", 1);
		}
	}
	void Update() {
		if (IsKeyJustUp(VK_F16)) {
			CamActive = !CamActive;
			if (CamActive) {
				m_CamPos = CAM::GET_GAMEPLAY_CAM_COORD();
				CAM::RENDER_SCRIPT_CAMS(1, 0, 0, 0, 0, 0);
			} else{
				CAM::RENDER_SCRIPT_CAMS(0, 0, 0, 0, 0, 0);
			}
		}
		if (!CamActive) return;
		if (IsKeyJustUp(VK_F10)) {
			LAGInterface::Writeln("Debug Cam: \n\tDump To log: \n\t\t%f, %f, %f\n\t\t%f, %f, %f\n\t\t%f", m_CamPos.GetX(), m_CamPos.GetY(), m_CamPos.GetZ(), m_CamRotation.GetX(), m_CamRotation.GetY(), m_CamRotation.GetZ(), fCurrentFov);
		}
		StopPlayerMovement();
		if (PAD::IS_CONTROL_PRESSED(1, 32)) { //  --W key
			m_CamPos = m_CamPos + (GetForwardVec() * 0.1);
		}
		if (PAD::IS_CONTROL_PRESSED(1, 33)) { //  --S key
			m_CamPos = m_CamPos - (GetForwardVec() * 0.1);
		}
		if (PAD::IS_CONTROL_PRESSED(1, 34)) { //  --A key(move left);
			m_CamPos = m_CamPos + (GetRightVec() * 0.1);
		}
		if (PAD::IS_CONTROL_PRESSED(1, 35)) { //  --D key(move right);
			m_CamPos = m_CamPos - (GetRightVec() * 0.1);
		}
		if (PAD::IS_CONTROL_PRESSED(1, 44)) { //  --Q key(move up);
			m_CamPos = m_CamPos + CVector3(0.0, 0.0, 0.1);
		}
		if (PAD::IS_CONTROL_PRESSED(1, 38)) { //  --E key(move down);
			m_CamPos = m_CamPos - CVector3(0.0, 0.0, 0.1);
		}
		CAM::SET_CAM_COORD(this->cCam, m_CamPos.GetX(), m_CamPos.GetY(), m_CamPos.GetZ());
		if (PAD::IS_DISABLED_CONTROL_PRESSED(0, 16)) {
			this->fCurrentFov = max(30, fCurrentFov - 1.0);
		}
		if (PAD::IS_DISABLED_CONTROL_PRESSED(0, 17)) {
			this->fCurrentFov = min(120.f, fCurrentFov + 1.0);
		}
		CAM::SET_CAM_FOV(this->cCam, fCurrentFov);
		auto xMagnitude = PAD::GET_CONTROL_NORMAL(0, 1) * 8.0f; //--Mouse X
		auto yMagnitude = PAD::GET_CONTROL_NORMAL(0, 2) * 8.0f; //--Mouse Y
		m_CamRotation = CVector3(m_CamRotation.GetX() - yMagnitude, m_CamRotation.GetY(), m_CamRotation.GetZ() - xMagnitude);
		CAM::SET_CAM_ROT(cCam, m_CamRotation.GetX(), m_CamRotation.GetY(), m_CamRotation.GetZ(), 2);
		DrawCamData();
	}
	~CDeveloperFreecam() {
		CAM::RENDER_SCRIPT_CAMS(0, 0, 0, 0, 0, 0);
		CAM::DESTROY_CAM(this->cCam, 0);
	}
private:
	void DrawCamData() {
		SHVDrawText(Stringify(this->m_CamPos).c_str(), 0.1, 0.1, 1.0f);
		SHVDrawText(Stringify(this->m_CamRotation).c_str(), 0.1, 0.15f, 1.0f);
		SHVDrawText(std::to_string(this->fCurrentFov).c_str(), 0.1, 0.2, 1.0f);
	}
	void StopPlayerMovement() {
		PAD::DISABLE_CONTROL_ACTION(0, 30, true); //  --Move Left / Right
		PAD::DISABLE_CONTROL_ACTION(0, 31, true); //  --Move Up / Down
		PAD::DISABLE_CONTROL_ACTION(0, 140, true); //  --Melee Attack Light
		PAD::DISABLE_CONTROL_ACTION(0, 141, true); //  --Melee Attack Heavy
		PAD::DISABLE_CONTROL_ACTION(0, 142, true); //  --Melee Attack Alternative
		PAD::DISABLE_CONTROL_ACTION(0, 24, true); //  --Attack
		PAD::DISABLE_CONTROL_ACTION(0, 25, true); //  --Aim
		PAD::DISABLE_CONTROL_ACTION(0, 22, true); //  --Jump
		PAD::DISABLE_CONTROL_ACTION(0, 23, true); //  --Enter Vehicle
		PAD::DISABLE_CONTROL_ACTION(0, 75, true); //  --Exit Vehicle
		PAD::DISABLE_CONTROL_ACTION(0, 45, true); //  --Reload
		PAD::DISABLE_CONTROL_ACTION(0, 12, true); // - Weapon Wheel stuff
		PAD::DISABLE_CONTROL_ACTION(0, 13, true); // - Weapon Wheel stuff
		PAD::DISABLE_CONTROL_ACTION(0, 14, true); // - Weapon Wheel stuff
		PAD::DISABLE_CONTROL_ACTION(0, 15, true); // - Weapon Wheel stuff
		PAD::DISABLE_CONTROL_ACTION(0, 16, true); // - Weapon Wheel stuff
		PAD::DISABLE_CONTROL_ACTION(0, 17, true); // - Weapon Wheel stuff
		PAD::DISABLE_CONTROL_ACTION(0, 37, true); // - Weapon Wheel stuff
	}
	float fCurrentFov = 60;
	bool CamActive = false;
	const float PI = 3.14159265358979323846f;
	constexpr float Radians(float degrees) { return degrees * (PI / 180.0f); }
	CVector3 GetForwardVec() {
		float x = -std::sin(Radians(m_CamRotation.GetZ())) * std::abs(std::cos(Radians(m_CamRotation.GetX())));
		float y = std::cos(Radians(m_CamRotation.GetZ())) * std::abs(std::cos(Radians(m_CamRotation.GetX())));
		float z = std::sin(Radians(m_CamRotation.GetX()));
		return { x, y, z };
	}
	CVector3 GetRightVec() {
		auto forward = GetForwardVec();
		return { -forward.GetY(), forward.GetX(), 0.0 };
	}
	float Convert(float degree)
	{
		float pi = 3.14159265359;
		return (degree * (pi / 180));
	}
	Cam cCam;
	CVector3 m_CamPos;
	CVector3 m_CamRotation;
};
class CPauseMenuPostFx {
private:
	enum eState {
		START_FADE_OUT,
		START_FADE_IN,
		RUNNING,
		IDLE
	};
	eState m_CurrentState;
public:
	enum eCharacter {
		INVALID,
		FRANKLIN,
		MICHAEL,
		TREVOR,
		MP,
	};
	enum eFadeType {
		FT_IN,
		FT_OUT,
		FT_MAX
	};
	void Init(eCharacter character) {
		this->m_Character = character;
	}
	void StartFade(eFadeType fade) {
		if (fade == FT_IN) {
			m_CurrentState = START_FADE_IN;
		}
		else {
			m_CurrentState = START_FADE_OUT;
		}
	}
	void Update() {
		if (m_CurrentState == START_FADE_IN) {
			StartFadeIn();
			m_CurrentState = RUNNING;
		}
		if (m_CurrentState == START_FADE_OUT) {
			StartFadeOut();
			m_CurrentState = RUNNING;
		}
	}

	void Shutdown() {
		GRAPHICS::ANIMPOSTFX_STOP_ALL();
	}
	void SetCharacterColor(eCharacter c) {
		this->m_Character = c;
	}
	eCharacter GetCharacterColor() { return this->m_Character; }
	char* GetPostEffectRun(bool out = false) {
		return out ? GetPostEffectRun_OUT() : GetPostEffectForRun_IN();
	}
private:
	void StartFadeIn() {
		GRAPHICS::ANIMPOSTFX_PLAY(GetPostEffectRun(), 0, 1);
	}
	void StartFadeOut() {
		GRAPHICS::ANIMPOSTFX_STOP_ALL();
		GRAPHICS::ANIMPOSTFX_PLAY(GetPostEffectRun(true), 0, 1);
	}
	char* GetPostEffectForRun_IN() {
		switch (m_Character) {
		default:
		case MP:
			return "PauseMenuIn"; // DEFAULT
		}
	}
	char* GetPostEffectRun_OUT() {
		switch (m_Character) {
		default:
		case MP:
			return "PauseMenuOut"; // DEFAULT
		}
	}
	eCharacter m_Character;
	char* RunningPostEffect = "";
};
#define Static static inline
class CPauseMenu {
public:
	Static void Activate(Hash hFrontEndMenuVersion, bool pauseGame = 0, int OpenComponent = 0) {
		HUD::ACTIVATE_FRONTEND_MENU(hFrontEndMenuVersion, pauseGame, OpenComponent);
		CurrentMenuVersion = hFrontEndMenuVersion;
		IsOurFrontendActive = true;
		m_PostEffects = new CPauseMenuPostFx();
		m_PostEffects->Init(m_PostEffects->MP);
		m_PostEffects->StartFade(CPauseMenuPostFx::FT_IN); // FADE_IN
	}
	template<typename... T>
	Static void DoMethodOnHeader(const char* method, T&&... args) {
		if (GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD_ON_FRONTEND_HEADER(method)) {
			CScaleform scaleform = CScaleform(0); // default init scaleform call. 
			(scaleform.Call(std::forward<T>(args)), ...);
			GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
		}
	}
	template<typename... T>
	Static void DoMethodOnFrontend(const char* method, T&&... args) {
		if (GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD_ON_FRONTEND(method)) {
			CScaleform scaleform = CScaleform(0); // default init scaleform call. 
			(scaleform.Call(std::forward<T>(args)), ...);
			GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
		}
	}
	Static bool IsMapInteriorModeActive() {
		return HUD::IS_PAUSEMAP_IN_INTERIOR_MODE();
	}
	Static void Update() {
		if (!IsOurFrontendActive) return;
		m_PostEffects->Update();
	}
	Static void CloseMenu() {
		if (!m_PostEffects) return;
		m_PostEffects->StartFade(CPauseMenuPostFx::FT_OUT); // FADE_IN
		Update();
		delete m_PostEffects;
		m_PostEffects = 0;
		IsOurFrontendActive = false;
		CurrentMenuVersion = 0x0;
	}
	Static bool IsMyMenuActive() {
		return IsOurFrontendActive;
	}
	Static bool IsGameFrontendActive() {
		return HUD::IS_PAUSE_MENU_ACTIVE();
	}
	Static bool IsGameFrontendRestarting() {
		return HUD::IS_PAUSE_MENU_RESTARTING();
	}

private:
	Static bool IsOurFrontendActive = false;
	Static Hash CurrentMenuVersion = 0x0;
	Static CPauseMenuPostFx* m_PostEffects = nullptr;
};
class CGamemodeHud {
	bool m_bIsFogOfWarHidden = true;
	int m_iCurrentCharacterColor = 0;
	void DisablePauseMenuActivation() {
		PAD::DISABLE_CONTROL_ACTION(2, 199, 0); // Disable Pause Control actions
		PAD::DISABLE_CONTROL_ACTION(2, 200, 0);
	}
	void ReplaceCharacterColour(int m_FreemodeColor = 116) {
		HUD::REPLACE_HUD_COLOUR(144, m_FreemodeColor);
		m_iCurrentCharacterColor = m_FreemodeColor;
	}
	std::vector<int> m_Blips;
public:
	CGamemodeHud(int HudColour = 116) {
		ReplaceCharacterColour(HudColour); // BLACK (funzies)
		HUD::SET_PLAYER_ICON_COLOUR(37);
		for (int i = 0; i < 20; i++) {
			Blip b = HUD::ADD_BLIP_FOR_COORD(MISC::GET_RANDOM_INT_IN_RANGE(-1000, 1000), MISC::GET_RANDOM_INT_IN_RANGE(-1000, 1000), MISC::GET_RANDOM_INT_IN_RANGE(-1000, 1000));
			HUD::SET_BLIP_AS_MISSION_CREATOR_BLIP(b, 1);
			HUD::SET_BLIP_SPRITE(b, 304);
			HUD::BEGIN_TEXT_COMMAND_SET_BLIP_NAME("STRING");
			HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME("Mission");
			HUD::END_TEXT_COMMAND_SET_BLIP_NAME(b);
			LAGInterface::Writeln("[BlipID:%d]%d", b, HUD::IS_MISSION_CREATOR_BLIP(b));
			m_Blips.push_back(b);
		}
	}
	bool IsPauseMenuAccessed() {
		return PAD::IS_DISABLED_CONTROL_JUST_RELEASED(2, 199) || PAD::IS_DISABLED_CONTROL_JUST_RELEASED(2, 200);
	}
	void HideFogOfWar(bool value) {
		m_bIsFogOfWarHidden = value;
	}
	int TestVal = 0;
	bool m_bColumnTitleneedsUpdate = true;
	int LastColumnUpdate = 0;
	char ColumnTitle[12] = "Player List";
	int column1Colour = 0;
	int column2Colour = 0;
	int column3Colour = 0;
	int lastItemMenuId = 0;
	int selectedItemMenuId = 0;
	int selectedItemUniqueId = 0;
	int SelectedBlipId = 0;
	void Update() {
		HUD::SET_MINIMAP_HIDE_FOW(m_bIsFogOfWarHidden); // Disable Hud Fog.
		DisablePauseMenuActivation();
		if (IsPauseMenuAccessed() && !CPauseMenu::IsGameFrontendActive()) { // note we have to disable pausemenu this frame because else we reveal the outside of the apartment.
			CPauseMenu::Activate(MISC::GET_HASH_KEY("FE_MENU_VERSION_SP_PAUSE")); // Multiplayer_Pause
		}
		if (IsKeyJustUp(VK_F10)) {
			if (IsKeyDown(VK_CONTROL)) {
				TestVal -= 1;
				LAGInterface::Writeln("TestVal == %d", TestVal);
				return;
			}
			TestVal += 1;
			LAGInterface::Writeln("TestVal == %d", TestVal);
		}
		HUD::SET_RADAR_ZOOM(0);

		if (CPauseMenu::IsGameFrontendActive()) {
			if (IsKeyJustUp(VK_F13)) {
				int amtOfBlipsAreCreator = 0;
				for (auto& b : m_Blips) {
					if (!HUD::IS_MISSION_CREATOR_BLIP(b)) {
						HUD::SET_BLIP_AS_MISSION_CREATOR_BLIP(b, true);
						continue;
					}
					LAGInterface::Writeln("Blip: %d is a creator blip");
					amtOfBlipsAreCreator++;
				}
				LAGInterface::Writeln("AmtOfBlipsCreator: %d", amtOfBlipsAreCreator);
			}

			if (MISC::GET_GAME_TIMER() >= LastColumnUpdate + 100) {
				m_bColumnTitleneedsUpdate = true;
			}
			if (this->m_bColumnTitleneedsUpdate) {
				LastColumnUpdate = MISC::GET_GAME_TIMER();
				char* selectedCharacter = 0;
				int selectedIndex = 0;
				bool bIsCaptial = 0;
				selectedIndex = MISC::GET_RANDOM_INT_IN_RANGE(0, strnlen_s(ColumnTitle, 64) - 1); // idk if it includes the null character?
				selectedCharacter = &ColumnTitle[selectedIndex];
				bIsCaptial = std::isupper(*selectedCharacter);
				if (bIsCaptial) {
					*selectedCharacter = std::tolower(*selectedCharacter);
				}
				else {
					*selectedCharacter = std::toupper(*selectedCharacter);
				}
				column1Colour = MISC::GET_RANDOM_INT_IN_RANGE(0, 116);
				column2Colour = MISC::GET_RANDOM_INT_IN_RANGE(0, 116);
				column3Colour = MISC::GET_RANDOM_INT_IN_RANGE(0, 116);

				m_bColumnTitleneedsUpdate = false;
				//return;
			}
			CPauseMenu::DoMethodOnHeader("SET_HEADING_DETAILS", "Unknown_Player_1", "SAT?SUN?MON?TUES?WEDS", "BANK $?,???,rscStructure::FixUpPointer", false); // FUCKING FINALLY BABY!!!
			//CPauseMenu::DoMethodOnHeader("SET_CREW_TAG", true, true, "...RSG", true); // FUCKING FINALLY BABY!!!
			CPauseMenu::DoMethodOnHeader("SET_HEADER_TITLE", "CGAME::0x0000021277DA7720@@::RAGE_TITLE_NAME", true, "", false); // FUCKING FINALLY BABY!!!
			CPauseMenu::DoMethodOnHeader("SHIFT_CORONA_DESC", 0, 1); // FUCKING FINALLY BABY!!!
			CPauseMenu::DoMethodOnHeader("SET_MENU_HEADER_TEXT_BY_INDEX", 0, "SCALEFORM::MAP");
			CPauseMenu::DoMethodOnHeader("SET_MENU_HEADER_TEXT_BY_INDEX", 1, "CNetwork@@131");
			CPauseMenu::DoMethodOnHeader("SET_MENU_HEADER_TEXT_BY_INDEX", 2, "CNetwork@@m_SC->Friends");
			CPauseMenu::DoMethodOnHeader("SET_MENU_HEADER_TEXT_BY_INDEX", 3, "RAGE::INFORM");
			CPauseMenu::DoMethodOnHeader("SET_MENU_HEADER_TEXT_BY_INDEX", 4, "CNetTransaction");
			CPauseMenu::DoMethodOnHeader("SET_MENU_HEADER_TEXT_BY_INDEX", 5, "RAGE::SETCONFIG");
			CPauseMenu::DoMethodOnHeader("SET_MENU_HEADER_TEXT_BY_INDEX", 6, "RAGE::TELEMETRY");
			CPauseMenu::DoMethodOnHeader("SET_MENU_HEADER_TEXT_BY_INDEX", 7, "CSocialClubV1::GetPictures()", 1, 0);
			//CPauseMenu::DoMethodOnHeader("addColumn", "freemodeList", 8, 540);
			CPauseMenu::DoMethodOnHeader("SHOW_HEADING_DETAILS", true);
			CPauseMenu::DoMethodOnHeader("SHOW_MENU", true);
			//CPauseMenu::DoMethodOnFrontend("SET_DATA_SLOT_EMPTY", 0, 0);
			//CPauseMenu::DoMethodOnHeader("SET_DATA_SLOT", 0, 0, 0, 0, 0, 0, 0, 0, 0);
			//CPauseMenu::DoMethodOnFrontend("DISPLAY_DATA_SLOT", 0);
			//Unsure
			//CPauseMenu::DoMethodOnFrontend("SET_DATA_SLOT_EMPTY", 0, 0); // first is columnID, second is Index In itemList
			//CPauseMenu::DoMethodOnFrontend("SET_DATA_SLOT", 0, 0, 0, 0, 2, 3, false, "name1", 116, false, 0, 1, 0, ".*+ICO", false, "status", 1);
			//CPauseMenu::DoMethodOnFrontend("DISPLAY_DATA_SLOT", 0);

			//Details Card
			if (HUD::HAS_MENU_LAYOUT_CHANGED_EVENT_OCCURRED() || HUD::HAS_MENU_TRIGGER_EVENT_OCCURRED()) {
				HUD::GET_MENU_LAYOUT_CHANGED_EVENT_DETAILS(&this->lastItemMenuId,
					&this->selectedItemMenuId,
					&this->selectedItemUniqueId);
				LAGInterface::Writeln ("[%d, %d, %d]", this->lastItemMenuId, this->selectedItemMenuId, this->selectedItemUniqueId);
				HUD::GET_MENU_TRIGGER_EVENT_DETAILS(&this->lastItemMenuId, &selectedItemUniqueId);
				LAGInterface::Writeln(" %d, %d]", this->lastItemMenuId, this->selectedItemUniqueId); // Last Menu Id Tells me where we are in the menu. 
				LAGInterface::Writeln("%d, %d", HUD::PAUSE_MENU_GET_MOUSE_HOVER_INDEX(), HUD::PAUSE_MENU_GET_MOUSE_HOVER_UNIQUE_ID());
			}
				//LAGInterface::Writeln("Hovering over Mission Creator Blip!");
				SelectedBlipId = HUD::GET_NEW_SELECTED_MISSION_CREATOR_BLIP();
				//LAGInterface::Writeln("Selecting Blip: %d", SelectedBlipId);
			CPauseMenu::DoMethodOnFrontend("SET_DATA_SLOT_EMPTY", 1, 0); // first is columnID, second is Index In itemList
			CPauseMenu::DoMethodOnFrontend("SET_COLUMN_TITLE", 1, "TITLE", "TITLE1", "TITLE2", "TITLE3", "TITLE4", "TITLE5", "TITLE6");
			CPauseMenu::DoMethodOnFrontend("SET_DATA_SLOT", 1, 0, 0, 0, 2, 3, false, "name22", 116, false, 0, 1, 0, "..+ICO", false, "status", 1);
			int icon = 8;
			int iconComplete = 2;
			int iconColour = 116;
			CPauseMenu::DoMethodOnFrontend("SET_DATA_SLOT", 1, 1, 0, 1, 2, 3, 4, "Name", "IconRightText", icon, iconColour, iconComplete);
			CPauseMenu::DoMethodOnFrontend("DISPLAY_DATA_SLOT", 1);
			//Unsure
			//CPauseMenu::DoMethodOnFrontend("SET_DATA_SLOT_EMPTY", 2, 0); // first is columnID, second is Index In itemList
			//CPauseMenu::DoMethodOnFrontend("SET_DATA_SLOT", 2, 0, 0, 0, 2, 3, false, "name33", 116, false, 0, 1, 0, "..+ICO", false, "status", 1);
			//CPauseMenu::DoMethodOnFrontend("DISPLAY_DATA_SLOT", 2);
			//PLAYER LIST -- PauseMPMenuFriendsListItem -- pause_menu_shared_components.gfx
			//int PLAYERLIST_COLUMN_ID = 3;
			//CPauseMenu::DoMethodOnHeader("SET_MENU_ITEM_COLOUR", 0, column1Colour);
			//CPauseMenu::DoMethodOnHeader("SET_MENU_ITEM_COLOUR", 1, column2Colour);
			//CPauseMenu::DoMethodOnHeader("SET_MENU_ITEM_COLOUR", 2, column3Colour);
			//CPauseMenu::DoMethodOnHeader("SET_ALL_HIGHLIGHTS", 0, column3Colour);
			//CPauseMenu::DoMethodOnHeader("SET_ALL_HIGHLIGHTS", 1, column2Colour);
			//CPauseMenu::DoMethodOnHeader("SET_ALL_HIGHLIGHTS", 2, column1Colour);
			//CPauseMenu::DoMethodOnFrontend("SET_COLUMN_FOCUS", 0, 0, 1, 0);
			//CPauseMenu::DoMethodOnFrontend("SET_COLUMN_TITLE", 0, "TITLE", "TITLE1", "TITLE2", "TITLE3", "TITLE4", "TITLE5", "TITLE6");
			//CPauseMenu::DoMethodOnFrontend("SET_COLUMN_TITLE", 2, "TITLE", "TITLE1", "TITLE2", "TITLE3", "TITLE4", "TITLE5", "TITLE6");
			//CPauseMenu::DoMethodOnFrontend("SET_COLUMN_TITLE", 3, "TITLE", "TITLE1", "TITLE2", "TITLE3", "TITLE4", "TITLE5", "TITLE6");
			//CPauseMenu::DoMethodOnFrontend("SET_DATA_SLOT_EMPTY", PLAYERLIST_COLUMN_ID); // first is columnID, second is Index In itemList
			//for (int i = 0; i < 2; i++) {
			//	CPauseMenu::DoMethodOnFrontend("SET_DATA_SLOT", 
			//		PLAYERLIST_COLUMN_ID, // Column Id 
			//		i, // Index In Item List
			//		0, // Menu Id?
			//		0, // Unique Id?
			//		2, // TYPE?
			//		1000, // PlayerRank?
			//		false, // isSelectable 
			//		"ITheWorthy", // Player Name 0
			//		116, // Row Colour (HUDCOLOUR) 1
			//		false, // vespura: reduce colours (pretty much always off no benefit.) 2
			//		0, // unused3
			//		65, // RightPlayerIcon 4
			//		0, // unused5
			//		".*+RSG", // 6
			//		false, // vespura says : blink kick icon 7
			//		"INVITED", // status Symbol Text 8
			//		6, // Status Symbol colour 9
			//		TestVal // 10?
			//	);
			//	CPauseMenu::DoMethodOnFrontend("DISPLAY_DATA_SLOT", PLAYERLIST_COLUMN_ID);
			
			//int f = GRAPHICS::ANIMPOSTFX_IS_RUNNING("PauseMenuFranklinIn");
			if (CPauseMenu::IsMapInteriorModeActive()) {
				HUD::HIDE_MINIMAP_EXTERIOR_MAP_THIS_FRAME();
			}
			if (!CPauseMenu::IsMapInteriorModeActive()) {
				HUD::SET_FAKE_PAUSEMAP_PLAYER_POSITION_THIS_FRAME(26.38, -1408.922);
				HUD::HIDE_MINIMAP_INTERIOR_MAP_THIS_FRAME();
			}
			if (IsPauseMenuAccessed()) {
				HUD::SET_FRONTEND_ACTIVE(0);
			}
			PauseMenuNeedsFadeOut = true;
			return;
		}
		if (PauseMenuNeedsFadeOut) {
			LAGInterface::Writeln("Pause Menu has Faded Out!");
			CPauseMenu::CloseMenu();
			GRAPHICS::ANIMPOSTFX_STOP("SwitchHUDFranklinOut");
			GRAPHICS::ANIMPOSTFX_PLAY("SwitchHUDOut", 0, 0);
			PauseMenuNeedsFadeOut = false;
		}
		HUD::HIDE_MINIMAP_EXTERIOR_MAP_THIS_FRAME();
		//HUD::SET_RADAR_AS_INTERIOR_THIS_FRAME()
		if (HUD::IS_HUD_COMPONENT_ACTIVE(19)) { // this actually does work to get the weapon wheels state. Just gotta find pfx
			if (!GRAPHICS::ANIMPOSTFX_IS_RUNNING("SwitchHUDIn")) {
				GRAPHICS::ANIMPOSTFX_STOP("SwitchHUDFranklinIn");
				GRAPHICS::ANIMPOSTFX_PLAY("SwitchHUDIn", 0, 1);
			}
			MISC::SET_TIME_SCALE(1.0f);
			WeaponWheelCheck = true;
			return;
		}
		if (WeaponWheelCheck) {
			GRAPHICS::ANIMPOSTFX_STOP_ALL();
			GRAPHICS::ANIMPOSTFX_PLAY("SwitchHUDOut", 0, 0);
			WeaponWheelCheck = false;
		}
		CPauseMenu::Update();
	}
	void Shutdown() {
		if (CPauseMenu::IsGameFrontendActive()) {
			HUD::SET_FRONTEND_ACTIVE(0);
			CPauseMenu::CloseMenu();
		}
		for (auto& b : m_Blips) {
			HUD::REMOVE_BLIP(&b);
		}
	}
private:
	bool WeaponWheelCheck = false;
	bool PauseMenuNeedsFadeOut = false;
};
class CCarWash : public fwScriptEnv{
public:
	SCRIPT_INIT(CCarWash);
	CCarWash() = default; // this makes references impossible btw. however there isn't a good way around it? unless you want to change Functor to be variadic as well lmao.
	CCarWash(int arg_1, int arg_2) {
		this->m_bInsideInterior = arg_1;
	}
	void OnInit() {
		DLC::ON_ENTER_MP();
	}
	void OnShutdown() {
		m_Hud.Shutdown();
	}
	CVector3 PlayerCoords() {
		return ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
	}
	float PlayerHeading() {
		return ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID());
	}
	void InitiateLoad() {

	}
	Ped pClone = 0;
	Cam c = 0;
	bool IsPauseMenuAccessed() {
		return PAD::IS_DISABLED_CONTROL_JUST_RELEASED(2, 199) || PAD::IS_DISABLED_CONTROL_JUST_RELEASED(2, 200);
	}
	bool m_bInsideInterior = false;
	void OnTick() {
		m_FreeCam.Update();
		if (!SyncScene) {
			DrawFreemodeMarker(DoorLocation); // door marker
			DrawFreemodeMarker(GarageLocation); // garage
		}
		if (DoorLocation.DistCustomZ(PlayerCoords(), 1.1) <= 1.3 && !SyncScene) {
			if (!pClone) {
				pClone = PLAYER::PLAYER_PED_ID();
			}
			const char* dict = "anim@apt_trans@hinge_l";
			const char* anim = "ext_player"; // check
			STREAMING::REQUEST_ANIM_DICT(dict);
			WHILE(!STREAMING::HAS_ANIM_DICT_LOADED(dict));
			SyncScene = PED::CREATE_SYNCHRONIZED_SCENE(26.336155, -1408.914062, 28.230229, 0, 0, 358, 2);
			TASK::TASK_SYNCHRONIZED_SCENE(pClone, SyncScene, dict, anim, 1000.0, -8.0, 4, 0, 0x447a0000, 0);
			ENTITY::SET_ENTITY_COMPLETELY_DISABLE_COLLISION(pClone, 0, 0);
			c = CAM::CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", 1);
			/*
				25.314474, -1409.389648, 30.139713
                -2.267719 0.000000 -108.787979
                37.000000
			*/
			CAM::SHAKE_CAM(c, "HAND_SHAKE", 1.0f);
			CAM::SET_CAM_COORD(c, 25.314474, -1409.389648, 29.839713);
			CAM::SET_CAM_ROT(c, -2.267719, 0.000000f, -108.787979, 2);
			CAM::SET_CAM_FOV(c, 37.000000);
			CAM::RENDER_SCRIPT_CAMS(1, 0, 0, 0, 0, 0);
		}
		if (GarageLocation.DistCustomZ(PlayerCoords(), 1.1) <= 1.3 && !SyncScene) {
			if (!pClone) {
				pClone = PLAYER::PLAYER_PED_ID();
			}
			const char* dict = "anim@apt_trans@garage";
			const char* anim = "gar_open_3_left"; 
			STREAMING::REQUEST_ANIM_DICT(dict);
			WHILE(!STREAMING::HAS_ANIM_DICT_LOADED(dict));
			SyncScene = PED::CREATE_SYNCHRONIZED_SCENE(10.244, -1405.610, 28.281797, 0, 0.000000, 50.0, 2);
			TASK::TASK_SYNCHRONIZED_SCENE(pClone, SyncScene, dict, anim, 1000.0, -8.0, 4, 0, 0x447a0000, 0);
			ENTITY::SET_ENTITY_COMPLETELY_DISABLE_COLLISION(pClone, 0, 0);
			c = CAM::CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", 1);
			/*
				0.954014, -1402.701050, 31.967506
				-15.685034, 0.000000, -107.779564
				45.000000
			*/
			CAM::SHAKE_CAM(c, "HAND_SHAKE", 1.0f);
			CAM::SET_CAM_COORD(c, 0.954014, -1402.701050, 31.967506);
			CAM::SET_CAM_ROT(c, -15.685034, 0.000000, -107.779564, 2);
			CAM::SET_CAM_FOV(c, 45.000000);
			CAM::RENDER_SCRIPT_CAMS(1, 0, 0, 0, 0, 0);
		}
		float fSync = SyncScene ? PED::GET_SYNCHRONIZED_SCENE_PHASE(SyncScene) : 0;
		SHVDrawText(std::to_string(fSync).c_str(), 0.1, 0.1, 1.0f);
		if (SyncScene && fSync >= 0.65) {
			ENTITY::SET_ENTITY_COMPLETELY_DISABLE_COLLISION(pClone, 1, 1);
			ENTITY::SET_ENTITY_HAS_GRAVITY(pClone, 1);
			STREAMING::LOAD_SCENE(8.4070, -1399.707, -72.8997);
			CAM::RENDER_SCRIPT_CAMS(0, 0, 0, 0, 0, 0);
			Interior interior = INTERIOR::GET_INTERIOR_AT_COORDS(26.07468000, -1398.97900000, -75.00000000);
			INTERIOR::ACTIVATE_INTERIOR_ENTITY_SET(interior, m_InteriorShellEntitySet);
			INTERIOR::REFRESH_INTERIOR(interior);
			GRAPHICS::REMOVE_DECALS_IN_RANGE(26.07468000, -1398.97900000, -75.00000000, 19);
			TASK::CLEAR_PED_TASKS_IMMEDIATELY(pClone);
			CAM::DESTROY_CAM(c, 0);
			ENTITY::SET_ENTITY_COORDS(pClone, 8.2346, -1399.715, -75.1, 1, 0, 0, 1);
			ENTITY::SET_ENTITY_HEADING(pClone, 270.0);
			CAM::SET_GAMEPLAY_CAM_RELATIVE_PITCH(0.0, 0.0);

			//ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 26.26, -1402.355, -73.9997, 1, 0, 0, 1);
			PLAYER::SIMULATE_PLAYER_INPUT_GAIT(PLAYER::PLAYER_ID(), 1.0, 1000, 1.0, 1, 0, 0);
			SyncScene = 0;
			if (pClone != PLAYER::PLAYER_PED_ID()) {
				PED::DELETE_PED(&pClone);
			}
			m_bInsideInterior = true;
		}
		if (m_bInsideInterior) {
			m_Hud.Update();
		}
	}
private:
	CGamemodeHud m_Hud = CGamemodeHud(116);
	bool WeaponWheelCheck = false;
	bool PauseMenuNeedsFadeOut = false;
	int SyncScene = 0;
	CDeveloperFreecam m_FreeCam = CDeveloperFreecam();
	const char* m_InteriorShellEntitySet = "aanthologies.burlesque";
	CVector3 GarageLocation = { 10.22, -1405.5, 28.08 };
	CVector3 DoorLocation = { 26.25, -1409.9, 28.08 };
	int Test1 = 0, Test2 = 0;
};
template< typename T >
std::string int_to_hex(T i)
{
	std::stringstream stream;
	stream << "0x"
		<< std::setfill('0') << std::setw(sizeof(T) * 2)
		<< std::hex << i;
	return stream.str();
}
class CTest : public fwScriptEnv {
	SCRIPT_INIT(CTest);
private:
	Hash DominatorHash = MISC::GET_HASH_KEY("DOMINATOR");
	Vehicle Dominator = 0;
	bool m_bIsInvincible = false;
	int SCL = 0, SCL_BG = 0, SCL_FG = 0;
	int Scaleforms[3] = { 0 };
	bool isSceneRunning = false;

	struct hashTEst {
		unsigned long long hash;
		std::array<const char* , 2> strs;
	};
	void TestFunc() {
		//PLAYER::FORCE_CLEANUP(2);
		bool m_bTestCollisions = true;
		//ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), -1160.49400000, -1538.93200000, -49.00000000, 0, 0, 0, 1);
		Interior interior = INTERIOR::GET_INTERIOR_FROM_ENTITY(PLAYER::PLAYER_PED_ID());
		std::vector<hashTEst> m_Tests = {
			{ 0x0c2b3a7f, {
				"acarbuncle.guantanamo", // higgins helli base
				"aworkloaddentist" // higgons helli props
			}},
			{ 0x5128b294, {
				"aorthopaedists.snitching", // weed ?
				"adeputing_hansel" // weed props?
			}},
		};
		LAGInterface::Writeln("populated hashes : size: %d ", m_Tests.size());
		WHILE(m_bTestCollisions) {
			auto it = m_Tests.begin();
			std::array<const char*, 2> m_Props = {};
			int index = 0;
			bool needsUpdate = true;
			WHILE(it != m_Tests.end()) {
				if (needsUpdate) {
					m_Props[0] = it->strs[0];
					m_Props[1] = it->strs[1]; // done
					INTERIOR::ACTIVATE_INTERIOR_ENTITY_SET(interior, m_Props[0]);
					INTERIOR::ACTIVATE_INTERIOR_ENTITY_SET(interior, m_Props[1]);
					INTERIOR::REFRESH_INTERIOR(interior);
					needsUpdate = false;
				}
				if (IsKeyJustUp(VK_F15)) {
					index++;
					INTERIOR::DEACTIVATE_INTERIOR_ENTITY_SET(interior, m_Props[0]);
					INTERIOR::DEACTIVATE_INTERIOR_ENTITY_SET(interior, m_Props[1]);
					INTERIOR::REFRESH_INTERIOR(interior);
					LAGInterface::Writeln("Deactivating interior set: %s , %s", m_Props[0], m_Props[1]);
					needsUpdate = true;
					if (index > m_Tests.size()) {
						index = 0;
					}
					it = m_Tests.begin() + index;
				}
			}
			m_bTestCollisions = false;
		};

		INTERIOR::REFRESH_INTERIOR(interior);
	}
	void CarWash() {
		const std::vector<const char*> Strings = { "aanthologies.burlesque", // all of these are the shell props for the game. just required a little bit of cleanup from collision checker.
	"acallouses_inebriations",
	"aguffhuffiness",
	"amummersspousals",
	"aintracranialbabushkas",
	"aphotoengravings_reexplaining",
	"abaptizerdebs" };
		bool m_bTestStrings = true;
		Interior interior = INTERIOR::GET_INTERIOR_AT_COORDS(26.07468000, -1398.97900000, -75.00000000);
		if (INTERIOR::GET_INTERIOR_FROM_ENTITY(PLAYER::PLAYER_PED_ID()) != interior) {
			ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 26.07468000, -1398.97900000, -74.00000000, 1, 0, 0, 1);
			GRAPHICS::REMOVE_DECALS_IN_RANGE(26.07468000, -1398.97900000, -75.00000000, 19);
		}
		WHILE(m_bTestStrings) {
			auto it = Strings.begin();
			bool needsUpdate = true;
			WHILE(it != Strings.end()) {
				//GRAPHICS::DRAW_MARKER_SPHERE(26.07468000, -1398.97900000, -75.00000000, 10, 255,0,0,0.7);
				if (needsUpdate) {
					INTERIOR::ACTIVATE_INTERIOR_ENTITY_SET(interior, *it);
					INTERIOR::REFRESH_INTERIOR(interior);
					needsUpdate = false;
				}
				if (IsKeyJustUp(VK_F15)) {
					INTERIOR::DEACTIVATE_INTERIOR_ENTITY_SET(interior, *it);
					INTERIOR::REFRESH_INTERIOR(interior);
					it++;
					needsUpdate = true;
				}
			}
			m_bTestStrings = false;
		}
	}
public:
	// Inherited via ScriptableBase
	float fRadius = 19.f;
	CHud* hud = nullptr;
	fwScriptEnv* m_pCarWash = nullptr;
	//fwScriptEnv* m_CarWash = nullptr;
	void OnTick() override
	{
		//logf("Test");
		if (IsKeyJustUp(VK_F14)&& !m_pCarWash) {
			static int i = 0;
			m_pCarWash = CScriptRuntime::Get()->AddScriptAndCreate<CCarWash>(1,0); // idk if this would work
			CScriptRuntime::Get()->PrintAll();
		}
		if (IsKeyJustUp(VK_F15) && m_pCarWash) {
			CScriptRuntime::Get()->TerminateScript(m_pCarWash);
			CScriptRuntime::Get()->PrintAll();
			m_pCarWash = nullptr;
		}
		hud->Update();
		/*
			hs4f_drp_off cutscene where lootbag gets dropped off. AFTER FIRST HEIST NOT MADRAZO CUT
			hs4f_drp_cel cutscene where player drinks and throws lime off of balcony
		*/

	}
	bool m_bIsScaleformActive = false;
	void ScaleformInit() {
		const char* scl_base = "HEIST_CELEBRATION";
		const char* scl_base_bg = "HEIST_CELEBRATION_BG";
		const char* scl_base_fg = "HEIST_CELEBRATION_FG";
		SCL = GRAPHICS::REQUEST_SCALEFORM_MOVIE(scl_base);
		SCL_BG = GRAPHICS::REQUEST_SCALEFORM_MOVIE(scl_base_bg);
		SCL_FG = GRAPHICS::REQUEST_SCALEFORM_MOVIE(scl_base_fg);
		WHILE(!GRAPHICS::HAS_SCALEFORM_MOVIE_LOADED(SCL)) {
			SCL = GRAPHICS::REQUEST_SCALEFORM_MOVIE(scl_base);
		}
		WHILE(!GRAPHICS::HAS_SCALEFORM_MOVIE_LOADED(SCL_BG)) {
			SCL_BG = GRAPHICS::REQUEST_SCALEFORM_MOVIE(scl_base_bg);
		}
		WHILE(!GRAPHICS::HAS_SCALEFORM_MOVIE_LOADED(SCL_FG)) {
			SCL_FG = GRAPHICS::REQUEST_SCALEFORM_MOVIE(scl_base_fg);
		}
		Scaleforms[0] = SCL;
		Scaleforms[1] = SCL_BG;
		Scaleforms[2] = SCL_FG;
	}
	//HeistCelebStatTable statTable;
	void OnInit() override
	{
		logf("Init");
		CScaleformManager::Init();
		hud = new CHud();
		m_pCarWash = CScriptRuntime::Get()->AddScriptAndCreate<CCarWash>(1, 0); // idk if this would work
	}
	void OnShutdown() override
	{
		delete hud;
		LAGInterface::Writeln("Help");
		LAGInterface::Writeln("[CTest] Shutdown called");
		m_bIsInvincible = false;
		ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), false);
	}
};