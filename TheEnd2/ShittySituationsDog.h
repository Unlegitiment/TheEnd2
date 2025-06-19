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
struct HeistCelebRepWall {
	int repGained, startRepPoints, MinRepOfRank, MaxRepOfRank, CurrentRank, NextRank;
	char* SmallText = "NICE";
	char* BigText = "RANKUP!"; //on average
};
struct HeistCelebJobWall {
	int points;
	bool xAlign;
};
struct HeistCelebData{ // how do I have timed events
	HeistCelebRepWall* RepWall = nullptr;
};
class CHeistCeleb {
	enum eStateMachine {
		eSM_INVALID,
		eSM_SETUP_DATA,
		eSM_RUN_DATA,
		eSM_CLEANUP,
		eSM_MAX
	};
	enum eScaleformIndex {
		BASE = 0,
		BACKGROUND = 1,
		FOREGROUND = 2
	};
public:
	void Update(HeistCelebData* data) {
		if (data == nullptr) return; // we are null and don't care about progression
		if (m_State == eSM_INVALID) { // this is gonna be rough but I don't really care smh.
			RunDataSetup(data);
			m_State = eSM_RUN_DATA;
		}
		if (m_State == eSM_RUN_DATA) {
			RenderData();
		}
	}
private:
	bool m_bisFirstInit = true;
	int m_ScaleformStateTimer = 0;
	int m_ScaleformPredictedEndTimer = 0;
	void RunDataSetup(const HeistCelebData* data) {
		if (m_bisFirstInit) {
			SetupInstances();
			m_bisFirstInit = false;
		}
		for (auto& scl : m_ScaleformInstances) {
			CScaleform scaleform = CScaleform(scl);
			if (data->RepWall) {
				scaleform.CallScaleform("ADD_REP_POINTS_AND_RANK_BAR_TO_WALL", );
			}
		}
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
		if (IsKeyJustUp(VK_F15) && !m_bIsScaleformActive) {
			TestOp();
			timeStarted = MISC::GET_GAME_TIMER();
			
			//GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(Scaleforms[0], "GET_TOTAL_WALL_DURATION");
			//int val = GRAPHICS::END_SCALEFORM_MOVIE_METHOD_RETURN_VALUE();
			//WHILE(!GRAPHICS::IS_SCALEFORM_MOVIE_METHOD_RETURN_VALUE_READY(val));
			//int mxs = GRAPHICS::GET_SCALEFORM_MOVIE_METHOD_RETURN_VALUE_INT(val);

			reqTimer = MISC::GET_GAME_TIMER() + 19000; // 600~ per-segment unless paused?
		}
		if (IsKeyJustUp(VK_F16)) {
			/*
				hs4f_drp_off cutscene where lootbag gets dropped off. AFTER FIRST HEIST NOT MADRAZO CUT
				hs4f_drp_cel cutscene where player drinks and throws lime off of balcony
			*/
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
		}
	}

	int timeStarted = 0;
	int reqTimer = 0;
	void TestOp() {
		ScaleformInit();
		for (int i = 0; i < 3; i++) {
			int s = Scaleforms[i];
			int WallId = 1;
			CallScaleform(s, "CLEANUP", WallId);
			CallScaleform(s, "CREATE_STAT_WALL", WallId, "HUD_COLOUR_HSHARD", 100); // 100?
			CallScaleform(s, "ADD_MISSION_RESULT_TO_WALL", WallId, "OHHH", "HARRY", "", true, true, true);
			int stepId = 1;
			int statTable = 2; // extentions to tables have to be separate in IDs. Like you can have multiple cash animations they have to be different id's though. 
			CallScaleform(s, "CREATE_STAT_TABLE", WallId, statTable, 3); // what is 3 sfxId? 
			CallScaleform(s, "ADD_STAT_TO_TABLE", WallId, statTable, "GARRY", "", true, true, false, false, "HUD_COLOUR_PLATINUM"); // check flags after value
			CallScaleform(s, "ADD_STAT_TO_TABLE", WallId, statTable, "HARRY", "", true, true, false, false, "HUD_COLOUR_SILVER"); // check flags after value
			CallScaleform(s, "ADD_STAT_TABLE_TO_WALL", WallId, statTable); // I lose it here?
			//CallScaleform(s, "PAUSE", WallId, 5 * 6);
			//CallScaleform(s, "ADD_MISSION_RESULT_TO_WALL", WallId, "IM BOUTA", "CUMMMMMM", "", true, true, true);
			CallScaleform(s, "CREATE_INCREMENTAL_CASH_ANIMATION", WallId, stepId);
			CallScaleform(s, "ADD_INCREMENTAL_CASH_WON_STEP", WallId, stepId, 0, 100000000, "IM BOUTA", "CUUUUUUUUUUUUM", "CAME", 1, 3);
			CallScaleform(s, "ADD_INCREMENTAL_CASH_ANIMATION_TO_WALL", WallId, stepId);
			CallScaleform(s, "ADD_JOB_POINTS_TO_WALL", WallId, 15, 1); // xAlign could be weird
			CallScaleform(s, "ADD_REP_POINTS_AND_RANK_BAR_TO_WALL", WallId, 2000, 0, 0, 2000, 1500, 2000, "NICE!", "CUM!");
			CallScaleform(s, "ADD_BACKGROUND_TO_WALL", WallId, 255, 1);
			CallScaleform(s, "SHOW_STAT_WALL", WallId);
			CallScaleform(s, "createSequence", WallId, 1, 1);
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

	}
	void OnShutdown() override
	{
		LAGInterface::Writeln("[CTest] Shutdown called");
		m_bIsInvincible = false;
		ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), false);
	}
};