#pragma once
#include "GTAVInfrastructure\ScriptEnv\Script.h"
#include "TheEnd2\keyboard.h"
#include "GTAVInfrastructure\World\World.h"
#include "EndTypes\EndWorld.h"
#include "LInfras.h"
#include "GTAVInfrastructure\SHV\natives.h"
#include "GTAVInfrastructure\streaming\IPL.h"
#include "GTAVInfrastructure\streaming\InteriorSet.h"
#include "GTAVInfrastructure\streaming\Model.h"
#include <string>
#include "EndTypes\Player.h"
#include <array>
#include <algorithm>
#include <GTAVInfrastructure\CGame.h>
#include <GTAVInfrastructure\Hud\Obj\MissionText.h>
#include "GTAVInfrastructure\macros\WHILE.h"
#include "GTAVInfrastructure\Camera\Camera.h"
/*
	I'll just lay out how I program here:
		- If you don't call new/malloc don't call delete. Unless doing a very specific pattern and its predictable i.e. Builder.
		- Be explicit
		- Don't wrap in Inheritance if there is no value to the abstraction. Meaning "do we really need virtual behavior here?"
		- Naming Scheme should match Hungarian ((Relative Position)_(type)(Name) ex: m_iFoo (member variable of type int named Foo) or sm_iFoo (static member variable of type int named Foo)
		- Use new if there is a value for it. For example "Do I want this data to exist later? (See CSlideObject* CTheEndScript::pObject)"
			Another reason for new would be to have stricter rules for objects that exist.
				For example the use of new can dictate whether an object should exist.
				This is important for static contexts because ScriptHookV hates globals and hates statics even more.
		- Super Systems should be entirely static look at CGame or LAGInterface. Both interact with different stuff but are entirely static.
		- Pointers are innately dangerous, use-after-frees, which is why if you take a pointer in to an object,
			never expect to free it. In-fact that class doesn't own anything. It needs access to that pointer.
			Not the allocator to the pointer else we'd just call new. Or whatever.
		- Other than that have fun. (Also char* when you don't allocate them suck)
											^( had to get my StackOverflow points LOL)
*/
class CSlideObject {
public:
	CSlideObject(CVector3 objectOrigin, Object o, float zDist) : m_Object(o), m_ObjectOrigin(objectOrigin), m_fzMax(objectOrigin.GetZ() + zDist), m_fzMin(m_fzMax - zDist) {}
	void SetStep(float _step) {
		this->m_fStep = _step;
	}
	float GetAjarState() {
		return m_fAjar;
	}
	bool _Slide(CVector3& out, float Step) { // impl
		CVector3 vector = ENTITY::GET_ENTITY_COORDS(m_Object, true);
		if (Step < 0) {
			if (vector.GetZ() <= m_ObjectOrigin.GetZ()) {
				return false;
			}
		}
		else { // Step > 0
			if (vector.GetZ() > m_fzMax) {
				return false;
			}
		}
		vector.SetZ(vector.GetZ() + Step);
		out = vector;
		return true; // we are sliding
	}
	void Reset() {
		ENTITY::SET_ENTITY_COORDS(m_Object, m_ObjectOrigin.GetX(), m_ObjectOrigin.GetY(), m_ObjectOrigin.GetZ(), 1, 0, 0, 1);
	}
	void RecalcAjar(const CVector3& curLocation) {
		m_fAjar = (curLocation.GetZ() - m_fzMin) / (m_fzMax - m_fzMin); // 
	}
	void SlideUp() {
		CVector3 out = m_ObjectOrigin;
		if (_Slide(out, m_fStep) && m_SlideState != SLIDE_MAX_UP) { // we are sliding
			ENTITY::SET_ENTITY_COORDS(m_Object, out.GetX(), out.GetY(), out.GetZ(), 1, 0, 0, 1);
			RecalcAjar(out);
			m_SlideState = SLIDING_UP;
		}
		else { // we are not sliding
			m_SlideState = SLIDE_MAX_UP;
		}
	}
	void SlideDown() {
		CVector3 out = m_ObjectOrigin;
		if (_Slide(out, m_fNegStep) && m_SlideState != SLIDE_MAX_DOWN) { // we are sliding
			ENTITY::SET_ENTITY_COORDS(m_Object, out.GetX(), out.GetY(), out.GetZ(), 1, 0, 0, 1);
			RecalcAjar(out);
			m_SlideState = SLIDING_DOWN;
		}
		else { // we are not sliding
			m_SlideState = SLIDE_MAX_DOWN;
		}
	}
	CVector3 GetOrigin() { return this->m_ObjectOrigin; }
	enum SlideState {
		SLIDING_UP,
		SLIDING_DOWN,
		SLIDE_MAX_DOWN, // this should be the default position
		SLIDE_MAX_UP,
	};
	float& GetTriggerDistance() {
		return m_fTriggerDistance;
	}
private:
	float m_fAjar = 0.0f;
	SlideState m_SlideState = SLIDE_MAX_DOWN;
	Object m_Object;
	CVector3 m_ObjectOrigin;
	float m_fzMax;
	float m_fzMin; // Min
	float m_fStep = 0.035f; // smooth enough
	float m_fNegStep = -m_fStep;
	float m_fTriggerDistance = 6.0f;
};
class CFreemodeMapLoader {
public:
	enum eInteriors : int {
		FACILITY_HATCH_,
		BUNKER_HATCH_,
		ARENA_BANNERS,
		CASINO_DIAMOND_,
		LOS_SANTOS_TUNERS,
		SECURITY_EXT_,
		DRUG_WARS_ADD,
		BAIL_OFFICE,
		AGENT_SABOTAGE,
		MAX
	};
	enum BunkerHatchLocations {
		DesertBunker,
		SmokeTreeBunker,
		ScrapyardBunker,
		OilfieldsBunker,
		RatonCanyonBunker,
		GrapeseedBunker,
		FarmhouseBunker,
		PaletoBunker,
		NULL_BUNKER,
		Route68Bunker,
		ZancudoBunker,
		Route1Bunker,
		MAX_BUNKER
	};
	enum eSupportedDLCs {
		GUNRUNNING,
		DOOMSDAY,
		DIAMOND_CASINO,
		CONTRACT,
		MAX_DLC,
	};
	void Init() {
		this->m_OnlineMapData[FACILITY_HATCH_] = MapDataBuilder()
			.Add("xm_hatches_terrain")
			.Add("xm_hatches_terrain_lod")
			.Add("xm_hatch_closed")
			.Build();
		this->m_OnlineMapData[BUNKER_HATCH_] = MapDataBuilder()
			.Add("gr_case0_bunkerclosed")
			.Add("gr_case1_bunkerclosed")
			.Add("gr_case2_bunkerclosed")
			.Add("gr_case3_bunkerclosed")
			.Add("gr_case4_bunkerclosed")
			.Add("gr_case5_bunkerclosed")
			.Add("gr_case6_bunkerclosed")
			.Add("gr_case7_bunkerclosed")
			.Add("gr_case9_bunkerclosed")
			.Add("gr_case10_bunkerclosed")
			.Add("gr_case11_bunkerclosed") // Really could have kept the gr_case{x}_bunkerclosed thing but I see no reason to keep it. Was cool
			.Build();
		this->m_OnlineMapData[ARENA_BANNERS] = MapDataBuilder().Add("xs_arena_banners_ipl").Build();
		this->m_OnlineMapData[CASINO_DIAMOND_] = MapDataBuilder()
			.Add("hei_dlc_casino_aircon")
			.Add("hei_dlc_casino_aircon_lod")
			.Add("hei_dlc_casino_door")
			.Add("hei_dlc_casino_door_lod")
			.Add("hei_dlc_vw_roofdoors_locked")
			.Add("hei_dlc_windows_casino")
			.Add("hei_dlc_windows_casino_lod")
			.Add("vw_ch3_additions")
			.Add("vw_ch3_additions_long_0")
			.Add("vw_ch3_additions_strm_0")
			.Add("vw_dlc_casino_door")
			.Add("vw_dlc_casino_door_lod")
			.Add("vw_casino_billboard")
			.Add("vw_casino_billboard_lod(1)")
			.Add("vw_casino_billboard_lod") // apa_prop_ss1_mpint_garage2
			.Build();
		this->m_OnlineMapData[DRUG_WARS_ADD] = MapDataBuilder()
			.Add("xm3_warehouse")
			.Add("xm3_warehouse_grnd")
			.Add("xm3_warehouse_lod")
			.Build();
		this->m_OnlineMapData[SECURITY_EXT_] = MapDataBuilder()
			.Add("sf_billboards") // MOMOMOTI?(lol) billboards
			.Add("sf_billboards_lod") // 
			.Add("sf_franklin") // Franklin's house
			.Add("sf_mansionroof") // finale?
			.Add("sf_musicrooftop") // rooftop for party
			.Add("sf_musicrooftop_lod")
			.Add("sf_phones") // pay phones
			.Add("sf_plaque_bh1_05") // plaques for outside Agencies
			.Add("sf_plaque_bh1_05_lod")
			.Add("sf_plaque_hw1_08")
			.Add("sf_plaque_kt1_05")
			.Add("sf_plaque_kt1_08").Build();
		this->m_OnlineMapData[LOS_SANTOS_TUNERS] = MapDataBuilder()
			.Add("tr_tuner_meetup")
			.Add("tr_tuner_meetup_lod")
			.Add("tr_tuner_race_line")
			.Add("tr_tuner_shop_burton")
			.Add("tr_tuner_shop_burton_lod")
			.Add("tr_tuner_shop_mesa")
			.Add("tr_tuner_shop_mesa_lod")
			.Add("tr_tuner_shop_mission")
			.Add("tr_tuner_shop_mission_lod")
			.Add("tr_tuner_shop_rancho")
			.Add("tr_tuner_shop_rancho_lod")
			.Add("tr_tuner_shop_strawberry")
			.Add("tr_tuner_shop_strawberry_lod").Build();
		this->m_OnlineMapData[AGENT_SABOTAGE] = MapDataBuilder()
			.Add("m24_2_airstrip")
			.Add("m24_2_airstrip_lod")
			.Add("m24_2_cargoship_overlay")
			.Add("m24_2_garment_factory")
			.Add("m24_2_garment_factory_door")
			.Add("m24_2_garment_factory_lod")
			.Add("m24_2_hanger_additions")
			.Add("m24_2_legacy_fixes")
			.Add("m24_2_mfh_finale_ground")
			.Add("m24_2_mfh_finale_ground_lod").Build(); // these don't work given that the game build is different under OpenIV. Need to Repatch I think.
		this->m_OnlineMapData[BAIL_OFFICE] = MapDataBuilder()
			.Add("m24_1_bailoffice_davis")
			.Add("m24_1_bailoffice_delperro")
			.Add("m24_1_bailoffice_missionrow")
			.Add("m24_1_bailoffice_paletobay")
			.Add("m24_1_bailoffice_vinewood").Build();
	}
	void InitFreemode() {
		this->Init();
		for (int i = 0; i < MAX; i++) {
			this->LoadInterior((eInteriors)i); // gotta cast lmao C++ is annoying.
		}
	}
	void LoadInterior(eInteriors interiors) {
		m_OnlineMapData[interiors]->Request();
	}
	void Unload(eInteriors interiors) { // this is so we don't delete interiors from the list.
		this->m_OnlineMapData[interiors]->Derequest();
	}
	void Remove(eInteriors interiors) {
		CMapData* interior = this->m_OnlineMapData[interiors];
		auto it = std::find(m_OnlineMapData.begin(), m_OnlineMapData.end(), interior);
		if (it == m_OnlineMapData.end()) { return; }
		m_OnlineMapData.erase(it);
		delete interior;
	}
	void AddIPLToInterior(eInteriors interiors, fwIPL ipl, bool load) {
		this->m_OnlineMapData[interiors]->Add(ipl);
		if (load) {
			ipl.Request();
		}
	}
	// @Problem: Bug: Calling this multiple times will remove multiple instances without reason. 
	void RemoveIPLFromInterior(eInteriors interior, fwIPL ipl, bool unload) {
		LAGInterface::Writeln(__FUNCTION__" called with ipl: %s", ipl.GetIPLName());
		this->m_OnlineMapData[interior]->Remove(ipl);
		LAGInterface::Writeln(__FUNCTION__" called with ipl: %s : 2nd Call", ipl.GetIPLName());

		if (unload) ipl.Derequest();

	}
	void RemoveAll() {
		for (int i = 0; i < MAX; i++) {
			Remove((eInteriors)i);
		}
	}
	~CFreemodeMapLoader() {
		RemoveAll();
	}
private:
	std::vector<CMapData*> m_OnlineMapData{ MAX };
};
/*
	Modeling after R*'s Door Sys
*/
class CDoorManager {
public:
	CDoorManager() {}
	int AddSlide(CVector3 originCoord, Object o, float zDist) {
		auto hash = GenerateHash(originCoord);
		this->m_pObject.insert({ hash, new CSlideObject(originCoord, o, zDist) });
		return hash;
	}
	CSlideObject* Get(const CVector3& coord) {
		int hash = GenerateHash(coord);
		auto it = m_pObject.find(hash);
		if (it == m_pObject.end()) return nullptr;
		return it->second;
	}
	CSlideObject* Get(int hash) {
		auto it = m_pObject.find(hash);
		if (it == m_pObject.end()) return nullptr;
		return it->second;
	}
	void RemoveSlide(CSlideObject* obj) {
		//auto it = std::find(m_pObject.begin(), m_pObject.end(), obj);
		//if (it == m_pObject.end()) return;
		//delete it->second; // ensure we delete the object first. 
		//m_pObject.erase(it);
	}
	void RemoveHash(int hash) {
		//auto it = std::find(m_pObject.begin(), m_pObject.end(), hash);
		//if (it == m_pObject.end()) return;
		//delete it->second; // ensure we delete the object first. 
		//m_pObject.erase(it);
	}
private:
	int GenerateHash(const CVector3& hash) {
		return MISC::GET_HASH_KEY((std::to_string(hash.GetX()) += std::to_string(hash.GetY()) += std::to_string(hash.GetZ())).c_str());
	}
	std::unordered_map<int, CSlideObject*> m_pObject;
};
class CFreemodeMap {
public:
	enum class eDoors : int {
		MAZE_BANK_GARAGE,
		MAZE_BANK_WEST_GARAGE,
		LOMBANK_WEST_GARAGE,
		ARCADIUS_GARAGE,
		MAX_DOORS
	};
	CFreemodeMap() {
		this->m_pMapLoader = new CFreemodeMapLoader();
		m_pMapLoader->InitFreemode(); // Setup Map Data
		//this->m_pDoorManager = new CDoorManager();
		InitCustomDoors();
	}
	void Update() {
		for (int i = 0; i < (int)eDoors::MAX_DOORS; i++) {
			if (OFFICE_GARAGE_DOORS[i] == nullptr) { continue; }
			auto* door = OFFICE_GARAGE_DOORS[i];
			if (CVector3(ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1)).Dist(door->GetOrigin()) <= door->GetTriggerDistance()) {
				door->SlideUp();
			}
			else {
				door->SlideDown();
			}
		}
	}
	static CVector3 GetDoorLocation(eDoors doors) {
		switch (doors) {
		case eDoors::ARCADIUS_GARAGE:
			return { 0,0,0 };
		case eDoors::LOMBANK_WEST_GARAGE:
			return { 0,0,0 };
		case eDoors::MAZE_BANK_GARAGE:
			return { -79.488, -786.100, 37.338 };
		case eDoors::MAZE_BANK_WEST_GARAGE:
			return { 0,0,0 };
		}
		throw std::runtime_error("Provided argument does not match any known location. ");
	}
	static CVector3 GetDoorRotation(eDoors doors) {
		switch (doors) {
		case eDoors::ARCADIUS_GARAGE:
			return { 0,0,0 };
		case eDoors::LOMBANK_WEST_GARAGE:
			return { 0,0,0 };
		case eDoors::MAZE_BANK_GARAGE:
			return { 0, 0, 14.750 };
		case eDoors::MAZE_BANK_WEST_GARAGE:
			return { 0,0,0 };
		}
		throw std::runtime_error("Provided argument does not match any known location. ");
	}
	void InitCustomDoors() { // Sliding Objects
		Object o = CreateObject("imp_Prop_ImpEx_Gate_01", GetDoorLocation(eDoors::MAZE_BANK_GARAGE), true);
		CVector3 v = GetDoorRotation(eDoors::MAZE_BANK_GARAGE);
		ENTITY::SET_ENTITY_ROTATION(o, v.GetX(), v.GetY(), v.GetZ(), 1, 1); // its time. 
		ScriptObjects.push_back(o);
		CSlideObject* SlideObject = CreateSlideObject(GetDoorLocation(eDoors::MAZE_BANK_GARAGE), o, 4.25f);
		OFFICE_GARAGE_DOORS[(int)eDoors::MAZE_BANK_GARAGE] = SlideObject;
	}
	~CFreemodeMap() {
		for (auto* ptr : OFFICE_GARAGE_DOORS) {
			delete ptr;
		}
		std::for_each(ScriptObjects.begin(), ScriptObjects.end(), [](Object o) {
			OBJECT::DELETE_OBJECT(&o);
			});
		//delete m_pDoorManager;
		delete m_pMapLoader;
		// I can't remove ScriptObjects without causing issue with exit procedure. Need a fix for that since destroying all the doors won't do. 
		// Normally on game exit its not as horrible because map is gone with it. But problem is that I am doing debug reloads
	}
