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


class CTest : public fwScriptEnv {
	//SPAWN_SCRIPT_MACRO(CTest);
	SCRIPT_INIT(CTest);
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
	void OnTick() override
	{
		//logf("Test");
		if (IsKeyDown(VK_F14)) {
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
	}
	void OnShutdown() override
	{
		delete hud;
		LAGInterface::Writeln("[CTest] Shutdown called");
		m_bIsInvincible = false;
		ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), false);
	}
};