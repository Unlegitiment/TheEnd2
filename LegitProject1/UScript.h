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
	enum eInteriors : int{
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
        this->m_OnlineMapData[BUNKER_HATCH_] =  MapDataBuilder()
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
			.Add("sf_plaque_kt1_08") .Build();
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

        if(unload) ipl.Derequest();

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
    std::vector<CMapData*> m_OnlineMapData{MAX};
};
/*
    Modeling after R*'s Door Sys
*/
class CDoorManager {
public:
    CDoorManager() {}
    unsigned long AddSlide(CVector3 originCoord, Object o, float zDist){
        auto hash = GenerateHash(originCoord);
        this->m_pObject.insert({ hash, new CSlideObject(originCoord, o, zDist) });
        return hash;
    }
    CSlideObject* Get(const CVector3& coord) {
        unsigned long hash = GenerateHash(coord);
        auto it = m_pObject.find(hash);
        if (it == m_pObject.end()) return nullptr;
        return it->second;
    }
	CSlideObject* Get(unsigned long hash) {
		auto it = m_pObject.find(hash);
		if (it == m_pObject.end()) return nullptr;
		return it->second;
	}
    void RemoveSlide(CSlideObject* obj){
        auto it = std::find(m_pObject.begin(), m_pObject.end(), obj);
        if (it == m_pObject.end()) return;
        delete it->second; // ensure we delete the object first. 
        m_pObject.erase(it);
    }
    void RemoveHash(unsigned long hash){
		auto it = std::find(m_pObject.begin(), m_pObject.end(), hash);
		if (it == m_pObject.end()) return;
		delete it->second; // ensure we delete the object first. 
		m_pObject.erase(it);
    }
private:
    unsigned long GenerateHash(const CVector3& hash){
        return MISC::GET_HASH_KEY((std::to_string(hash.GetX()) += std::to_string(hash.GetY()) += std::to_string(hash.GetZ())).c_str());
    }
    std::unordered_map<unsigned long,CSlideObject*> m_pObject; 
};
class CFreemodeMap {
public:
    enum class eDoors  : int {
        MAZE_BANK_GARAGE,
        MAZE_BANK_WEST_GARAGE,
        LOMBANK_WEST_GARAGE,
        ARCADIUS_GARAGE,
        MAX_DOORS
    };
    CFreemodeMap() {
        this->m_pMapLoader = new CFreemodeMapLoader();
        m_pMapLoader->InitFreemode(); // Setup Map Data
        this->m_pDoorManager = new CDoorManager();
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
        delete m_pDoorManager;
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
    CDoorManager* m_pDoorManager = nullptr;
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
enum eState : int{
    SP,
    END
};

class CTheEndScript : public ScriptableBase<CTheEndScript> {
private: 
    static inline int State = SP;
    enum eSlideState {
        SLIDE_UP,
        SLIDE_MAX_UP,
        SLIDE_DOWN,
        SLIDE_MAX_DOWN,
        DEFAULT
    };
public: 
    CTheEndScript() : ScriptableBase() { // @TODO: Create some way to create instances more nicely. Since ScriptableBase basically just adds it to the list rn. 
        
    }
    // Inherited via ScriptableBase
	CFreemodeMap* m_pMap = nullptr;
    Object object = 0;
    float fCurrentGarageSpeed = 3.0f;
	CVector3 vCurrentPos = { -79.488, -786.100, 37.338 };
    CSlideObject* pObject = nullptr;
    CStreamingModel pModel = CStreamingModel(MISC::GET_HASH_KEY("mp_m_freemode_01"));
    Ped iPlayerPed = 0;
    void OnTick() override
    {
		int worldState = CGameWorld::GetWorldState();
		if (IsKeyJustUp(VK_ADD)) {
            if(pObject) pObject->Reset();
            vCurrentPos = { -79.488, -786.100, 37.338 };
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