private:
	Object CreateObject(StringHash hash, CVector3 position, bool dynamic) {
		CStreamingModel model = hash.GetHash();
		model.Request(); // not a pointer type. we don't need to send to other thing for this. temp model
		return OBJECT::CREATE_OBJECT(hash.GetHash(), position.GetX(), position.GetY(), position.GetZ(), 0, 0, 1);
	}
	CSlideObject* CreateSlideObject(CVector3 objectOrigin, Object o, float zDist) {
		return new CSlideObject(objectOrigin, o, zDist);
	}
	CSlideObject* OFFICE_GARAGE_DOORS[(int)eDoors::MAX_DOORS] = { nullptr };
	CFreemodeMapLoader* m_pMapLoader = nullptr;
	//CDoorManager* m_pDoorManager = nullptr;
	std::vector<Object> ScriptObjects;
};
/*
	This represents a strand of a World State.
	This means props, objects, spawned items are constrained to this world capture.
	World Captures can be persistent in the case of potentially Map Data or other extreme cases.
	Or other minor cases. Copying data is also important. Which means override Copy is important. Always take from the current and supply the next world state.
	Chain them like a list to get an Entire Mission's/Activity's WorldState.
	Built from the ashes of the old Capture System from TheEnd.
	TheEnd's Capture was more significant in the fact it captured PlayerState.
	PlayerState is not included in a WorldCapture, this is because PlayerState and WorldState are different innately.
*/
class Action {
public:
	virtual void Perform() = 0;
	virtual void Cancel() = 0;
private:
};
struct sTimeData {
	int hour, minute, second;
	bool DoTransition;
	bool AllowTransitionToSpinBackTime;
	bool FreezeTimeAt;
};
class CWorldCapture {
public:

protected:
	std::vector<Object> m_GameObjects;
	std::vector<Vehicle> m_GameVehicles;
	std::vector<Ped> m_Peds;
	std::vector<CMapData> m_MapData; // IPL Changes, Interior Protocols, etc.
	std::vector<Action> m_Interactions;
	sTimeData m_TimeData;
private:

};
enum eState : int {
	SP,
	END
};

