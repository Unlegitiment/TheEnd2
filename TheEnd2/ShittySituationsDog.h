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
class CScaleform {
public:
	explicit CScaleform(int Handle) :m_Handle(Handle) {} // fuck shit 
	template<typename... T> bool CallScaleform(const char* method, T&&... args) {
		bool bRes = GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(m_Handle, method);
		(Call(std::forward<T>(args)), ...); // this syntax is just so weird. 
		GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
		return bRes;
	}
	template<typename T> void Call(T data) {}
	template<> void Call<int>(int data) {
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(data);
	}
	template<> void Call<const char*>(const char* data) {
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING(data);
	}
	template<> void Call<float>(float data) {
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_FLOAT(data);
	}
	template<> void Call<bool>(bool data) {
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_BOOL(data);
	}
private:
	int m_Handle = 0;
}; 
struct HeistCelebBase {
	HeistCelebBase(const char* base, int _wallId) : method(base) , wallId(_wallId){}
	const char* GetMethod() {
		return method;
	}
	int GetWallId() { return this->wallId; }
private:
	int wallId;
	const char* method;
};
struct HeistCelebRepWall : public HeistCelebBase{
	HeistCelebRepWall(int Wall) : HeistCelebBase("ADD_REP_POINTS_AND_RANK_BAR_TO_WALL", Wall) {}
	void ExeScaleformCommand(CScaleform& scaleform, int wallID) {
		scaleform.CallScaleform(GetMethod(), wallID, );
	}
	int repGained = 0, startRepPoints = 0, MinRepOfRank = 0, MaxRepOfRank = 0, CurrentRank = 0, NextRank = 0;
	char* SmallText = "NICE";
	char* BigText = "RANKUP!"; //on average
};
struct HeistCelebJobPoints : public HeistCelebBase {
	bool xAlign;
	int pointsGained;
};

struct HeistCelebWallStat : public HeistCelebBase{
	HeistCelebWallStat(int Wall) : HeistCelebBase("ADD_STAT_TO_TABLE", Wall) {};
	char* name;
	char* value;
	bool isNameRawText = true;
	bool isValueRawText = true;
	bool isTotalRow = true;
	bool isStatValueTime = false;
	char* colourId; // HUD_COLOURS.
};
struct HeistCelebWallStatTable : public HeistCelebBase {
public:
	HeistCelebWallStatTable(int Wall) : HeistCelebBase("ADD_STAT_TABLE_TO_WALL", Wall){}
	void ExeAllScaleformCommands(CScaleform& scaleform, int wallid, int stepId) {
		for (auto& stat : m_Stats) {
			scaleform.CallScaleform(stat.GetMethod(), wallid, stepId, stat.name, stat.value, stat.isNameRawText, stat.isValueRawText, stat.isTotalRow, stat.isStatValueTime, stat.colourId); // this is expected to be a wall stat thus add_stat_to_table's syntax is used.
		}
	}
private:
	std::vector<HeistCelebWallStat> m_Stats;
};

