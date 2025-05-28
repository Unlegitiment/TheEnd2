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
class CFreemodeMap {
public:
	enum eInteriors : int{
		FACILITY_HATCH_,
        BUNKER_HATCH_,
        ARENA_BANNERS,
        CASINO_DIAMOND_,
        SECURITY_EXT_,
        DRUG_WARS_ADD,
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
			.Add("sf_plaque_kt1_08")
            .Build();
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
    ~CFreemodeMap() {
        RemoveAll();
    }
private:
    std::vector<CMapData*> m_OnlineMapData{MAX};

};
enum eState : int{
    SP,
    END
};
class CSlideObject {
public:
    CSlideObject(CVector3 objectOrigin, Object& o, float zDist):  m_Object(o), m_ObjectOrigin(objectOrigin), m_fzMax(objectOrigin.GetZ() + zDist), m_fzMin(m_fzMax - zDist){}
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
        ENTITY::SET_ENTITY_COORDS(m_Object, m_ObjectOrigin.GetX(), m_ObjectOrigin.GetY(), m_ObjectOrigin.GetZ(), 1,0,0,1);
    }
    void RecalcAjar(const CVector3& curLocation) {
		m_fAjar = (curLocation.GetZ() - m_fzMin) / (m_fzMax - m_fzMin); // 
    }
    void SlideUp() {
        CVector3 out = m_ObjectOrigin;
        if (_Slide(out, m_fStep) && m_SlideState != SLIDE_MAX_UP) { // we are sliding
            ENTITY::SET_ENTITY_COORDS(m_Object, out.GetX(), out.GetY(), out.GetZ(), 1,0,0,1);
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
    enum SlideState {
        SLIDING_UP,
        SLIDING_DOWN,
        SLIDE_MAX_DOWN, // this should be the default position
        SLIDE_MAX_UP,
    };
private:
    float m_fAjar = 0.0f;
    SlideState m_SlideState = SLIDE_MAX_DOWN;
    Object& m_Object;
    CVector3 m_ObjectOrigin;
    float m_fzMax;
    float m_fzMin; // Min
    float m_fStep = 0.035f; // smooth enough
    float m_fNegStep = -m_fStep;
};
class CTheEndScript : public ScriptableBase<CTheEndScript> {
private: 
    static inline int State = SP;
    bool Slide(CVector3& base, float step, float fMax) {

    }
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
        if (IsKeyJustUp(VK_DIVIDE)) {
            Hash objectHash = MISC::GET_HASH_KEY("imp_Prop_ImpEx_Gate_01");
            while (!STREAMING::HAS_MODEL_LOADED(objectHash)) {
                STREAMING::REQUEST_MODEL(objectHash);
                WAIT(0);
            }
            object = OBJECT::CREATE_OBJECT(objectHash, vCurrentPos.GetX(), vCurrentPos.GetY(), vCurrentPos.GetZ(), 1, 1, 1);
            ENTITY::SET_ENTITY_ROTATION(object, 0, 0, 14.750, 1, true);   
            pObject = new CSlideObject(vCurrentPos, object, 4.25);
        }
        if (pObject) {
            float fOldAjar = pObject->GetAjarState();
            if (CVector3(ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1)).Dist(vCurrentPos) <= 6.0f) {
                pObject->SlideUp();
            }
            else {
                pObject->SlideDown();
            }
            if (fOldAjar != pObject->GetAjarState()) {
                fOldAjar = pObject->GetAjarState();
                LAGInterface::Writeln("%f", fOldAjar);
            }
        }

        
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
        m_pMap->InitFreemode();
		CEndWorld::Init();
        CGameWorld::GetStreamingMgr()->Add(&pModel);
        STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_LAST_MP_CHAR"), 0, 1);

		LAGInterface::Writeln("Init Called for CTheEndScript");
    }
private:
};