struct TextureGTAV {
	const char* Dictionary;
	const char* Name;
	TextureGTAV(const char* Dict, const char* Name) :Dictionary(Dict), Name(Name) {
		GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT(Dictionary, 1); // we need this immediately. 
	}
	const char* GetDictionary() { return Dictionary; }
	const char* GetName() { return Name; }
};
void DrawMarker(eMarkerType marker, CVector3 position, CVector3 Direction, CVector3 Rotation, CVector3 Scale, Color32 col, TextureGTAV* text, bool bobUpDwn = false, bool faceCam = false, bool rotate = false, bool drawOnEnter = false) {
	GRAPHICS::DRAW_MARKER(marker, position.GetX(), position.GetY(), position.GetZ(), Direction.GetX(), Direction.GetY(), Direction.GetZ(), Rotation.GetX(), Rotation.GetY(), Rotation.GetZ(), Scale.GetX(), Scale.GetY(), Scale.GetZ(), (int)col.GetR(), (int)col.GetG(), (int)col.GetB(), (int)col.GetA(), bobUpDwn, faceCam, 0, rotate, text ? text->GetDictionary() : nullptr, text ? text->GetName() : nullptr, drawOnEnter);
}
#include "Interiors/Submarine.h"
#include <GTAVInfrastructure\Scene\SyncScene.h>
class CFreemodeMission;
class CMissionLauncher {
public:
	void ActivateMission(CFreemodeMission* actMission) {
		this->IsMissionActive = true;
		MissionActive = actMission;
	}
	void DeactivateMission() {
		MissionActive = nullptr;
		this->IsMissionActive = false;
	}
private:
	bool IsMissionActive = false;
	CFreemodeMission* MissionActive = false;
};