struct HeistCelebSetup { // background,and other stuff.
	const char* bgColour = "HUD_COLOUR_BLACK";
	const int sfxId = 3; 
	const int alpha = 100;
	const int moneyMesh = 1; // check val
	bool IsScaleformDone = false;
};
struct HeistCelebData{ // how do I have timed events
	HeistCelebSetup m_Base;
	HeistCelebRepWall* RepWall = nullptr;
	void ResetDrawFlag() { m_Base.IsScaleformDone = false; }
};
class CHeistCeleb {
	enum eStateMachine {
		eSM_INVALID,
		eSM_SETUP_DATA,
		eSM_RUN_DATA,
		eSM_CLEANUP,
		eSM_DONE,
		eSM_MAX
	};
	enum eScaleformIndex {
		BASE = 0,
		BACKGROUND = 1,
		FOREGROUND = 2,
		MAX_SCALEFORM
	};
	template<typename... T>
	bool CallScaleform(const char* method, T&&... args) {
		for (auto& s : m_ScaleformInstances) {
			CScaleform scaleform = CScaleform(s);
			scaleform.CallScaleform(method, std::forward<T>(args)...);
		}
		return true;
	}
public:
	//this shit floods the logs. @TODO: remove all debugprint statements pretty please!
	void Update(HeistCelebData* data) {
		if (data == nullptr) {
			LAGInterface::Writeln("data is nullptr! quick return triggered");
			if (m_State != eSM_INVALID) {
				//yikes we got a problem! Scaleform data was just randomly changed to nullptr! this is a problem!
				CallScaleform("CLEANUP", WallId);
				LAGInterface::Writeln("data is nullptr! but our state is not invalid! quick return triggered");
				m_State = eSM_INVALID;
			}
			return;
		}
		if (data->m_Base.IsScaleformDone) {
			LAGInterface::Writeln("data->m_Base.IsScaleformDone is true! quick return triggered");
			return;
		}
		if (m_State == eSM_INVALID) { // this is gonna be rough but I don't really care smh.
			RunDataSetup(data);
			LAGInterface::Writeln("SetupData Triggered");
			DATA = data; // might not be necessary
			m_State = eSM_RUN_DATA;
			return;
		}
		if (m_State == eSM_RUN_DATA) {
			if (MISC::GET_GAME_TIMER() >= m_ScaleformPredictedEndTimer) {
				LAGInterface::Writeln("MISC::GET_GAME_TIMER() >= m_ScaleformPredictedEndTimer is true! setting state to Cleanup!");
				m_State = eSM_CLEANUP;
			}
			LAGInterface::Writeln("RENDER Triggered");

			RenderData();
		}
		if (m_State == eSM_CLEANUP) {
			CallScaleform("CLEANUP", WallId);
			LAGInterface::Writeln("CLEANUP Triggered");
			data->m_Base.IsScaleformDone = true;
			m_State = eSM_DONE;
		}
		if (m_State == eSM_DONE) {
			LAGInterface::Writeln("eSM_DONE Triggered");
			m_State = eSM_INVALID;
		}
	}
private:
	bool m_bisFirstInit = true;
	int m_ScaleformStateTimer = 0;
	int m_ScaleformPredictedEndTimer = 0;
	HeistCelebData* DATA = nullptr;
	void RunDataSetup(const HeistCelebData* data) {
		if (m_bisFirstInit) {
			SetupInstances();
			m_bisFirstInit = false;
		}
		for (auto& scl : m_ScaleformInstances) {
			CScaleform scaleform = CScaleform(scl);
			scaleform.CallScaleform("CREATE_STAT_WALL", WallId, data->m_Base.bgColour, data->m_Base.sfxId);
			scaleform.CallScaleform("ADD_MISSION_RESULT_TO_WALL", WallId, "MISSION", "PASSED", "", true, true, true);
			if (data->RepWall) {
				scaleform.CallScaleform("ADD_REP_POINTS_AND_RANK_BAR_TO_WALL", WallId, data->RepWall->repGained, data->RepWall->startRepPoints, data->RepWall->MinRepOfRank, data->RepWall->MaxRepOfRank, data->RepWall->CurrentRank, data->RepWall->NextRank, data->RepWall->SmallText, data->RepWall->BigText);
			}
			scaleform.CallScaleform("ADD_BACKGROUND_TO_WALL", WallId, data->m_Base.alpha, data->m_Base.moneyMesh);
			scaleform.CallScaleform("SHOW_STAT_WALL", WallId);
		}
		m_ScaleformPredictedEndTimer = MISC::GET_GAME_TIMER() + 9000; // not exactly sure.
	}
	void RenderData() {
		GRAPHICS::DRAW_SCALEFORM_MOVIE_FULLSCREEN_MASKED(m_ScaleformInstances[BACKGROUND], m_ScaleformInstances[FOREGROUND], 255, 255, 255, 255);
		GRAPHICS::DRAW_SCALEFORM_MOVIE_FULLSCREEN(m_ScaleformInstances[BASE], 255, 255, 255, 255, 1); // yokay!
	}
	bool SetupInstances() {
		const char* scl_base = "HEIST_CELEBRATION";
		const char* scl_base_bg = "HEIST_CELEBRATION_BG";
		const char* scl_base_fg = "HEIST_CELEBRATION_FG";
		m_ScaleformInstances[0] = GRAPHICS::REQUEST_SCALEFORM_MOVIE(scl_base);
		m_ScaleformInstances[1] = GRAPHICS::REQUEST_SCALEFORM_MOVIE(scl_base_bg);
		m_ScaleformInstances[2] = GRAPHICS::REQUEST_SCALEFORM_MOVIE(scl_base_fg);
		WHILE(!GRAPHICS::HAS_SCALEFORM_MOVIE_LOADED(m_ScaleformInstances[0])) {
			m_ScaleformInstances[0] = GRAPHICS::REQUEST_SCALEFORM_MOVIE(scl_base);
		}
		WHILE(!GRAPHICS::HAS_SCALEFORM_MOVIE_LOADED(m_ScaleformInstances[1])) {
			m_ScaleformInstances[1] = GRAPHICS::REQUEST_SCALEFORM_MOVIE(scl_base_bg); // condense but for now it works
		}
		WHILE(!GRAPHICS::HAS_SCALEFORM_MOVIE_LOADED(m_ScaleformInstances[2])) {
			m_ScaleformInstances[2] = GRAPHICS::REQUEST_SCALEFORM_MOVIE(scl_base_fg);
		}
		return true;
	}
	int WallId = 0; // 0 is a valid WallId. Ideally we'd run some sort of check so we could have multiple walls but I don't know if thats possible in this case.
	std::array<int, 3> m_ScaleformInstances;
	HeistCelebData* m_Data = nullptr;
	eStateMachine m_State;
};
class CHud {
public:
	void SetHeistData(HeistCelebData* data) {
		this->Data = data;
	}
	void Update() {
		celeb->Update(Data); 
	}
private:
	CHeistCeleb* celeb = new CHeistCeleb();
	HeistCelebData* Data = nullptr;
};
class CTest : public ScriptableBase<CTest> {
private:
	Hash DominatorHash = MISC::GET_HASH_KEY("DOMINATOR");
	Vehicle Dominator = 0;
	bool m_bIsInvincible = false;
	int SCL = 0, SCL_BG = 0, SCL_FG = 0;
	int Scaleforms[3] = { 0 };
	bool isSceneRunning = false;
	HeistCelebData data;
	HeistCelebRepWall repWall;
	CHud hud;

