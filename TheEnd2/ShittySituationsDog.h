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
public:
	// Inherited via ScriptableBase
	void OnTick() override
	{
		if (IsKeyJustUp(VK_F13)) {
			//LAGInterface::Writeln("[CTest] Testing!");
			//Vector3 vec = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
			//STREAMING::REQUEST_MODEL(DominatorHash);
			//Dominator = VEHICLE::CREATE_VEHICLE(DominatorHash, vec.x, vec.y, vec.z, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()), 0, 0, 1);
			GRAPHICS::TOGGLE_PAUSED_RENDERPHASES(true);
		}
		if (IsKeyJustUp(VK_F14)) {
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
		if (m_bIsInvincible) {
			HUD::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
			HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME("Player is Invulnerable");
			HUD::END_TEXT_COMMAND_DISPLAY_TEXT(0.2, 0.1, 1);
		}
		if (IsKeyJustUp(VK_F15) && !m_bIsScaleformActive) {
			TestOp();
			timeStarted = MISC::GET_GAME_TIMER();
			reqTimer = MISC::GET_GAME_TIMER() + 3330; // 10 seconds roughly? 
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
			GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(s, "CREATE_STAT_WALL");
			GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(WallId);
			GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING("HUD_COLOUR_BLACK");
			GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(1);
			GRAPHICS::END_SCALEFORM_MOVIE_METHOD();

			GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(s, "ADD_INTRO_TO_WALL");
			GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(WallId); // wallid
			GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING("MISSION"); //mode label
			GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING("Don't Fuck With Dre"); // jobName
			GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING("1"); // challengeTextLabel - makes the box bigger? 
			GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING(""); // ChallengePartsText
			GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING(""); // targetTypeTextLabel
			GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING(""); // targetval
			GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(0); // delay
			GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING(""); // TargetValuePrefix
			GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_BOOL(1); // modeIsStringLiteral
			//GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING("HUD_COLOUR_WHITE"); // when we override text color the mode label does not appear?
			GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
			//GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(s, "ADD_SCORE_TO_WALL");
			//GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(WallId);
			//GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING("textLabel");
			//GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(10000); // score
			//GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
			GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(s, "ADD_BACKGROUND_TO_WALL"); // this has to be down here? I'm not sure why. has to happen after all the ADD_X_TO_WALL calls
			GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(WallId);
			GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING("HUD_COLOUR_BLACK");
			GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(40);
			GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
			GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(s, "SHOW_STAT_WALL");
			GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(WallId);
			GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
			m_bIsScaleformActive = true;
		}
	}
	bool m_bIsScaleformActive = false;
	void ScaleformInit() {
		SCL = GRAPHICS::REQUEST_SCALEFORM_MOVIE("MP_CELEBRATION");
		SCL_BG = GRAPHICS::REQUEST_SCALEFORM_MOVIE("MP_CELEBRATION_BG");
		SCL_FG = GRAPHICS::REQUEST_SCALEFORM_MOVIE("MP_CELEBRATION_FG");
		WHILE(!GRAPHICS::HAS_SCALEFORM_MOVIE_LOADED(SCL)) {
			SCL = GRAPHICS::REQUEST_SCALEFORM_MOVIE("MP_CELEBRATION");
		}
		WHILE(!GRAPHICS::HAS_SCALEFORM_MOVIE_LOADED(SCL_BG)) {
			SCL_BG = GRAPHICS::REQUEST_SCALEFORM_MOVIE("MP_CELEBRATION_BG");
		}
		WHILE(!GRAPHICS::HAS_SCALEFORM_MOVIE_LOADED(SCL_FG)) {
			SCL_FG = GRAPHICS::REQUEST_SCALEFORM_MOVIE("MP_CELEBRATION_FG");
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