class CFreemodeMission {
public:
	void Init(CMissionLauncher* launchParameters) {
		this->pLauncher = launchParameters;
		pLauncher->ActivateMission(this);
		CGameWorld::GetStreamingMgr()->Add(&this->model);
		SpawnAssets();
		SpawnBlips();
		missionstate = 10;
	}
	int missionstate = 0;
	void Update() {
		if (missionstate == 10) {
			HUD::BEGIN_TEXT_COMMAND_PRINT("STRING");
			HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME("Go to the ~b~Kosatka");
			HUD::END_TEXT_COMMAND_PRINT(0, true);
			if (CVector3(ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1)).Dist(VehicleEntrance) < 3.f) {
				HUD::BEGIN_TEXT_COMMAND_DISPLAY_HELP("STRING");;
				HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME("Press ~INPUT_CONTEXT~ to enter.");
				HUD::END_TEXT_COMMAND_DISPLAY_HELP(0, 0, 0, -1);
				if (PAD::IS_CONTROL_JUST_PRESSED(0, 51)) {
					pInterior = new CSubInterior();
					pInterior->SetMissileChairState(CSubInterior::LEFT, CSubInterior::EMSA_READY);
					pInterior->SetMissileChairState(CSubInterior::RIGHT, CSubInterior::EMSA_READY);
					pInterior->SetSonarState(CSubInterior::ESSA_OFFLINE);
					ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1561.5070, 392.0058, -50, 1, 0, 0, 1);
					ENTITY::SET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID(), 178.0);
					CAM::SET_GAMEPLAY_CAM_RELATIVE_HEADING(0);
					missionstate = 20;
				}
			}
		}
		if (missionstate == 20) {
			//Set player blip inside of Kosatka's
			//Remove blip assets from outside world.
			//Summon enemies
			//Summon Pickups.
			//Handle Death Logic. 
		}
	}
	void Deactivate() {
		CGameWorld::GetStreamingMgr()->Remove(&this->model);
		this->pLauncher->DeactivateMission();
		this->CleanAssets();
		delete pInterior;
	}
	~CFreemodeMission() {
		Deactivate();
	}