	template< typename T >
	std::string int_to_hex(T i)
	{
		std::stringstream stream;
		stream << "0x"
			<< std::setfill('0') << std::setw(sizeof(T) * 2)
			<< std::hex << i;
		return stream.str();
	}
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
public:
	// Inherited via ScriptableBase
	float fRadius = 19.f;
	void OnTick() override
	{
		//if (IsKeyJustUp(VK_F13)) {
		//	if (IsKeyDown(VK_CONTROL)) {
		//		fRadius -= 1.0f;
		//		LAGInterface::Writeln("Radius: %f", fRadius);
		//	}
		//	else {
		//		fRadius += 1.0f;
		//		LAGInterface::Writeln("Radius: %f", fRadius);
		//	}
		//}
		if (IsKeyJustUp(VK_F14)) {
			//TestFunc();
			const std::vector<const char*> Strings = { "aanthologies.burlesque", // all of these are the shell props for the game. just required a little bit of cleanup from collision checker.
				"acallouses_inebriations",
				"aguffhuffiness",
				"amummersspousals",
				"aintracranialbabushkas",
				"aphotoengravings_reexplaining",
				"abaptizerdebs" };
			bool m_bTestStrings = true;
			Interior interior = INTERIOR::GET_INTERIOR_AT_COORDS(26.07468000, -1398.97900000, -75.00000000);
			if (INTERIOR::GET_INTERIOR_FROM_ENTITY(PLAYER::PLAYER_PED_ID() ) != interior) {
				ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 26.07468000, -1398.97900000, -74.00000000, 1, 0, 0, 1);
				GRAPHICS::REMOVE_DECALS_IN_RANGE(26.07468000, -1398.97900000, -75.00000000, 19);
			}
			WHILE(m_bTestStrings) {
				auto it = Strings.begin();
				bool needsUpdate = true;
				WHILE (it != Strings.end()) {
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
		if (m_bIsInvincible) {
			HUD::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
			HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME("Player is Invulnerable");
			HUD::END_TEXT_COMMAND_DISPLAY_TEXT(0.2, 0.1, 1);
		}
		if (IsKeyJustUp(VK_F15)) {
			if (data.m_Base.IsScaleformDone) data.ResetDrawFlag();
			data.RepWall = &repWall;
			hud.SetHeistData(&data);
		}
		hud.Update();
		if (IsKeyJustUp(VK_F16)) {

			hud.SetHeistData(nullptr);
		}
	}

	int timeStarted = 0;
	int reqTimer = 0;
	void TestOp() {
		ScaleformInit();
		for (int i = 0; i < 3; i++) {
			int s = Scaleforms[i];
			int WallId = 1;
			CScaleform scaleform = CScaleform(s);
			scaleform.CallScaleform("CLEANUP", WallId);
			scaleform.CallScaleform("CREATE_STAT_WALL", WallId, "HUD_COLOUR_HSHARD", 100); // 100?
			scaleform.CallScaleform("ADD_MISSION_RESULT_TO_WALL", WallId, "OHHH", "HARRY", "", true, true, true);
			int stepId = 1;
			int statTable = 2; // extentions to tables have to be separate in IDs. Like you can have multiple cash animations they have to be different id's though. 
			scaleform.CallScaleform("CREATE_STAT_TABLE", WallId, statTable, 3); // what is 3 sfxId? 
			scaleform.CallScaleform("ADD_STAT_TO_TABLE", WallId, statTable, "GARRY", "", true, true, false, false, "HUD_COLOUR_PLATINUM"); // check flags after value
			scaleform.CallScaleform("ADD_STAT_TO_TABLE", WallId, statTable, "HARRY", "", true, true, false, false, "HUD_COLOUR_SILVER"); // check flags after value
			scaleform.CallScaleform("ADD_STAT_TABLE_TO_WALL", WallId, statTable); // I lose it here?
			//scaleform.CallScaleform("PAUSE", WallId, 5 * 6);
			//scaleform.CallScaleform("ADD_MISSION_RESULT_TO_WALL", WallId, "IM BOUTA", "CUMMMMMM", "", true, true, true);
			scaleform.CallScaleform("CREATE_INCREMENTAL_CASH_ANIMATION", WallId, stepId);
			scaleform.CallScaleform("ADD_INCREMENTAL_CASH_WON_STEP", WallId, stepId, 0, 100000000, "IM BOUTA", "CUUUUUUUUUUUUM", "CAME", 1, 3);
			scaleform.CallScaleform("ADD_INCREMENTAL_CASH_ANIMATION_TO_WALL", WallId, stepId);
			scaleform.CallScaleform("ADD_JOB_POINTS_TO_WALL", WallId, 15, 1); // xAlign could be weird
			scaleform.CallScaleform("ADD_REP_POINTS_AND_RANK_BAR_TO_WALL", WallId, 2000, 0, 0, 2000, 1500, 2000, "NICE!", "CUM!");
			scaleform.CallScaleform("ADD_BACKGROUND_TO_WALL", WallId, 255, 1);
			scaleform.CallScaleform("SHOW_STAT_WALL", WallId);
			scaleform.CallScaleform("createSequence", WallId, 1, 1);
			m_bIsScaleformActive = true;
		}
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
	void OnInit() override
	{
		repWall.CurrentRank = 0;
		repWall.NextRank = 1;
		repWall.MinRepOfRank = 0;
		repWall.MaxRepOfRank = 1000;
		repWall.repGained = 1000;
		repWall.startRepPoints = 0; //im gonna assume this is like the start of where we are?
	}
	void OnShutdown() override
	{
		LAGInterface::Writeln("[CTest] Shutdown called");
		m_bIsInvincible = false;
		ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), false);
	}
};

/* old test cutscene data. 
			//Add commentMore actions
			//	hs4f_drp_off cutscene where lootbag gets dropped off. AFTER FIRST HEIST NOT MADRAZO CUT
			//	hs4f_drp_cel cutscene where player drinks and throws lime off of balcony
			
const char* cutname = "hs4f_drp_cel";
if (!isSceneRunning) {
	CUTSCENE::REQUEST_CUTSCENE(cutname, 8);
	CUTSCENE::START_CUTSCENE(0);
	isSceneRunning = true;
}
//isSceneRunning = CUTSCENE::IS_CUTSCENE_PLAYING();
		}
		if (isSceneRunning) {
			//LAGInterface::Writeln("%d", CUTSCENE::GET_CUTSCENE_TIME());
			if (CUTSCENE::GET_CUTSCENE_TIME() >= 12080 && GRAPHICS::GET_TOGGLE_PAUSED_RENDERPHASES_STATUS()) {
				//LAGInterface::Writeln("Over 12080! %d", CUTSCENE::GET_CUTSCENE_TIME());
				GRAPHICS::TOGGLE_PAUSED_RENDERPHASES(false);
			}
		}
		//GRAPHICS::DRAW_MARKER_SPHERE(26.07468000, -1398.97900000, -75.00000000, fRadius, 255, 0, 0, 0.7);

		isSceneRunning = CUTSCENE::IS_CUTSCENE_PLAYING();
		if (m_bIsScaleformActive) { // gotta start a timer or something 
			GRAPHICS::DRAW_SCALEFORM_MOVIE_FULLSCREEN_MASKED(SCL_BG, SCL_FG, 255, 255, 255, 255);
			GRAPHICS::DRAW_SCALEFORM_MOVIE_FULLSCREEN(SCL, 255, 255, 255, 255, 1);
			HUD::HIDE_HUD_AND_RADAR_THIS_FRAME();
			HUD::HIDE_STREET_AND_CAR_NAMES_THIS_FRAME();
			if (MISC::GET_GAME_TIMER() >= reqTimer) {
				GRAPHICS::SET_SCALEFORM_MOVIE_AS_NO_LONGER_NEEDED(&SCL);
				GRAPHICS::SET_SCALEFORM_MOVIE_AS_NO_LONGER_NEEDED(&SCL_BG);
				GRAPHICS::SET_SCALEFORM_MOVIE_AS_NO_LONGER_NEEDED(&SCL_FG);
				m_bIsScaleformActive = false;
			}

*/