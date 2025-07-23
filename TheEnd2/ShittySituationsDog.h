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
#include "streaming_replacement\fwStream.h"
void SHVDrawText(const char* string, float x, float y, float scale);
class CDeveloperFreecam {
public:
	std::string Stringify(const CVector3& vec) {
		return std::to_string(vec.GetX()) + " " + std::to_string(vec.GetY()) + " " + std::to_string(vec.GetZ());
	}
	CDeveloperFreecam() : m_CamPos(CAM::GET_GAMEPLAY_CAM_COORD()), m_CamRotation(0, 0, 0) {
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
			}
			else {
				CAM::RENDER_SCRIPT_CAMS(0, 0, 0, 0, 0, 0);
			}
		}
		if (!CamActive) return;
		if (IsKeyJustUp(VK_F10)) {
			LAGInterface::Writeln("Debug Cam: \n\tDump To log: \n\t\t%f, %f, %f\n\t\t%f, %f, %f\n\t\t%f", m_CamPos.GetX(), m_CamPos.GetY(), m_CamPos.GetZ(), m_CamRotation.GetX(), m_CamRotation.GetY(), m_CamRotation.GetZ(), fCurrentFov);
		}
		StopPlayerMovement();
		static constexpr int SPEED = 5.0f;
		if (PAD::IS_CONTROL_PRESSED(1, 32)) { //  --W key
			m_CamPos = m_CamPos + (GetForwardVec() * SPEED);
		}
		if (PAD::IS_CONTROL_PRESSED(1, 33)) { //  --S key
			m_CamPos = m_CamPos - (GetForwardVec() * SPEED);
		}
		if (PAD::IS_CONTROL_PRESSED(1, 34)) { //  --A key(move left);
			m_CamPos = m_CamPos + (GetRightVec() * SPEED);
		}
		if (PAD::IS_CONTROL_PRESSED(1, 35)) { //  --D key(move right);
			m_CamPos = m_CamPos - (GetRightVec() * SPEED);
		}
		if (PAD::IS_CONTROL_PRESSED(1, 44)) { //  --Q key(move up);
			m_CamPos = m_CamPos + CVector3(0.0, 0.0, SPEED);
		}
		if (PAD::IS_CONTROL_PRESSED(1, 38)) { //  --E key(move down);
			m_CamPos = m_CamPos - CVector3(0.0, 0.0, SPEED);
		}
		CAM::SET_CAM_COORD(this->cCam, m_CamPos.GetX(), m_CamPos.GetY(), m_CamPos.GetZ());
		if (PAD::IS_DISABLED_CONTROL_PRESSED(0, 16)) {
			this->fCurrentFov = max(30, fCurrentFov - 1.0f);
		}
		if (PAD::IS_DISABLED_CONTROL_PRESSED(0, 17)) {
			this->fCurrentFov = min(120.f, fCurrentFov + 1.0f);
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
class DevTools {
public:
	DevTools() {

	}
	static void InitClass() {
		sm_pDevTools = new DevTools;
	}
	static DevTools* GetClass() {
		return sm_pDevTools;
	}
	void Init() {
	
	}
	void Update() {
		sm_Freecam.Update();
	}
	void Destroy() {
		
	}
	~DevTools() {
		this->Destroy();
	}
	static void DestroyClass() {
		delete sm_pDevTools;
	}
private:
	CDeveloperFreecam sm_Freecam;
	static inline DevTools* sm_pDevTools = nullptr;
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
class CScaleformBase {
public:
	template<typename T> void Call(T) { LAGInterface::Writeln("Invalid Type specified To Frontend! Type: %s" typeid(T).name()); }
	template<>void Call<const char*>(const char* x) { GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING(x); }
	template<>void Call<char[]>(char x[]) { GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING(x); }
	template<>void Call<int>(int x) { GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(x); }
	template<>void Call<float>(float f) { GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_FLOAT(f); }
	template<>void Call<bool>(bool x) { GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_BOOL(x); }
	template<>void Call<char*>(char* x) { GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING(x); }
};
class CFrontendMenu {
public:
	template<typename... T>
	void Header(const char* Method, T&&... args) {
		GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD_ON_FRONTEND_HEADER(Method);
		(m_SclBase.Call(std::forward<T>(args)), ...);
		GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
	}
	template<typename... T>
	void Frontend(const char* Method, T&&... args) {
		GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD_ON_FRONTEND(Method);
		(m_SclBase.Call(std::forward<T>(args)), ...);
		GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
	}
	CScaleformBase& GetBase() { return m_SclBase; }
private:
	CScaleformBase m_SclBase = CScaleformBase();
};
//This is just a frontend mapping of items hmm. 
/*
			//CPauseMenu::DoMethodOnHeader("SET_HEADING_DETAILS", "Unknown_Player_1", "SAT?SUN?MON?TUES?WEDS", "BANK $?,???,rscStructure::FixUpPointer", false); // FUCKING FINALLY BABY!!!
			//CPauseMenu::DoMethodOnHeader("SET_CREW_TAG", true, true, "...RSG", true); // FUCKING FINALLY BABY!!!
			//CPauseMenu::DoMethodOnHeader("SET_HEADER_TITLE", "CGAME::0x0000021277DA7720@@::RAGE_TITLE_NAME", true, "", false); // FUCKING FINALLY BABY!!!
			//CPauseMenu::DoMethodOnHeader("SHIFT_CORONA_DESC", 0, 1); // FUCKING FINALLY BABY!!!
			//CPauseMenu::DoMethodOnHeader("SET_MENU_HEADER_TEXT_BY_INDEX", 0, "SCALEFORM::MAP");
			//CPauseMenu::DoMethodOnHeader("SET_MENU_HEADER_TEXT_BY_INDEX", 1, "CNetwork@@131");
			//CPauseMenu::DoMethodOnHeader("SET_MENU_HEADER_TEXT_BY_INDEX", 2, "CNetwork@@m_SC->Friends");
			//CPauseMenu::DoMethodOnHeader("SET_MENU_HEADER_TEXT_BY_INDEX", 3, "RAGE::INFORM");
			//CPauseMenu::DoMethodOnHeader("SET_MENU_HEADER_TEXT_BY_INDEX", 4, "CNetTransaction");
			//CPauseMenu::DoMethodOnHeader("SET_MENU_HEADER_TEXT_BY_INDEX", 5, "RAGE::SETCONFIG");
			//CPauseMenu::DoMethodOnHeader("SET_MENU_HEADER_TEXT_BY_INDEX", 6, "RAGE::TELEMETRY");
			//CPauseMenu::DoMethodOnHeader("SET_MENU_HEADER_TEXT_BY_INDEX", 7, "CSocialClubV1::GetPictures()", 1, 0);
			//CPauseMenu::DoMethodOnHeader("addColumn", "freemodeList", 8, 540);
			//CPauseMenu::DoMethodOnHeader("SHOW_HEADING_DETAILS", true);
			//CPauseMenu::DoMethodOnHeader("SHOW_MENU", true);
			//CPauseMenu::DoMethodOnFrontend("SET_DATA_SLOT_EMPTY", 0, 0);
			//CPauseMenu::DoMethodOnHeader("SET_DATA_SLOT", 0, 0, 0, 0, 0, 0, 0, 0, 0);
			//CPauseMenu::DoMethodOnFrontend("DISPLAY_DATA_SLOT", 0);
			//Unsure
			//CPauseMenu::DoMethodOnFrontend("SET_DATA_SLOT_EMPTY", 0, 0); // first is columnID, second is Index In itemList
			//CPauseMenu::DoMethodOnFrontend("SET_DATA_SLOT", 0, 0, 0, 0, 2, 3, false, "name1", 116, false, 0, 1, 0, ".*+ICO", false, "status", 1);
			//CPauseMenu::DoMethodOnFrontend("DISPLAY_DATA_SLOT", 0);
*/
class CPauseMenuBase {
public:
	void ActivateMenu() {
		StartInPostEffects();
		// we don't actually worry about opening the menu here. we don't care about that. 
		m_pScaleform = new CFrontendMenu();
	}
	//Header Methods. (not all but most of them in fairness. I think im missing some CREW stuff and character profile picture but no use to modify picture lmao. and the crew stuff is broken.)
	void ShowHeadingDetails(bool b = true) {
		if (!m_pScaleform) return;
		m_pScaleform->Header("SHOW_HEADING_DETAILS", b);
	}
	void ShowHeader(bool b = true) {
		if (!m_pScaleform) return; // you don't have an active menu lmao
		m_pScaleform->Header("SHOW_MENU", b); // I think this is syntax check below
	}
	void SetHeaderTitle(const char* title, bool Somevar, const char* subHeader, bool SomeOthervar) { // @TODO: Fix var name
		if (!m_pScaleform) return;
		m_pScaleform->Header("SET_HEADER_TITLE", title, Somevar, subHeader, SomeOthervar);
	}
	void SetHeadingDetails(const char* FirstLine, const char* SecondLine, const char* ThirdLine, bool SomeFlag) {
		if (!m_pScaleform) return;
		m_pScaleform->Header("SET_HEADING_DETAILS", FirstLine, SecondLine, ThirdLine, SomeFlag); // maybe separate header flags?
	}
	void ShiftCoronaDescription(bool arg0, bool arg1) {
		if (!m_pScaleform) return;
		m_pScaleform->Header("SHIFT_CORONA_DESC", arg0, arg1);
	}
	void SetMenuHeaderTextByIndex(int idx, const char* Title) {
		if (!m_pScaleform) return;
		m_pScaleform->Header("SET_MENU_HEADER_TEXT_BY_INDEX", Title);
	}
	//begin of annoying stuff.

	void EmptyDataSlot(int PM_COLUMN_ID, int m_ItemIndex) {
		//CPauseMenu::DoMethodOnFrontend("SET_DATA_SLOT_EMPTY", 0, 0); // first is columnID, second is Index In itemList
		if (!m_pScaleform) return;
		m_pScaleform->Frontend("SET_DATA_SLOT_EMPTY", PM_COLUMN_ID, m_ItemIndex); // fun.
	}
	/*
	7 standard arguments but can vary depending on the scaleform lmao Here:
		0	Column Index	INT	The column containing the slot to set the data of.
		1	slotIndex	INT	The index of slot to set the data of.
		2	menuID	INT	Used to keep track of what item of the pause menu this slot belongs to. You should use IDs from com.rockstargames.gtav.constants.PauseMenuLUT where it makes sense to do so.
		3	uniqueID	INT	Used to uniquely identify items within a view. You can really use anything you want however, integers greater than 1000 can have special functions and execute actions when clicked that you’d otherwise not want to happen so, to be safe, don’t use anything greater than 1000.
		4	type	INT	The subtype of this item.
		5	initialIndex	INT	The initial index to show in a list item.
		6	isSelectable	BOOL	A boolean controlling if the item can be selected. Unselectable items are greyed out when displayed and skipped over in navigation.
	Src: https://forum.cfx.re/t/lobby-menus-discussion-pause-menu-research/3909562/12
	*/
	template<typename... Args>
	void SetDataSlot(int PM_COLUMN_ID, int ItemId, Args&&... args) {
		if (!m_pScaleform) return;
		m_pScaleform->Frontend("SET_DATA_SLOT", PM_COLUMN_ID, ItemId, std::forward<Args>(args)...); // gotta perfect forward lmao
	}
	void DisplayDataSlot(int PM_COLUMN_ID) { // check vars
		//CPauseMenu::DoMethodOnFrontend("DISPLAY_DATA_SLOT", 0);
		if (!m_pScaleform) return;
		m_pScaleform->Frontend("DISPLAY_DATA_SLOT", PM_COLUMN_ID); // gotta perfect forward lmao
	}
	void CloseMenu() {
		ShutdownPostEffects();
	}
	CFrontendMenu*& GetRawScaleform() {
		return m_pScaleform; // if you shoot me in the foot I will execute you. 
	}
private:
	void StartInPostEffects() {
		m_pPostEffects = new CPauseMenuPostFx();
		m_pPostEffects->Init(CPauseMenuPostFx::MP);
		m_pPostEffects->StartFade(CPauseMenuPostFx::FT_IN); // open Menu
		m_pPostEffects->Update();
	}
	void ShutdownPostEffects() {
		m_pPostEffects->StartFade(CPauseMenuPostFx::FT_OUT);
		m_pPostEffects->Update();
		//m_pPostEffects->Shutdown();
		delete m_pPostEffects;
		m_pPostEffects = nullptr;
	}
	CFrontendMenu* m_pScaleform = nullptr;
	CPauseMenuPostFx* m_pPostEffects = nullptr;
};
constexpr size_t MAX_SAFE_STRING_LENGTH = 1024; // Fetch this from a file if applicable. Would be nicer <3 
inline bool IsStringEmptyOrNull(const char* String) {
	return String == nullptr || strnlen_s(String, MAX_SAFE_STRING_LENGTH) == 0;
}
inline char* CopySafeStr(const char* src, size_t maxLen = MAX_SAFE_STRING_LENGTH) {
	if (IsStringEmptyOrNull(src)) return "";
	size_t size = strnlen_s(src, maxLen);
	char* result = new char[size + 1] {};
	strncpy_s(result, size + 1, src, maxLen);
	return result;
}
class CPauseMenuHeader {
public:
	struct sPauseMenuHeaderInfo {
		char* m_HeaderString = nullptr;
		int m_HudColourBody, m_HudColourTab;
	};
	struct CoronaInformation {
		char* m_TitleInfo = nullptr;
		char* m_Description = nullptr;
	};
	struct HeaderInformation {
		//Could take a texture dict here to make sure that we know what we are doing but idrc about setting picture right now lmao
		char* NameLine;
		char* DateLine; // omg weird show reference !?!?!
		char* CashLine;
		bool m_bHeaderInfo;
	};
	static constexpr size_t cMaxHeaderLength = 64;
	CPauseMenuHeader(CPauseMenuBase*& rbase, int& rmenumax) : m_rMenuMax(rmenumax), m_rBase(rbase) {
		this->m_vpHeaderInfo.resize(m_rMenuMax, nullptr);
	} // values must exist before we get here. 
	static constexpr size_t DESCRIPTION_MAX = MAX_SAFE_STRING_LENGTH * 2;
	void SetTitleAndDesc(const char* Title, const char* Desc) {
		if (IsStringEmptyOrNull(Title) && IsStringEmptyOrNull(Desc)) {
			if (m_pCoronaInfo) {
				DestroyCorona();
			}
			return; // theres no point in continuing all the shit is empty anyways. 
		}
		if (!m_pCoronaInfo) {
			m_pCoronaInfo = new CoronaInformation();
		}
		delete[] this->m_pCoronaInfo->m_TitleInfo;
		this->m_pCoronaInfo->m_TitleInfo = CopySafeStr(Title);
		delete[] this->m_pCoronaInfo->m_Description;
		this->m_pCoronaInfo->m_Description = CopySafeStr(Desc, DESCRIPTION_MAX);

	}
	void SetHeaderInfo(const char* FirstLine, const char* SecondLine, const char* ThirdLine, bool fHeaderSomething) {
		if (IsStringEmptyOrNull(FirstLine) && IsStringEmptyOrNull(SecondLine) && IsStringEmptyOrNull(ThirdLine)) {
			if (m_pHeaderInfo) {
				DestroyHeader();
			}
			return; // theres no point in continuing all the shit is empty anyways. 
		}
		if (!m_pHeaderInfo) {
			m_pHeaderInfo = new HeaderInformation();
		}
		if (!IsStringEmptyOrNull(FirstLine)) {
			delete[] this->m_pHeaderInfo->NameLine;
			this->m_pHeaderInfo->NameLine = CopySafeStr(FirstLine);
		}
		if (!IsStringEmptyOrNull(SecondLine)) {
			delete[] this->m_pHeaderInfo->DateLine;
			this->m_pHeaderInfo->DateLine = CopySafeStr(SecondLine);
		}
		if (!IsStringEmptyOrNull(ThirdLine)) {
			delete[] this->m_pHeaderInfo->CashLine;
			this->m_pHeaderInfo->CashLine = CopySafeStr(ThirdLine);
		}
		m_pHeaderInfo->m_bHeaderInfo = fHeaderSomething; // i do NOT know what this boolean controls. @TODO: Test m_bHeaderInfo/HeaderInfoString;
	}
	void SetString(int headerItemIdx, char* str) {
		LAGInterface::Writeln(__FUNCTION__"");
		auto* pData = GetOrCreate(headerItemIdx);
		LAGInterface::Writeln(__FUNCTION__"");
		if (headerItemIdx < 0 || headerItemIdx >= m_rMenuMax || !str || !pData) return;
		delete[] pData->m_HeaderString;
		pData->m_HeaderString = new char[cMaxHeaderLength + 1] {};
		strncpy_s(pData->m_HeaderString, cMaxHeaderLength + 1, str, cMaxHeaderLength); // hate string operations lmao.!
		return;
	}
	void SetBodyColour(int headerItemIdx, int Col) {
		auto* pData = GetOrCreate(headerItemIdx);
		if (headerItemIdx < 0 || headerItemIdx >= m_rMenuMax || !pData) return;
		pData->m_HudColourBody = Col;
		return;
	}
	void SetStripColour(int headerItemIdx, int Col) {
		auto* pData = GetOrCreate(headerItemIdx);
		if (headerItemIdx < 0 || headerItemIdx >= m_rMenuMax || !pData) return;
		pData->m_HudColourTab = Col;
		return;
	}
	sPauseMenuHeaderInfo*& GetItem(int idx) {

	}
	void Update() { // we know that the pause menu must be open before we get here	
		if (!m_rBase) return;
		if (m_pCoronaInfo) {
			if (!IsStringEmptyOrNull(m_pCoronaInfo->m_Description)) {
				//LAGInterface::Writeln("Test");
				bool KeepStripBars = 1; // we want to keep the strip bars for this specific menu. In the cases of other menus we may or may not want to toggle them. either way its a separate module basically complete from this code. 
				bool BumpTitle = 1;
				m_rBase->ShiftCoronaDescription(BumpTitle, KeepStripBars); // @TODO: Check bools First Truthy moves stuff up second removes strip bars
			}
			bool bArg_Unused = 0;
			bool bArg1_Unused = 0;
			m_rBase->SetHeaderTitle(m_pCoronaInfo->m_TitleInfo, bArg_Unused, m_pCoronaInfo->m_Description, bArg1_Unused); // @TODO: I don't know what these two do?
		}
		if (m_pHeaderInfo) {
			bool captializeName = false;
			m_rBase->SetHeadingDetails(m_pHeaderInfo->NameLine, m_pHeaderInfo->DateLine, m_pHeaderInfo->CashLine, captializeName); // @TODO: Check Bools
		}
	}
	void Destroy() {
		DestroyCustomItems();
	}
	~CPauseMenuHeader() {
		Destroy();
	}
private:
	void DestroyCorona() {
		delete[] m_pCoronaInfo->m_TitleInfo;
		delete[] m_pCoronaInfo->m_Description;
		delete m_pCoronaInfo;
		m_pCoronaInfo = nullptr;
	}
	void DestroyHeader() {
		delete[] m_pHeaderInfo->NameLine;
		delete[] m_pHeaderInfo->DateLine;
		delete[] m_pHeaderInfo->CashLine;
		delete m_pHeaderInfo;
		m_pHeaderInfo = nullptr;
	}
	void DestroyCustomItems() {
		if (m_pCoronaInfo) {
			DestroyCorona();
		}
		if (m_pHeaderInfo) {
			DestroyHeader();
		}
		for (auto* it : m_vpHeaderInfo) {
			delete it;
		}
		m_vpHeaderInfo.clear();
	}
	sPauseMenuHeaderInfo* GetOrCreate(int headerItemIdx) {
		if (this == nullptr) {
			LAGInterface::Writeln("Help");
			return nullptr;
		}
		if (headerItemIdx < 0 || headerItemIdx >= m_rMenuMax) return nullptr;
		sPauseMenuHeaderInfo* dataref = m_vpHeaderInfo[headerItemIdx];
		LAGInterface::Writeln("Set String");
		if (!dataref) dataref = new sPauseMenuHeaderInfo();
		sPauseMenuHeaderInfo* data = dataref;
		return data;
	}
	CoronaInformation* m_pCoronaInfo = nullptr; // can be optional lmao
	HeaderInformation* m_pHeaderInfo = nullptr;
	CPauseMenuBase*& m_rBase;
	int& m_rMenuMax;
	std::vector<sPauseMenuHeaderInfo*> m_vpHeaderInfo;
};
class CPauseMenuController {
	// should have header items basically represents the entire state of the current pause menu.
	// also move to a new super object like CMod and make it have its own skeleton and specify we want custom header logic.
public:
	enum ePauseMenuVersion {
		SP,
		MP, // These are the only two we support. Other front end menus will get their own classes.
	};
	CPauseMenuController(ePauseMenuVersion ePMV) : m_iMenuMax(ePMV == SP ? 10 : 8), m_pHeader(new CPauseMenuHeader(m_pBase, m_iMenuMax)) {

	}
	CPauseMenuHeader*& GetHeader() {
		return this->m_pHeader; // Do not delete this pointer lmao
	}
	void ActivateMenu() {
		this->m_pBase->ActivateMenu();
	}
	void CloseMenu() {
		this->m_pBase->CloseMenu();
	}
	void Update() { // when we get here we don't care how we activate the menu. But we do want to disable control actions for base menus
		m_pHeader->Update();
	}
	~CPauseMenuController() {
		delete m_pHeader; // depends on m_pBase
		m_pHeader = nullptr;
		delete m_pBase;
		m_pBase = nullptr;
	}
private:
	int m_iMenuMax = 0;
	CPauseMenuHeader* m_pHeader = nullptr;
	//std::vector<sPauseMenuHeaderInfo*> m_Header; // 10 items mp 8 items sp?
	CPauseMenuBase* m_pBase = new CPauseMenuBase();
};
class CPauseMenu {
public:
	enum MenuChoice {
		MP,
		SP,
	};
	static inline void Activate(MenuChoice h) {
		CPauseMenu::ActiveMenu = CPauseMenu::GetMenuForChoice(h);
		HUD::ACTIVATE_FRONTEND_MENU(CPauseMenu::ActiveMenu, 0, -1); // we don't start out with anything lmao.
		m_pPauseMenuController = new CPauseMenuController(h == MP ? CPauseMenuController::MP : CPauseMenuController::SP);
		m_pPauseMenuController->ActivateMenu();
		m_bIsFrontendActive = true;
	}
	static inline CPauseMenuController*& GetMenuController() {
		return CPauseMenu::m_pPauseMenuController;
	}
	static inline void Update() {
		if (m_pPauseMenuController && IsGameFrontendActive()) m_pPauseMenuController->Update();
	}
	static inline void CloseMenu() {
		m_pPauseMenuController->CloseMenu();
		m_bIsFrontendActive = false;
	}
	static inline void Shutdown() {
		if (m_pPauseMenuController) {
			delete m_pPauseMenuController;
		}
	}
	static inline bool IsFrontendActive() {
		return m_bIsFrontendActive;
	}
	static inline bool IsGameFrontendActive() {
		return HUD::IS_PAUSE_MENU_ACTIVE();
	}
	static inline bool IsPauseMenuInInteriorMode() {
		return HUD::IS_PAUSEMAP_IN_INTERIOR_MODE();
	}
private:
	static inline Hash GetMenuForChoice(MenuChoice c) {
		if (c == MP) {
			return MISC::GET_HASH_KEY("FE_MENU_VERSION_MP_PAUSE");
		}
		else {
			return MISC::GET_HASH_KEY("FE_MENU_VERSION_SP_PAUSE");
		}
	}
	static inline Hash ActiveMenu = 0x0;
	static inline CPauseMenuController* m_pPauseMenuController = nullptr;
	static inline bool m_bIsFrontendActive = false;
};

class CGamemodeHud {
	bool m_bIsFogOfWarHidden = true;
	int m_iCurrentCharacterColor = 0;
	void DisablePauseMenuActivation() {
		PAD::DISABLE_CONTROL_ACTION(2, 199, 0); // Disable Pause Control actions
		PAD::DISABLE_CONTROL_ACTION(2, 200, 0);
	}
	void ReplaceCharacterColour(int m_FreemodeColor = 116) {
		HUD::REPLACE_HUD_COLOUR(143, m_FreemodeColor);
		HUD::REPLACE_HUD_COLOUR(144, m_FreemodeColor);
		HUD::REPLACE_HUD_COLOUR(145, m_FreemodeColor); // we replace all hud colours because there isn't a good way to not do it and when the player dies sometimes the hud colour changes when the game's scripts are canceled. 
		//I wonder if we didn't cancel MAIN_PERSISTENT if it would allow us some more leeway. 
		m_iCurrentCharacterColor = m_FreemodeColor;
	}
	std::vector<int> m_Blips;
public:
	void ShowChip(int chip) {
		if (GRAPHICS::BEGIN_SCALEFORM_SCRIPT_HUD_MOVIE_METHOD(21, "SET_PLAYER_CHIPS")) {
			GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(0);
			GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(1);
		}
	}
	CGamemodeHud(int HudColour = 116) {
		//for (int i = 0; i < 20; i++) {
		//	HUD::SET_MINIMAP_COMPONENT(i, true, -1);
		//}
		//HUD::USE_FAKE_MP_CASH(1);
		ReplaceCharacterColour(HudColour); // BLACK (funzies)
		HUD::SET_PLAYER_ICON_COLOUR(37);

		//for (int i = 0; i < 20; i++) {
		//	Blip b = HUD::ADD_BLIP_FOR_COORD(MISC::GET_RANDOM_INT_IN_RANGE(-1000, 1000), MISC::GET_RANDOM_INT_IN_RANGE(-1000, 1000), MISC::GET_RANDOM_INT_IN_RANGE(-1000, 1000));
		//	HUD::SET_BLIP_AS_MISSION_CREATOR_BLIP(b, 1);
		//	HUD::SET_BLIP_SPRITE(b, 304);
		//	HUD::BEGIN_TEXT_COMMAND_SET_BLIP_NAME("STRING");
		//	HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME("Mission");
		//	HUD::END_TEXT_COMMAND_SET_BLIP_NAME(b);
		//	LAGInterface::Writeln("[BlipID:%d]%d", b, HUD::IS_MISSION_CREATOR_BLIP(b));
		//	m_Blips.push_back(b);
		//}
		//this->MoneyScaleform = GRAPHICS::REQUEST_SCALEFORM_MOVIE("HUD_MP_CASH");
		//this->m_MoneyScaleform = new CScaleform(this->MoneyScaleform);

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
		HUD::HIDE_HUD_COMPONENT_THIS_FRAME(4);
		HUD::HIDE_HUD_COMPONENT_THIS_FRAME(5);
		HUD::HIDE_HUD_COMPONENT_THIS_FRAME(13);
		HUD::HIDE_HUD_COMPONENT_THIS_FRAME(3);
		if (!GRAPHICS::HAS_SCALEFORM_SCRIPT_HUD_MOVIE_LOADED(21) && !GRAPHICS::HAS_SCALEFORM_SCRIPT_HUD_MOVIE_LOADED(22)) { // HUD_CHIPS AND HUD_CHIPS_CHANGE
			GRAPHICS::REQUEST_SCALEFORM_SCRIPT_HUD_MOVIE(21);
			WHILE(!GRAPHICS::HAS_SCALEFORM_SCRIPT_HUD_MOVIE_LOADED(21));
			GRAPHICS::REQUEST_SCALEFORM_SCRIPT_HUD_MOVIE(22);
			WHILE(!GRAPHICS::HAS_SCALEFORM_SCRIPT_HUD_MOVIE_LOADED(22));
		}

		GRAPHICS::BEGIN_SCALEFORM_SCRIPT_HUD_MOVIE_METHOD(21, "SET_PLAYER_MP_CASH_WITH_STRING");
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING("");
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(1);
		GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
		//STATS::STAT_SET_INT(MISC::GET_HASH_KEY("SP0_"));
		if (IsKeyJustUp(VK_F13)) {
			ShowChip(4000);
		}
		const char* Cash = "0";
		static constexpr float WEAPON_DISPLACEMENT = 0.0012f;
		{
			//PLAYER::SET_SPECIAL_ABILITY_MP(PLAYER::PLAYER_ID(), 0, 0); //-- I don't think these natives work. Special Abilities are tied to the character model I think
			//PLAYER::SPECIAL_ABILITY_FILL_METER(PLAYER::PLAYER_ID(), 1, 0);
			HUD::SET_MINIMAP_HIDE_FOW(m_bIsFogOfWarHidden); // Disable Hud Fog.
			DisablePauseMenuActivation();
			if (IsPauseMenuAccessed() && !CPauseMenu::IsGameFrontendActive() && (MISC::UPDATE_ONSCREEN_KEYBOARD() != 0)) { // check uokb() != 0 bc we don't want to open whilst typing this frame.
				CPauseMenu::Activate(CPauseMenu::SP); // Multiplayer_Pause give me a minute god damn vs
				CPauseMenu::GetMenuController()->GetHeader()->SetTitleAndDesc("Title", "Desc"); // fetch these from CGamemode::GetModeName() && CGamemode::GetModeDescription(); but more likely just CGamemode::GetModeName();
				CPauseMenu::GetMenuController()->GetHeader()->SetHeaderInfo(PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID()), "SATURDAY 00:00", "$2,147,000,800", true); // @todo update timer
			}
			//LAGInterface::Writeln("AFTER FOW && DISABLE BUTTONS");
			//if (IsKeyJustUp(VK_F13)) {
			//	int sclIndex = GRAPHICS::REQUEST_SCALEFORM_MOVIE("pause_menu_header");
			//	WHILE(!GRAPHICS::HAS_SCALEFORM_MOVIE_LOADED(sclIndex));
			//	CScaleform scaleform = CScaleform(sclIndex);
			//	scaleform.CallScaleform("REMOVE_MENU");
			//	scaleform.CallScaleform("BUILD_MENU", 0,1,2,3);
			//	scaleform.CallScaleform("SET_HEADER_TITLE", "TEST", 1, "TEST", 1);
			//	bool m_bTest = true;
			//	WHILE(m_bTest) {
			//		GRAPHICS::DRAW_SCALEFORM_MOVIE(scaleform.GetHandle(), 0.5f, 0.25f, 0.5f, 0.15, 255, 255, 255, 255, 255);
			//		if (IsKeyJustUp(VK_F13)) {
			//			m_bTest = false;
			//		}
			//	}
			//}
			//if (IsKeyJustUp(VK_F10)) {
			//	if (IsKeyDown(VK_CONTROL)) {
			//		TestVal -= 1;
			//		LAGInterface::Writeln("TestVal == %d", TestVal);
			//		return;
			//	}
			//	TestVal += 1;
			//	LAGInterface::Writeln("TestVal == %d", TestVal);
			//}
			//HUD::SET_RADAR_ZOOM(0);
			if (CPauseMenu::IsGameFrontendActive()) {
				//if (IsKeyJustUp(VK_F13)) {
				//	int amtOfBlipsAreCreator = 0;
				//	for (auto& b : m_Blips) {
				//		if (!HUD::IS_MISSION_CREATOR_BLIP(b)) {
				//			HUD::SET_BLIP_AS_MISSION_CREATOR_BLIP(b, true);
				//			continue;
				//		}
				//		LAGInterface::Writeln("Blip: %d is a creator blip");
				//		amtOfBlipsAreCreator++;
				//	}
				//	LAGInterface::Writeln("AmtOfBlipsCreator: %d", amtOfBlipsAreCreator);
				//}
				//if (MISC::GET_GAME_TIMER() >= LastColumnUpdate + 100) {
				//	m_bColumnTitleneedsUpdate = true;
				//}
				//if (this->m_bColumnTitleneedsUpdate) {
				//	LastColumnUpdate = MISC::GET_GAME_TIMER();
				//	char* selectedCharacter = 0;
				//	int selectedIndex = 0;
				//	bool bIsCaptial = 0;
				//	selectedIndex = MISC::GET_RANDOM_INT_IN_RANGE(0, strnlen_s(ColumnTitle, 64) - 1); // idk if it includes the null character?
				//	selectedCharacter = &ColumnTitle[selectedIndex];
				//	bIsCaptial = std::isupper(*selectedCharacter);
				//	if (bIsCaptial) {
				//		*selectedCharacter = std::tolower(*selectedCharacter);
				//	}
				//	else {
				//		*selectedCharacter = std::toupper(*selectedCharacter);
				//	}
				//	column1Colour = MISC::GET_RANDOM_INT_IN_RANGE(0, 116);
				//	column2Colour = MISC::GET_RANDOM_INT_IN_RANGE(0, 116);
				//	column3Colour = MISC::GET_RANDOM_INT_IN_RANGE(0, 116);
				//	m_bColumnTitleneedsUpdate = false;
				//	//return;
				//}
				//Details Card
				if (HUD::HAS_MENU_LAYOUT_CHANGED_EVENT_OCCURRED() || HUD::HAS_MENU_TRIGGER_EVENT_OCCURRED()) {
					HUD::GET_MENU_LAYOUT_CHANGED_EVENT_DETAILS(&this->lastItemMenuId,
						&this->selectedItemMenuId,
						&this->selectedItemUniqueId);
					LAGInterface::Writeln("[%d, %d, %d]", this->lastItemMenuId, this->selectedItemMenuId, this->selectedItemUniqueId);
					HUD::GET_MENU_TRIGGER_EVENT_DETAILS(&this->lastItemMenuId, &selectedItemUniqueId);
					LAGInterface::Writeln(" %d, %d]", this->lastItemMenuId, this->selectedItemUniqueId); // Last Menu Id Tells me where we are in the menu. 
					LAGInterface::Writeln("%d, %d", HUD::PAUSE_MENU_GET_MOUSE_HOVER_INDEX(), HUD::PAUSE_MENU_GET_MOUSE_HOVER_UNIQUE_ID());
				}
				if (CPauseMenu::IsPauseMenuInInteriorMode()) {
					HUD::HIDE_MINIMAP_EXTERIOR_MAP_THIS_FRAME();
				}
				if (!CPauseMenu::IsPauseMenuInInteriorMode()) {
					HUD::HIDE_MINIMAP_INTERIOR_MAP_THIS_FRAME();
				}
				CPauseMenu::Update();
				PauseMenuNeedsFadeOut = true;
				return;
			}
			//LAGInterface::Writeln("After main pause menu blk");
			if (PauseMenuNeedsFadeOut) {
				CPauseMenu::CloseMenu();
				PauseMenuNeedsFadeOut = false;
				return;
			}
			//HUD::HIDE_MINIMAP_EXTERIOR_MAP_THIS_FRAME();
			//HUD::SET_RADAR_AS_INTERIOR_THIS_FRAME()
			if ((HUD::IS_HUD_COMPONENT_ACTIVE(19) && !PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0)) || HUD::IS_HUD_COMPONENT_ACTIVE(16)) { // this actually does work to get the weapon wheels state. Just gotta find pfx
				//LAGInterface::Writeln("Wheel or Radio active");
				if (!GRAPHICS::ANIMPOSTFX_IS_RUNNING("SwitchHUDIn")) {
					GRAPHICS::ANIMPOSTFX_STOP("SwitchHUDFranklinIn");
					GRAPHICS::ANIMPOSTFX_STOP("SwitchHUDMichaelIn");
					GRAPHICS::ANIMPOSTFX_STOP("SwitchHUDTrevorIn");
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
		}
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
class CSelector {
public:
	enum class eSlotIndex {
		FRANKLIN = 0,
		TREVOR = 1,
		ONLINE = 2,
		MICHAEL = 3
	};
	enum class eCharacters {
		MICHAEL, TREVOR, FRANKLIN, ONLINE
	};
	enum eSelectorState {
		UNDEFINED,
		AVAILABLE,
		UNAVAILABLE,
		NOT_MET
	};
	void Init() {
		m_Scaleform = CScaleformManager::Get()->GetScaleform("PLAYER_SWITCH");
		if (!m_Scaleform) {
			LAGInterface::Writeln("Scaleform not found!");
		}
	}
	void Update() {
		m_Scaleform->CallScaleform("SET_SWITCH_VISIBLE", false);
		m_Scaleform->CallScaleform("SET_PLAYER_SELECTED", eSlotIndex::FRANKLIN);
		m_Scaleform->CallScaleform("SET_SWITCH_SLOT", eSlotIndex::FRANKLIN, AVAILABLE, eCharacters::FRANKLIN, true);
		m_Scaleform->CallScaleform("SET_SWITCH_SLOT", eSlotIndex::TREVOR, AVAILABLE, eCharacters::TREVOR, false);
		m_Scaleform->CallScaleform("SET_SWITCH_SLOT", eSlotIndex::ONLINE, UNDEFINED, eCharacters::ONLINE, false);
		m_Scaleform->CallScaleform("SET_SWITCH_SLOT", eSlotIndex::MICHAEL, AVAILABLE, eCharacters::MICHAEL, false);
		m_Scaleform->CallScaleform("SET_MP_LABEL", "24 HOURS");
		m_Scaleform->CallScaleform("SET_SWITCH_HINTED_ALL", false, false, false, false);
		m_Scaleform->CallScaleform("SET_SWITCH_COUNTER_ALL", 0, 0, 0, 0);
		m_Scaleform->CallScaleform("SET_SWITCH_VISIBLE", true);
		GRAPHICS::DRAW_SCALEFORM_MOVIE(m_Scaleform->GetHandle(), 0.9346683f, 0.913f, 0.1306634f, 0.175f, 255, 255, 255, 255, 0);
	}
	void Shutdown() {
		if (m_Scaleform) {
			CScaleformManager::Get()->Release("PLAYER_SWITCH");
		}
	}
private:
	CScaleform* m_Scaleform;
};
class CGM_ScriptMgr { // Game Scripts
public:
	struct sScriptInfo {
		char* scriptName;
		bool m_bIsSHV = false;
	};
	CGM_ScriptMgr() {
		PopulateScripts();
	}
	int SummonScript(const char* scriptName, int scriptStackSize) {
		SCRIPT::REQUEST_SCRIPT(scriptName);
		int m_Attempts = 0;
		static constexpr int MAX_SCRIPT_ATTEMPTS = 10;
		WHILE(!SCRIPT::HAS_SCRIPT_LOADED(scriptName)) {
			m_Attempts++;
			if (m_Attempts >= MAX_SCRIPT_ATTEMPTS) {
				LAGInterface::Writeln("[CGM_ScriptMgr] Script: %s is invalid", scriptName);
				break; // fuhhh
			}
		}
		int threadId = SYSTEM::START_NEW_SCRIPT(scriptName, scriptStackSize);
		LAGInterface::Writeln("[CGM_ScriptMgr] Script: %s is valid pushing threadidx: %d", scriptName, threadId);

		//check threadid != invalid thread
		m_Scripts.insert({ threadId, {CopySafeStr(scriptName), GetIfScriptIsSHV(MISC::GET_HASH_KEY(scriptName))} }); // @todo: check
	}
	bool m_bForceScriptCheck = false;
	void Update() {
		if (MISC::GET_GAME_TIMER() >= LastScriptCheck || LastScriptCheck == 0 || m_bForceScriptCheck) {
			DestroyList();
			PopulateScripts();
			LAGInterface::Writeln("[CGM_ScriptMgr] Checking Scripts");
			for (auto& m : m_Scripts) {
				LAGInterface::Writeln("[CGM_ScriptMgr] %d { %s, %d}", m.first, m.second.scriptName, m.second.m_bIsSHV);
			}
			m_bForceScriptCheck = false;
			LastScriptCheck = MISC::GET_GAME_TIMER() + 60'000; // 60 seconds/1 minute update
		}
	}
	void TerminateThread(int threadId) {

	}
private:
	void DestroyItem(sScriptInfo& script) {
		delete[] script.scriptName;
		script.scriptName = nullptr;
	}
	void DestroyList() {
		for (auto& m : m_Scripts) {
			DestroyItem(m.second);
		}
		m_Scripts.clear();

	}
	int LastScriptCheck = 0;
	bool GetIfScriptIsSHV(Hash hash) {
		return hash == MISC::GET_HASH_KEY(SCRIPT::GET_THIS_SCRIPT_NAME());
	}
	void PopulateScripts() {
		SCRIPT::SCRIPT_THREAD_ITERATOR_RESET();
		int scriptThreadIterator = SCRIPT::SCRIPT_THREAD_ITERATOR_GET_NEXT_THREAD_ID();
		WHILE(SCRIPT::IS_THREAD_ACTIVE(scriptThreadIterator)) {
			char* scriptname = CopySafeStr(SCRIPT::GET_NAME_OF_SCRIPT_WITH_THIS_ID(scriptThreadIterator));
			bool isShv = false;
			if (strncmp(scriptname, SCRIPT::GET_THIS_SCRIPT_NAME(), MAX_SAFE_STRING_LENGTH) == 0) { // this is unlikely we could force a compiler optimization if C++20
				isShv = true;
			}
			LAGInterface::Writeln("[CGM_ScriptMgr] Debug Script: %d:%s", scriptThreadIterator, scriptname);
			m_Scripts.insert({ scriptThreadIterator,  {scriptname, isShv} }); // I think this is fine? 
			scriptThreadIterator = SCRIPT::SCRIPT_THREAD_ITERATOR_GET_NEXT_THREAD_ID(); // we have thread id.
		}
	}
	std::unordered_map<int, sScriptInfo> m_Scripts;
};
enum eStackSize : int {
	MICRO = 128,
	MINI = 512,
	DEFAULT = 1424,
	SPECIAL_ABILITY = 1828,
	FRIEND = 2050,
	SHOP = 2324,
	CELLPHONE = 2552,
	VEHICLE_SPAWN = 3568,
	CAR_MOD_SHOP = 3750,
	PAUSE_MENU_SCRIPT = 3076,
	APP_INTERNET = 4592,
	MULTIPLAYER_MISSION = 5050,
	CONTACTS_APP = 4000,
	INTERACTION_MENU = 9800,
	SCRIPT_XML = 8344,
	PROPERTY_INT = 19400,
	ACTIVITY_CREATOR_INT = 15900,
	SMPL_INTERIOR = 2512,
	WAREHOUSE = 14100,
	IE_DELIVERY = 2324,
	SHOP_CONTROLLER = 3800,
	AM_MP_YACHT = 5000,
	INGAMEHUD = 4600,
	TRANSITION = 8032,
	FMMC_LAUNCHER = 27000,
	MULTIPLAYER_FREEMODE = 85000,
	MISSION = 62500,
	MP_LAUNCH_SCRIPT = 34750
};
struct EnterSequence {
	EnterSequence(const std::string& dict, const std::string& name, CVector3 sceneStart, float fRot) : m_AnimDictionary(dict), m_AnimName(name), m_SceneStart(sceneStart), m_fSequenceRotation(fRot){
		STREAMING::REQUEST_ANIM_DICT(dict.c_str());
		WHILE(!STREAMING::HAS_ANIM_DICT_LOADED(dict.c_str()));
		
	}
	~EnterSequence() {
		STREAMING::REMOVE_ANIM_DICT(m_AnimDictionary.c_str());
	}
	void BeginScene() {
		m_SceneHandle = PED::CREATE_SYNCHRONIZED_SCENE(m_SceneStart.GetX(), m_SceneStart.GetY(), m_SceneStart.GetZ(), 0, 0, m_fSequenceRotation, 0);
	}
	//Animation Data
	std::string m_AnimDictionary, m_AnimName;
	CVector3 m_SceneStart;
	float m_fSequenceRotation;
	int m_SceneHandle;
	// ** Cam Data. ** 
};
class CScriptUtils {
public:
	static CVector3 PlayerCoords() {
		return ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
	}
	static float PlayerHeading() {
		return ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID());
	}
private:

};
class CCarWashProperty {
public:
	CCarWashProperty() {}
	CCarWashProperty(const CCarWashProperty&) = delete;
	CCarWashProperty& operator=(const CCarWashProperty&) = delete;
	void Update() {
		if (m_DoorSequence) {
			if (GetSyncSceneProgression() >= 0.65f) {
				delete m_DoorSequence;
				m_DoorSequence = nullptr;
				EndEntranceGarage();
			}
			return;
		}
		DrawFreemodeMarker(m_GarageLocation);
		DrawFreemodeMarker(m_DoorLocation);
		if (m_GarageLocation.DistCustomZ(CScriptUtils::PlayerCoords(), 1.1) <= 1.3) {
			BeginEntranceSequenceGarage();
			return;
		}
	}
	~CCarWashProperty() {
		if(m_DoorSequence){
			delete m_DoorSequence;
			m_DoorSequence = nullptr;
		}
	}
private:
	void BeginEntranceSequenceGarage() {
		const char* dict = "anim@apt_trans@garage";
		const char* anim = "gar_open_3_left";
		CVector3 m_SyncStart = { 10.244, -1405.610, 28.281797 };
		float fRot = 50.f;
		m_pClone = PLAYER::PLAYER_PED_ID();
		m_DoorSequence = new EnterSequence(dict, anim, m_SyncStart, fRot);
		m_DoorSequence->BeginScene();
		TASK::TASK_SYNCHRONIZED_SCENE(m_pClone, m_DoorSequence->m_SceneHandle, dict, anim, 1000.0, -8.0, 4, 0, 0x447a0000, 0);
		ENTITY::SET_ENTITY_COMPLETELY_DISABLE_COLLISION(m_pClone, 0, 0);
		//c = CAM::CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", 1);
		//
		//	0.954014, -1402.701050, 31.967506
		//	-15.685034, 0.000000, -107.779564
		//	45.000000
		//
		//CAM::SHAKE_CAM(c, "HAND_SHAKE", 1.0f);
		//CAM::SET_CAM_COORD(c, 0.954014, -1402.701050, 31.967506);
		//CAM::SET_CAM_ROT(c, -15.685034, 0.000000, -107.779564, 2);
		//CAM::SET_CAM_FOV(c, 45.000000);
		//CAM::RENDER_SCRIPT_CAMS(1, 0, 0, 0, 0, 0);
	}
	void EndEntranceGarage() {
		// ped reset
		ENTITY::SET_ENTITY_COMPLETELY_DISABLE_COLLISION(m_pClone, 1, 1);
		ENTITY::SET_ENTITY_HAS_GRAVITY(m_pClone, 1);
		STREAMING::LOAD_SCENE(8.4070, -1399.707, -72.8997);
		CAM::RENDER_SCRIPT_CAMS(0, 0, 0, 0, 0, 0);
		//interior
		Interior interior = INTERIOR::GET_INTERIOR_AT_COORDS(26.07468000, -1398.97900000, -75.00000000);
		INTERIOR::ACTIVATE_INTERIOR_ENTITY_SET(interior, m_InteriorShellEntitySet);
		INTERIOR::REFRESH_INTERIOR(interior);
		//clean interior and prep for enter
		GRAPHICS::REMOVE_DECALS_IN_RANGE(26.07468000, -1398.97900000, -75.00000000, 19);
		TASK::CLEAR_PED_TASKS_IMMEDIATELY(m_pClone);
		// put player inside interior
		ENTITY::SET_ENTITY_COORDS(m_pClone, 8.2346, -1399.715, -75.1, 1, 0, 0, 1);
		ENTITY::SET_ENTITY_HEADING(m_pClone, 270.0);
		CAM::SET_GAMEPLAY_CAM_RELATIVE_PITCH(0.0, 0.0);
		//ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 26.26, -1402.355, -73.9997, 1, 0, 0, 1);
		//walk player
		PLAYER::SIMULATE_PLAYER_INPUT_GAIT(PLAYER::PLAYER_ID(), 1.0, 1000, 1.0, 1, 0, 0);
		if (m_pClone != PLAYER::PLAYER_PED_ID()) {
			PED::DELETE_PED(&m_pClone);
		}
	}
	int GetSyncSceneProgression() {
		return PED::GET_SYNCHRONIZED_SCENE_PHASE(this->m_DoorSequence->m_SceneHandle);
	}
	Ped m_pClone =0;
	const char* m_InteriorShellEntitySet = "aanthologies.burlesque";
	EnterSequence* m_DoorSequence = nullptr;
	CVector3 m_GarageLocation = { 10.22, -1405.5, 28.08 };
	CVector3 m_DoorLocation = { 26.25, -1409.9, 28.08 };
};
class CCarWash : public fwScriptEnv {
private:
	CGM_ScriptMgr m_LocalScriptManager;
	int StartupThreadId = 0;
	int PLAYER_SWITCH_SCALEFORM = 0;
	CSelector* m_Selector = nullptr;
	CCarWashProperty* m_pProperty = nullptr;
public:
	SCRIPT_INIT(CCarWash);
	CCarWash() = default; // this makes references impossible btw. however there isn't a good way around it? unless you want to change Functor to be variadic as well lmao.
	CCarWash(int arg_1, int arg_2) {
		this->m_bInsideInterior = 0;
		m_Selector = new CSelector();
		m_Selector->Init();
		m_pProperty = new CCarWashProperty();
	}

	void OnInit() {
		DevTools::InitClass();
		DLC::ON_ENTER_MP();
		Streamer::Init();
	}
	void OnShutdown() {
		m_Selector->Shutdown();
		delete m_Selector;
		m_Selector = nullptr;
		delete m_pProperty;
		m_pProperty = nullptr;
		DevTools::DestroyClass();
		m_Hud.Shutdown();
		Streamer::Shutdown();
	}

	Ped pClone = 0;
	Cam c = 0;
	bool IsPauseMenuAccessed() {
		return PAD::IS_DISABLED_CONTROL_JUST_RELEASED(2, 199) || PAD::IS_DISABLED_CONTROL_JUST_RELEASED(2, 200);
	}
	bool m_bInsideInterior = false;
	void OnTick() {
		GRAPHICS::RESET_SCRIPT_GFX_ALIGN();
		if (IsKeyJustUp(VK_F13)) {
			m_LocalScriptManager.m_bForceScriptCheck = true;
		}
		m_Selector->Update();
		m_LocalScriptManager.Update();
		m_pProperty->Update();
		DevTools::GetClass()->Update();
		if (DoorLocation.DistCustomZ(CScriptUtils::PlayerCoords(), 1.1) <= 1.3 && !SyncScene) {
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
		if (m_bInsideInterior) {

		}
		m_Hud.Update();
	}
private:
	CGamemodeHud m_Hud = CGamemodeHud(116);
	bool WeaponWheelCheck = false;
	bool PauseMenuNeedsFadeOut = false;
	int SyncScene = 0;
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
		std::array<const char*, 2> strs;
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
		if (IsKeyJustUp(VK_F14) && !m_pCarWash) {
			static int i = 0;
			m_pCarWash = CScriptRuntime::Get()->AddScriptAndCreate<CCarWash>(1, 0); // idk if this would work
			CScriptRuntime::Get()->PrintAll();
		}
		if (IsKeyJustUp(VK_F15) && m_pCarWash) {
			CScriptRuntime::Get()->TerminateScript(m_pCarWash);
			CScriptRuntime::Get()->PrintAll();
			m_pCarWash = nullptr;
		}
		hud->Update();
		//LAGInterface::Writeln("After Hud Update");
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