private:
	void SpawnBlips() {
		bMissionEntity = HUD::ADD_BLIP_FOR_ENTITY(Kosatka);
		HUD::SET_BLIP_SPRITE(bMissionEntity, 760);
		HUD::SET_BLIP_COLOUR(bMissionEntity, 28);
		HUD::BEGIN_TEXT_COMMAND_SET_BLIP_NAME("STRING");
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME("Kosatka");
		HUD::END_TEXT_COMMAND_SET_BLIP_NAME(bMissionEntity);
		HUD::SET_BLIP_ROTATION(bMissionEntity, (int)VehRot.GetZ());
	}
	void SpawnAssets() {
		WHILE(!model.IsLoaded()) {
			model.Request();
			LAGInterface::Writeln("Model Was not loaded!");
		}
		Kosatka = VEHICLE::CREATE_VEHICLE(model.GetHash(), VehPos.GetX(), VehPos.GetY(), VehPos.GetZ(), VehRot.GetZ(), 1, 1, 1);
		ENTITY::SET_ENTITY_INVINCIBLE(Kosatka, 1);
		ENTITY::FREEZE_ENTITY_POSITION(Kosatka, 1);
	}
	void CleanAssets() {
		if (HUD::DOES_BLIP_EXIST(bMissionEntity)) HUD::REMOVE_BLIP(&bMissionEntity);
		if (ENTITY::DOES_ENTITY_EXIST(Kosatka)) VEHICLE::DELETE_VEHICLE(&Kosatka);
		if (pInterior) delete pInterior; pInterior = nullptr;
	}
	CStreamingModel model = CStreamingModel(StringHash("KOSATKA").GetHash());
	CSubInterior* pInterior = nullptr;
	Blip bMissionEntity = 0;
	CVector3 VehPos = { 422.7261, -3048.638, -8.5299 };
	CVector3 VehRot = { 0, 0,180 };
	CVector3 VehicleEntrance = { 422.6445, -3078.946, 3.2290 };
	Vehicle Kosatka = 0;
	CMissionLauncher* pLauncher = nullptr;
	Interior iKosatkaInteriorId;
};
//Both of these functions come from a very old code base.
void clearAll() {
	for (int i = 0; i < 7; i++) {
		GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD_ON_FRONTEND("SHOW_COLUMN");
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(i);
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_BOOL(FALSE);
		GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
		GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD_ON_FRONTEND("KILL_PAGE");
		GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
	}
}
void setCurrentColumn(int columnID) {
	for (int i = 0; i < 7; i++) {
		GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD_ON_FRONTEND("SHOW_COLUMN");
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(i);
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_BOOL(FALSE);
		GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
	}
	if (columnID >= 0) {
		GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD_ON_FRONTEND("SHOW_COLUMN");
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(columnID);
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_BOOL(TRUE);
		GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
	}
}
class ApartmentTest {
public:
	Blip SetupAptBlip(CVector3 position, bool isHeistsAvailable) {
		Blip b = HUD::ADD_BLIP_FOR_COORD(position.GetX(), position.GetY(), position.GetZ());
		HUD::SET_BLIP_SPRITE(b, 40);
		if (isHeistsAvailable) HUD::SET_BLIP_COLOUR(b, 2);
		HUD::BEGIN_TEXT_COMMAND_SET_BLIP_NAME("STRING");
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME("Apartment");
		HUD::END_TEXT_COMMAND_SET_BLIP_NAME(b);

		return b;
	}
	CSynchronizedScene* scene = nullptr;
	CCamera* cam = 0;
	ApartmentTest() {
		this->b = SetupAptBlip(position, true);
	}
	void Update() {
		
		DrawMarker(eMarkerType::MarkerTypeVerticalCylinder, position, { 0,0,0 }, { 0,0,0 }, { 0.7,0.7,0.6 }, { 93, 182, 229, 255 }, nullptr);
		if (position.Dist(ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1)) < 5) {
			if (!scene) {
				HUD::BEGIN_TEXT_COMMAND_DISPLAY_HELP("STRING");
				HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME("Press ~INPUT_CONTEXT~ to enter Apartment");
				HUD::END_TEXT_COMMAND_DISPLAY_HELP(0, 0, 0, -1);
			}
			if (scene && scene->GetSceneProgression() >= 0.95) {
				TASK::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::PLAYER_PED_ID());
				// Whats hard here is that an individual apartment must be checked for ownership by the player before we actually can access it. 
				// Obviously first we just define this basic apt entrance protocol but we also need a menu and the camera to be setup properly. Also need animations triggering, which could take a bit :/
				Ped playerPed = PLAYER::PLAYER_PED_ID();
				ENTITY::SET_ENTITY_COORDS(playerPed, -778.6782, 317.4247, 175.8036, 1, 0, 0, 1);
				ENTITY::SET_ENTITY_HEADING(playerPed, 270.3589);
				CAM::SET_GAMEPLAY_CAM_RELATIVE_HEADING(0); // fix camera
				CAM::RENDER_SCRIPT_CAMS(0, 0, 0, 0, 0, 0);
				CAM::DESTROY_CAM(*cam, 1);
				CGame::GetCameraMgr().DestroyCamera(cam);
				
				//Object o = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(-779.3145, 317.8782, 176.9175, 0.1, MISC::GET_HASH_KEY("hei_v_ilev_fh_heistdoor2"), 1, 0, 0);
				OBJECT::ADD_DOOR_TO_SYSTEM(doorHash, MISC::GET_HASH_KEY("hei_v_ilev_fh_heistdoor2"), -779.3145, 317.8782, 176.9175, 0, 1, 1);
				HUD::SET_BLIP_DISPLAY(b, 0);
				delete scene; scene = nullptr;
				m_bIsInApt = true;
			}
			//if(scene) LAGInterface::Writeln("Writing while sync scene progress");
			if (PAD::IS_CONTROL_JUST_PRESSED(0, 51) && !scene) {
				cam = CGame::GetCameraMgr().CreateCamera({ -778.8375, 312.4263, 86.2157 }, { -3.6467, -0.0242, -61.4538 }, 26);
				CGame::GetCameraMgr().SetCameraAsActive(CGame::GetCameraMgr().GetCamTypeFromName(cam->GetCameraName()));
				cam->AppleCamShake("HAND_SHAKE", 1.0f);
				CAM::RENDER_SCRIPT_CAMS(1, 0, 0, 0, 0, 0);
				WEAPON::SET_CURRENT_PED_WEAPON(PLAYER::PLAYER_PED_ID(), MISC::GET_HASH_KEY("WEAPON_UNARMED"), 1);
				Object AptDoorExterior = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(-776.1967, 313.5395, 86.1433, 0.3f, MISC::GET_HASH_KEY("APA_PROP_SS1_MPINT_DOOR_R"), 0, 0, 0);
				scene = new CSynchronizedScene();
				scene->SetupScene("anim@apt_trans@hinge_r", { -776.8116f, 313.5119f, 84.6981f }, { 0.0, 0.0, 180.0 });
				scene->QueueAnimation(PLAYER::PLAYER_PED_ID(), "ext_player");
				scene->QueueAnimation(AptDoorExterior, "ext_door");
				TASK::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::PLAYER_PED_ID());
				scene->RunScene();
			}
		}
		if (IsKeyJustUp(VK_F15)) {
			//We need the door when inside the apt.
			WEAPON::SET_CURRENT_PED_WEAPON(PLAYER::PLAYER_PED_ID(), MISC::GET_HASH_KEY("WEAPON_UNARMED"), 1);
			OBJECT::ADD_DOOR_TO_SYSTEM(doorHash, MISC::GET_HASH_KEY("hei_v_ilev_fh_heistdoor2"), -779.3145, 317.8782, 176.9175, 0, 1, 1);
			m_bIsInApt = true;
		}
		if (m_bIsInApt) {
			PAD::DISABLE_CONTROL_ACTION(0, 199, 0); // THIS FRAME!
			PAD::DISABLE_CONTROL_ACTION(0, 200, 0);
			PAD::DISABLE_CONTROL_ACTION(2, 199, 0);
			PAD::DISABLE_CONTROL_ACTION(2, 200, 0);
			if (PAD::IS_DISABLED_CONTROL_JUST_RELEASED(2, 199) || PAD::IS_DISABLED_CONTROL_JUST_RELEASED(2, 200)) { // note we have to disable pausemenu this frame because else we reveal the outside of the apartment.
				HUD::ACTIVATE_FRONTEND_MENU(MISC::GET_HASH_KEY("FE_MENU_VERSION_SP_PAUSE"), 0, 0);
			}
			//PAD::DISABLE_CONTROL_ACTION(0, 21, 1); //--Left shift for sprint
			PAD::DISABLE_CONTROL_ACTION(0, 22, 1); // JUMP
			//PLAYER::SET_PLAYER_SPRINT(PLAYER::PLAYER_ID(), 0);
			PLAYER::SET_PLAYER_CAN_USE_COVER(PLAYER::PLAYER_ID(), 0);
			HUD::HUD_SUPPRESS_WEAPON_WHEEL_RESULTS_THIS_FRAME();
			OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(doorHash, 4, 0, 1);
			for (int i = 0; i < 6; i++) {
				INTERIOR::ENABLE_EXTERIOR_CULL_MODEL_THIS_FRAME(AptExteriorHashes[i]);
				INTERIOR::ENABLE_SHADOW_CULL_MODEL_THIS_FRAME(AptExteriorHashes[i]);
			}
			GRAPHICS::DISABLE_OCCLUSION_THIS_FRAME();
			GRAPHICS::SET_DISABLE_DECAL_RENDERING_THIS_FRAME();
			UpdateMinigames();
			DrawMarker(eMarkerType::MarkerTypeVerticalCylinder, interiorDoorPosition, { 0,0,0 }, { 0,0,0 }, { 0.5,0.5,0.5 }, { 93, 182, 229, 255 }, nullptr);
			if (interiorDoorPosition.DistNoZ(ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1)) <= 1 && ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()) <= 145 && ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()) >= 36) { // between 145 && 36. Represent apt specific door
				// This should render some menu. GTAO does it so do we. 
				HUD::BEGIN_TEXT_COMMAND_DISPLAY_HELP("STRING");
				HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME("Press ~INPUT_CONTEXT~ to leave Apartment\nPress ~INPUT_CONTEXT_SECONDARY~ to enter Garage");
				HUD::END_TEXT_COMMAND_DISPLAY_HELP(0, 0, 0, -1);
				if (PAD::IS_CONTROL_JUST_PRESSED(0, 51)) {
					Ped plr = PLAYER::PLAYER_PED_ID();
					ENTITY::SET_ENTITY_COORDS(plr, position.GetX(), position.GetY(), position.GetZ(), 1, 0, 0, 1);
					ENTITY::SET_ENTITY_HEADING(plr, 180);
					CAM::SET_GAMEPLAY_CAM_RELATIVE_HEADING(0);
					HUD::SET_BLIP_DISPLAY(b, 2);
					PAD::ENABLE_CONTROL_ACTION(0, 199, 0);
					PAD::ENABLE_CONTROL_ACTION(0, 200, 0);
					PAD::ENABLE_CONTROL_ACTION(2, 199, 0);
					PAD::ENABLE_CONTROL_ACTION(2, 200, 0);
					PAD::ENABLE_CONTROL_ACTION(0, 21, 1); //--Left shift for sprint
					PAD::ENABLE_CONTROL_ACTION(0, 22, 1); // JUMP
					m_bIsInApt = false;
				}
			}
		}
	}
	void UpdateMinigames() {

	}
	~ApartmentTest() {
		OBJECT::REMOVE_DOOR_FROM_SYSTEM(doorHash, 1); // cleanup
	}
private:
	bool m_bIsInApt = false;
	CVector3 position = { -778.8582, 312.9203, 84.4984 };
	CVector3 interiorDoorPosition = { -778.6782, 317.4247, 175.8036 };
	Hash doorHash = MISC::GET_HASH_KEY("PROP_BUILDING_1_INT_DOOR_1");
	Hash AptExteriorHashes[7] = {
		MISC::GET_HASH_KEY("apa_ss1_11_flats"),
		MISC::GET_HASH_KEY("ss1_11_ss1_emissive_a"),
		MISC::GET_HASH_KEY("ss1_11_detail01b"),
		MISC::GET_HASH_KEY("ss1_11_Flats_LOD"),
		MISC::GET_HASH_KEY("SS1_02_Building01_LOD"),
		MISC::GET_HASH_KEY("SS1_LOD_01_02_08_09_10_11"),
		MISC::GET_HASH_KEY("SS1_02_SLOD1")
	};
	Blip b = 0;
};

class CTheEndScript : public ScriptableBaseV1<CTheEndScript> {
private:
	static inline int State = SP;
	enum eSlideState {
		SLIDE_UP,
		SLIDE_MAX_UP,
		SLIDE_DOWN,
		SLIDE_MAX_DOWN,
		DEFAULT
	};
	ApartmentTest Test;
public:
	CTheEndScript() : ScriptableBaseV1() { // @TODO: Create some way to create instances more nicely. Since ScriptableBase basically just adds it to the list rn. 

	}
	CFreemodeMission* m_pMission = nullptr;
	// Inherited via ScriptableBase
	CFreemodeMap* m_pMap = nullptr;
	Object object = 0;
	float fCurrentGarageSpeed = 3.0f;
	CVector3 vCurrentPos = { -79.488, -786.100, 37.338 };
	CSlideObject* pObject = nullptr;
	CStreamingModel pModel = CStreamingModel(MISC::GET_HASH_KEY("mp_m_freemode_01"));
	Ped iPlayerPed = 0;
	CMissionLauncher Launcher = CMissionLauncher();
	int curAction = CSubInterior::EMSA_OFFLINE;
	void OnTick() override
	{
		Test.Update();
		int worldState = CGameWorld::GetWorldState();
		//if (IsKeyJustUp(VK_ADD)) {
		//    if (pObject) pObject->Reset();
		//    vCurrentPos = { -79.488, -786.100, 37.338 };
		//}
		if (IsKeyJustUp(VK_MULTIPLY)) {
			int currentSet = 0;
			ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 345.00000000, 4842.00000000, -60.00000000, 1, 0, 0, 1);
			Interior interior = INTERIOR::GET_INTERIOR_AT_COORDS(345.00000000, 4842.00000000, -60.00000000);
			INTERIOR::ACTIVATE_INTERIOR_ENTITY_SET(interior, "set_int_02_shell");
			CVector3 vec = { 345.00000000, 4842.00000000, -60.00000000 };
			WHILE_EX(5000, currentSet != 11) {
				INTERIOR::SET_INTERIOR_ENTITY_SET_TINT_INDEX(interior, "set_int_02_shell", currentSet);
				INTERIOR::REFRESH_INTERIOR(interior);
				currentSet++;
				LAGInterface::Writeln("Interior: %d {%.f, %.f, %.f}Setting interior index %d", interior, vec.GetX(), vec.GetY(), vec.GetZ(), currentSet);
			}
		}
		if (m_pMission) {
			m_pMission->Update();
		}
		if (IsKeyJustUp(VK_F13)) {
			if (m_pMission) {
				delete m_pMission;
				m_pMission = nullptr;
			}
		}
		if (IsKeyJustUp(VK_F14) && !m_pMission) {
			//INTERIOR::DEACTIVATE_INTERIOR_ENTITY_SET(intid, "entity_set_brig");
			//INTERIOR::DEACTIVATE_INTERIOR_ENTITY_SET(intid, "entity_set_guide");

			m_pMission = new CFreemodeMission();
			m_pMission->Init(&Launcher);
			LAGInterface::Writeln("Test");
		}
		m_pMap->Update();
		CEndWorld::Update();
	}

	void OnInit() override
	{
		LoadEnd();
	}

	void OnShutdown() override
	{
		delete m_pMission; // Mission removes stuff when it goes out of scope this is called before deconstructor. 
		delete m_pMap;
		CEndWorld::Destroy();
		LAGInterface::Writeln("Shutdown Called for CTheEndScript");
	}

	static void ChangeState(eState _newState) {
		State = _newState;
	}
	void LoadEnd() {
		CGameWorld::GetWorldStateMgr()->EnableMPMap(true);
		m_pMap = new CFreemodeMap();
		CEndWorld::Init();
		CGameWorld::GetStreamingMgr()->Add(&pModel);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_LAST_MP_CHAR"), 0, 1);

		LAGInterface::Writeln("Init Called for CTheEndScript");
	}
private:
};








//          {DWORD key = VK_ADD;
//          using namespace GRAPHICS;
//          /*
//          <Item>
//              <MenuVersionId>FE_MENU_VERSION_MP_CHARACTER_CREATION</MenuVersionId>
//              <InitialScreen>MENU_UNIQUE_ID_HEADER_MP_CHARACTER_CREATION</InitialScreen>
//              <VersionFlags>kNoSwitchDelay kTabsAreColumns kNoTabChangeWhileLocked kNoTabChange kNotDimmable kNoPlayerInfo kNoBackground kHideHeaders kUseAlternateContentPos</VersionFlags>
//          </Item>
//          */
//          Hash menuHash = MISC::GET_HASH_KEY("FE_MENU_VERSION_MP_CHARACTER_CREATION");
//          //drawTextOnScreen(menuHash);
//          if (IsKeyJustUp(key)) {
//              BOOL isActive = TRUE;
//              int collID = 5; // 0 = FULL LIST MENU no breaks possible. 1 allows for it and im assuming 6 as well.
//              if (HUD::IS_PAUSE_MENU_ACTIVE() || HUD::IS_PAUSE_MENU_ACTIVE()) {
//                  if (HUD::GET_CURRENT_FRONTEND_MENU_VERSION() != 1399975061) {
//                      HUD::SET_FRONTEND_ACTIVE(FALSE);
//                  }
//              }
//              else {
//                  HUD::SET_FRONTEND_ACTIVE(FALSE);
//                  HUD::ACTIVATE_FRONTEND_MENU(menuHash, FALSE, 42);
//              }
//              WAIT(0); // Wait for 333 milliseconds (ScriptHookV function for wait)
//              if (HUD::IS_PAUSE_MENU_ACTIVE() || HUD::IS_PAUSE_MENU_ACTIVE()) {
//                  if (HUD::GET_CURRENT_FRONTEND_MENU_VERSION() != 1399975061) {
//                      HUD::SET_FRONTEND_ACTIVE(FALSE);
//                  }
//              }
//              else {
//                  HUD::SET_FRONTEND_ACTIVE(FALSE);
//                  HUD::ACTIVATE_FRONTEND_MENU(menuHash, FALSE, 42);
//              }
//              WAIT(0); // Wait for 333 milliseconds (ScriptHookV function for wait)
//              while (isActive) {
//                  //const InteractionMenu* m = menu; // has to be allocated AT runTime. For somereason? I guess it's  required at runtime so having it stored is weird.
//                  //if (m == NULL) return;
//                  BEGIN_SCALEFORM_MOVIE_METHOD_ON_FRONTEND("SET_DATA_SLOT_EMPTY");
//                  SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(collID);
//                  END_SCALEFORM_MOVIE_METHOD();
//                  BEGIN_SCALEFORM_MOVIE_METHOD_ON_FRONTEND("SET_COLUMN_TITLE");
//                  SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(collID);
//                  SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING("NAME");
//                  SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(1); //bool ?
//                  END_SCALEFORM_MOVIE_METHOD();
//                  BEGIN_SCALEFORM_MOVIE_METHOD_ON_FRONTEND("SET_COLUMN_TITLE");
//                  SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(collID);
//                  SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING("(*.CREW");
				  //SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(1); //bool ?
				  //SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(2); //bool ?
				  //SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(3); //bool ?
				  //SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(4); //bool ?
				  //SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(5); //bool ?
//                  SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(24); //bool ?
//                  END_SCALEFORM_MOVIE_METHOD();
//                  // struct -> createFunc -> saved -> drawList. (Just Like the PlayerList) however ideally carry the list on the backend so the API user doesn't have to deal with  it.
//                  //loop inside loop for rank
//                  //https://imgur.com/tTHhrph
//                  /* https://imgur.com/ibDgnKz */
//                  BEGIN_SCALEFORM_MOVIE_METHOD_ON_FRONTEND("SET_DATA_SLOT");
//                  SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(collID); // columnID
//                  SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(0); //INDEX OF BUTTONS. 0th Index controls hardware and rank bar everything but statistic. 
//                  SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(1040); //menuID placiblo
//                  SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(0); //null val? 0 does nothing?
//                  SCALEFORM_MOVIE_METHOD_ADD_PARAM_BOOL(FALSE); //default add lines this toggles off 1
//                  SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(0); //nothing? 2
//                  SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(1); //toggles plane? 3 
//                  SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(1); //toggles heli and adds val to pre-val? 4
//                  SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(0); //end val and boat? 5
//                  SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(0); //controls amount of bar filled and car 6
//                  SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING("7"); // 18 characters; rank
//                  SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING("BOTTOM"); // 8 val
//                  SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(0); // 9 text pos
//                  SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING(""); // NULL?
//                  SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING("MIDDLE"); // 11 val
//                  SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(0); // 12 val //pos?
//                  SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING(""); // 13 val //Null?
//                  SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING("TOP"); // 15 val // val
//                  SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(0); // 16 val // val
//                  SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING(""); // 17 val // val
//                  END_SCALEFORM_MOVIE_METHOD();
//                  for (int i = 1; i < 7; i++) {
//                      BEGIN_SCALEFORM_MOVIE_METHOD_ON_FRONTEND("SET_DATA_SLOT");
//                      SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(collID); // columnID
//                      SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(i); //INDEX OF BUTTONS. hardware access might be 0th index?
//                      SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(1040); //menuID placiblo
//                      SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(2); //null val? 0 does nothing?
//                      SCALEFORM_MOVIE_METHOD_ADD_PARAM_BOOL(FALSE); //default add lines this toggles off 1
//                      SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(0); //nothing? 2
//                      SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(23); //toggles plane? 3 
//                      SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(0); //toggles heli and adds val to pre-val? 4
//                      SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(100); //end val and boat? 5
//                      SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(50); //controls amount of bar filled and car 6
//                      END_SCALEFORM_MOVIE_METHOD();
//                  }
//                  BEGIN_SCALEFORM_MOVIE_METHOD_ON_FRONTEND("DISPLAY_DATA_SLOT");
//                  SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(collID);
//                  END_SCALEFORM_MOVIE_METHOD();
//                  BEGIN_SCALEFORM_MOVIE_METHOD_ON_FRONTEND("SET_COLUMN_FOCUS");
//                  SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(collID);
//                  SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(1);
//                  SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(1);
//                  END_SCALEFORM_MOVIE_METHOD();
//                  BEGIN_SCALEFORM_MOVIE_METHOD_ON_FRONTEND("SET_COLUMN_CAN_JUMP");
//                  SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(collID);
//                  SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(1);
//                  END_SCALEFORM_MOVIE_METHOD();
//                  setCurrentColumn(collID);
//                  if (1 > 7) {
//                      BEGIN_SCALEFORM_MOVIE_METHOD_ON_FRONTEND("INIT_COLUMN_SCROLL");
//                      SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(collID);
//                      SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(1); // null?
//                      SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(0); // hideFutureMenu?
//                      SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(1); // change scrollbar ico 0 = weird 1 = vertical 2 = horizontal 3 = null
//                      SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(1); // Scroll Icon Position 0 = Left; 1 = Middle; 2 = Right.
//                      SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(2); // Index in Context bar.
//                      END_SCALEFORM_MOVIE_METHOD();
//                  }
//                  if (IsKeyJustUp(key)) {
//                      for (int i = 0; i < 1; i++) {
//                          //free(&m->button[i]);
//                      }
//                      clearAll();
//                      HUD::SET_FRONTEND_ACTIVE(0);
//                      //free(&GLOBAL_MENU);
//                      isActive = FALSE;
//                  }
//                  WAIT(0);
//              }
//          }
//      }

//Previous Line is a BUNCH of old src for FrontendMenus? Doesn't work / Is Broken right now. 