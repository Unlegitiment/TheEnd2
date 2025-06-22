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
#include <typeinfo>
class CScaleform {
public:
	explicit CScaleform(int Handle) :m_Handle(Handle) {} // fuck shit 
	template<typename... T> bool CallScaleform(const char* method, T&&... args) {
		bool bRes = GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(m_Handle, method);
		(Call(std::forward<T>(args)), ...); // this syntax is just so weird. 
		GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
		return bRes;
	}
	operator int() {
		return m_Handle;
	}
	int GetHandle() const { return m_Handle; }
	template<typename T> void Call(T data) {
		LAGInterface::Writeln("Call<T>: Unhandled template specialization type: %s", typeid(T).name());
	}
	template<> void Call<int>(int data) {
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(data);
	}
	template<> void Call<const char*>(const char* data) {
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING(data);
	}
	template<> void Call<char*>(char* data) {
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING(data);
	}
	template<> void Call<char[]>(char data[]) {
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
//class CScaleformManager;
//extern CScaleformManager* g_pScaleformManager;
class CScaleformManager {
public:
	static inline CScaleformManager* sm_pInstance = nullptr;
	static void Init() {
		LAGInterface::Write(__FUNCTION__ " called ");
		//g_pScaleformManager = new CScaleformManager();
		sm_pInstance = new CScaleformManager();
		LAGInterface::Writeln(" g_pScaleformManager = 0x%p", sm_pInstance);
	}
	static CScaleformManager* Get() {
		return sm_pInstance;
	}
	static void Destroy() {
		delete sm_pInstance;
	}
	CScaleform* GetScaleform(const std::string& name) {
		auto it = m_Scaleforms.find(name);
		CScaleform* retScaleform = nullptr;
		if (it == m_Scaleforms.end()) { // we didn't find it.
			retScaleform = AddScaleform(name);
		}
		else {
			it->second.bIsInUse = true;
			retScaleform = it->second.pScaleform;
		}
		return retScaleform;
	}
	void Release(const std::string& name) {
		auto it = m_Scaleforms.find(name);
		if (it == m_Scaleforms.end()) {
			return;
		}
		it->second.bIsInUse = false;
	}
private:
	CScaleform* AddScaleform(const std::string name) {
		int handle = GRAPHICS::REQUEST_SCALEFORM_MOVIE(name.c_str());
		WHILE(!GRAPHICS::HAS_SCALEFORM_MOVIE_LOADED((int)handle)){
			handle = GRAPHICS::REQUEST_SCALEFORM_MOVIE(name.c_str());
		}
		CScaleform* scaleform = new CScaleform(handle);
		m_Scaleforms.insert({ name, sScaleform(scaleform, true) });
		return scaleform;
	}
	struct sScaleform {
		sScaleform(CScaleform* scaleform, bool bUse) {
			this->pScaleform = scaleform;
			this->bIsInUse = bUse;
		}
		CScaleform* pScaleform = nullptr;
		bool bIsInUse = false;
	};
	std::unordered_map<std::string, sScaleform> m_Scaleforms;
};

struct HeistCelebSetup {
	const char* bgColour = "HUD_COLOUR_BLACK";
	const int sfxId = 3;
	const int alpha = 100;
	const int moneyMesh = 1; // check val
	bool IsScaleformDone = false;
};
struct HeistCelebRepWall{
	int repGained, startRepPoints, MinRepOfRank, MaxRepOfRank, CurrentRank, NextRank;
	char* SmallText = "NICE";
	char* BigText = "RANKUP!"; //on average
};
struct HeistCelebJobWall{
	int points;
	bool xAlign;
};
struct HeistCelebCashWall { // these two are the same object just merge them into one super at some point since your gonna need it for MP_CELEBRATION
	int points;
	bool xAlign;
};
struct HeistCelebCashDeduction {
	char* title, * desc;
	int value;
};
struct HeistCelebCashWon {
	char* statLabel;
	int finalValue, startValue;
	bool xAlign, isRawText;
};
struct HeistCelebCompleteMessage {
	char* missionTextLabel, * completeTextLabel, * messageLabel;
	bool isMessageRawText, isCompleteRawText, isMissionTextRawText;
};
struct HeistCelebMissionResult {
	char* missionTextLabel, *passFailTextLabel, *messageLabel;
	bool isMessageRawText = true, isPassFailRawText = true, isMissionTextRawText = true;
};

struct sStat {
	char* name, value;
	bool isNameRawText = true, isValueRawText = true, isTotalRow = true, isStatValueTime = false;
	char* colourId;
};
class CStatTable {
public:
	friend class CWall;
	template<typename T, size_t Size>
	using Array = std::array<T, Size>;
	explicit CStatTable(int statId) /* m_Scaleforms(scaleforms)*/ {
		this->m_iStatId = statId;
	}
	void AddStat(const sStat& stat) {
		m_Stats.push_back(stat);
	}
private:
	int m_iStatId = 0;
	std::vector<sStat> m_Stats;
};
struct sIncrementalCashStep {
	int startDollars, finishDollars;
	char* topText, * bottomText, * rightHandStat;
	int rightHandIcon;
	int soundType;
};
class CIncrementalCashAnim {
public:
	friend class CWall;
	CIncrementalCashAnim(int id) : m_iId(id) { }
	void AddStep(const sIncrementalCashStep& step) {	
		this->m_Steps.push_back(step);
	}
private:
	std::vector<sIncrementalCashStep> m_Steps;
	int m_iId;
};
struct HeistCelebData { // how do I have timed events
	HeistCelebSetup m_Base;
	HeistCelebRepWall* RepWall = nullptr;
	HeistCelebJobWall* JobPointData = nullptr;
	HeistCelebMissionResult* MissionResult = nullptr;
	CIncrementalCashAnim* CashAnimation = nullptr;
	CStatTable* StatTable = nullptr;
	HeistCelebCashWall* m_pCashWall = nullptr;
	HeistCelebCashDeduction* m_pDeduction = nullptr;
	HeistCelebCashWon* m_pCashWon = nullptr;
	HeistCelebCompleteMessage* m_pCompleteMessage = nullptr;
	int Timer = 7000;
};
struct MapEntry_HeistCeleb {
	CIncrementalCashAnim* pCash = nullptr;
	CStatTable* pTable = nullptr;
};
template<typename K, typename V>
using HashMap = std::unordered_map<K, V>;
class CStatTableMgr {
public:
#ifdef STAT_TABLE_MGR_USE_SINGLETON
	static void Init() {
		sm_pInstance = new CStatTableMgr();
	}
	static void Destroy() { 
		delete sm_pInstance; 
	}
	static CStatTableMgr* Get() {
		return sm_pInstance;
	}
#endif
	CStatTableMgr(HashMap<int, MapEntry_HeistCeleb>& map) : m_Map(map){

	}
	CStatTable* Get(int id) {
		auto& it = m_Map.find(id);
		if (it == m_Map.end()) { // did not find
			return Create(id);
		}
		return it->second.pTable;
	}
	~CStatTableMgr() {
		InternalDestroy();
	}
private:
	void InternalDestroy() {
		for (auto& stat : m_Map) {
			if (stat.second.pTable) {
				delete stat.second.pTable;
			}
		}
		//m_Map.clear(); // remove all.
	}
	CStatTable* Create(int Id) {
		CStatTable* table = new CStatTable(Id);
		MapEntry_HeistCeleb ent = MapEntry_HeistCeleb();
		ent.pTable = table;
		m_Map.insert({ Id, ent });
		return table;
	}
	HashMap<int, MapEntry_HeistCeleb>& m_Map;
#ifdef STAT_TABLE_USE_SINGLETON
	static inline CStatTableMgr* sm_pInstance = nullptr;
#endif
};
class CCashAnimMgr {
public:
	CCashAnimMgr(HashMap<int, MapEntry_HeistCeleb>& rMap) : m_rMap(rMap){ }
	CIncrementalCashAnim* Get(int id) {
		auto& it = m_rMap.find(id);
		if (it == m_rMap.end()) { // did not find.
			LAGInterface::Writeln("Couldn't find the id. Creation triggered");
			return Create(id);
		}
		return it->second.pCash;
	}
	~CCashAnimMgr() {
		for (auto& m : m_rMap) {
			delete m.second.pCash;
		}
	}
private:
	CIncrementalCashAnim* Create(int id) {
		MapEntry_HeistCeleb ent{};
		ent.pCash = new CIncrementalCashAnim(id);
		m_rMap.insert({id, ent});
		return ent.pCash;
	}
	HashMap<int, MapEntry_HeistCeleb>& m_rMap;
};
#define logf(expr, ...) LAGInterface::Writeln("%s@@@%d: " expr, __FUNCTION__, __LINE__, __VA_ARGS__)
class CWall {
	int m_iWallId = 0;
public:
	template<typename T, size_t Size>
	using Array = std::array<T, Size>;
	CWall(Array<CScaleform*, 3>& scalef, HashMap<int, MapEntry_HeistCeleb>& data, int WallId) : m_rScaleforms(scalef), m_iWallId(WallId), m_Entries(data){
		logf("0x%p, 0x%p, 0x%p", m_pCashManager, m_pManager, &m_Entries);
		//CreateWall("HUD_COLOUR_HSHARD");
	}
	void CreateWall(const char* HUDCOLOUR, int sfxId = 3) {
		AllCall("CREATE_STAT_WALL", m_iWallId, HUDCOLOUR, sfxId); // Obviously we want to try to rebind these. But for now it should be good. 
	}
	// @TODO: Create Id generation system so user doesn't have to provide an ID. (Ease of Use)
	CStatTable* CreateStatTable(int StatId) {
		return m_pManager->Get(StatId);
	}
	CIncrementalCashAnim* CreateCashAnimation(int Id) { 
		CIncrementalCashAnim* anim = m_pCashManager->Get(Id);
		logf("Id passed: %d Anim value: 0x%p", Id, anim);
		return anim;
	}
	template<typename T>
	void AddToWall(T*) {
		LAGInterface::Writeln(__FUNCTION__"test");
		LAGInterface::Writeln("AddToWall<%s(T*)> is not valid, no default specialization." typeid(T).name());
		return;
	}
	using JP = HeistCelebJobWall;
	template<> void AddToWall<JP>(JP* jobWall) {
		if (!jobWall) return;
		LAGInterface::Writeln(__FUNCTION__"test");
		AllCall("ADD_JOB_POINTS_TO_WALL", m_iWallId, jobWall->points, jobWall->xAlign);
	}
	using RepAndRank = HeistCelebRepWall;
	template<> void AddToWall<RepAndRank>(RepAndRank* wall){
		if (!wall) return;	
		LAGInterface::Writeln(__FUNCTION__"test");

		AllCall("ADD_REP_POINTS_AND_RANK_BAR_TO_WALL", this->m_iWallId, 
			wall->repGained, wall->startRepPoints, wall->MinRepOfRank, wall->MaxRepOfRank, wall->CurrentRank, wall->NextRank, wall->SmallText, wall->BigText);
	}
	using MissionResult = HeistCelebMissionResult;
	template<> void AddToWall<MissionResult>(MissionResult* wall) {
		if (!wall) return;
		LAGInterface::Writeln(__FUNCTION__"test");
		AllCall("ADD_MISSION_RESULT_TO_WALL", this->m_iWallId, 
			wall->missionTextLabel, wall->passFailTextLabel, wall->messageLabel, wall->isMessageRawText, wall->isPassFailRawText, wall->isMissionTextRawText);
	}
	using StatTbl = CStatTable;	
	template<> void AddToWall<StatTbl>(StatTbl* table) {
		if (!table) return;
		AllCall("CREATE_STAT_TABLE", m_iWallId, table->m_iStatId);
		for (auto& it : table->m_Stats) {
			AllCall("ADD_STAT_TO_TABLE", this->m_iWallId, table->m_iStatId, it.name, it.value, it.isNameRawText, it.isValueRawText, it.isTotalRow, it.isStatValueTime, it.colourId);
		}
		ShowStatTable(table);
	}
	using CashAnim = CIncrementalCashAnim;
	template<> void AddToWall<CashAnim>(CashAnim* anim) {
		if (!anim) return;
		AllCall("CREATE_INCREMENTAL_CASH_ANIMATION", this->m_iWallId, anim->m_iId);
		for (auto& it : anim->m_Steps) { 
			AllCall("ADD_INCREMENTAL_CASH_WON_STEP", this->m_iWallId, anim->m_iId, it.startDollars, it.finishDollars, it.topText, it.bottomText, it.rightHandStat, it.rightHandIcon, it.soundType);
			logf("%d %d [%d %d %s %s %s %d %d]", this->m_iWallId, anim->m_iId, it.startDollars, it.finishDollars, it.topText, it.bottomText, it.rightHandStat, it.rightHandIcon, it.soundType);
		}
		ShowCashAnim(anim);
		logf("CashAnim: 0x%p", anim);
	}
	template<> void AddToWall<HeistCelebCashWon>(HeistCelebCashWon* won) {
		if (!won) return;
		AllCall("ADD_CASH_WON_TO_WALL", this->m_iWallId, won->statLabel, won->finalValue, won->startValue, won->xAlign, won->isRawText);
	}
	template<> void AddToWall<HeistCelebCashDeduction>(HeistCelebCashDeduction* won) {
		if (!won) return;
		AllCall("ADD_CASH_DEDUCTION", this->m_iWallId, won->title, won->desc, won->value);
	}
	template<> void AddToWall<HeistCelebCashWall>(HeistCelebCashWall* won) {
		if (!won) return;
		AllCall("ADD_CASH_TO_WALL", this->m_iWallId, won->points, won->xAlign);
	}
	template<> void AddToWall<HeistCelebCompleteMessage>(HeistCelebCompleteMessage* won) {
		if (!won) return;
		AllCall("ADD_COMPLETE_MESSAGE_TO_WALL", this->m_iWallId, won->missionTextLabel, won->completeTextLabel, won->messageLabel, won->isMessageRawText, won->isCompleteRawText, won->isMissionTextRawText);
	}
	void ShowCashAnim(CashAnim* anim) {
		if (!anim) return;
		logf("CashAnim: 0x%p \t\tid: %d, sizeOfData: %d", anim, anim->m_iId, anim->m_Steps.size());
		AllCall("ADD_INCREMENTAL_CASH_ANIMATION_TO_WALL", this->m_iWallId, anim->m_iId);
	}
	void ShowStatTable(StatTbl* table) {
		if (!table) return;
		AllCall("ADD_STAT_TABLE_TO_WALL", this->m_iWallId, table->m_iStatId);
	}
	void TestMethod() {
		AllCall("ADD_CASH_DEDUCTION", m_iWallId, "Title", "Desc", 8000);
		AllCall("ADD_CASH_WON_TO_WALL", m_iWallId, "statLabel", 8000, 0, false, true);
		AllCall("ADD_CASH_TO_WALL", m_iWallId, 8000, false);
	}
	void Show() {
		AllCall("SHOW_STAT_WALL", this->m_iWallId);
	}
	void AddBackground(int alpha = 96, int moneyMesh = 3) {
		AllCall("ADD_BACKGROUND_TO_WALL", this->m_iWallId, alpha, alpha);
	}
	void Clean() {
		AllCall("CLEANUP", m_iWallId);
	}
	~CWall() {
		delete m_pCashManager;
		delete m_pManager;
		m_Entries.clear();
	}
private:
	template<typename...Args> void AllCall(const char* method, Args&&... args) {
		for (auto& scl : m_rScaleforms) {
			if (!scl->CallScaleform(method, args...)) {
				logf("Scaleform Failed on %d", scl->GetHandle());
			}
		}
		return;
	}
	HashMap<int, MapEntry_HeistCeleb>& m_Entries;
	CStatTableMgr* m_pManager = new CStatTableMgr(m_Entries); // this is a per-scaleform item. Might get moved out of here, not sure.
	CCashAnimMgr* m_pCashManager = new CCashAnimMgr(m_Entries);
	Array<CScaleform*, 3>& m_rScaleforms;
};

class CHeistCelebRenderer {
public:
	CHeistCelebRenderer() { Init(); }
	void Init() {
		SetupInstances();
		m_pWall = new CWall(m_ScaleformInstances, m_MapData, WallId);
	}
	void Update() {
		RenderData();
	}
	CWall* GetWall() { /*LAGInterface::Writeln(__FUNCTION__ " 0x%p", m_pWall);*/ return m_pWall; }
private:
	HashMap<int, MapEntry_HeistCeleb> m_MapData;
	void RenderData() {
		GRAPHICS::DRAW_SCALEFORM_MOVIE_FULLSCREEN_MASKED(m_ScaleformInstances[BACKGROUND]->GetHandle(), m_ScaleformInstances[FOREGROUND]->GetHandle(), 255, 255, 255, 255);
		GRAPHICS::DRAW_SCALEFORM_MOVIE_FULLSCREEN(m_ScaleformInstances[BASE]->GetHandle(), 255, 255, 255, 255, 1); // yokay!
	}
	enum eScaleformIndex {
		BASE = 0,
		BACKGROUND = 1,
		FOREGROUND = 2,
		MAX_SCALEFORM_INSTANCES
	};
	bool SetupInstances() {
		const char* scl_base = "HEIST_CELEBRATION";
		const char* scl_base_bg = "HEIST_CELEBRATION_BG";
		const char* scl_base_fg = "HEIST_CELEBRATION_FG";
		m_ScaleformInstances[BASE] = CScaleformManager::Get()->GetScaleform(scl_base);
		m_ScaleformInstances[BACKGROUND] = CScaleformManager::Get()->GetScaleform(scl_base_bg);
		m_ScaleformInstances[FOREGROUND] = CScaleformManager::Get()->GetScaleform(scl_base_fg);
		return true;
	}
private:
	std::array<CScaleform*, MAX_SCALEFORM_INSTANCES> m_ScaleformInstances;
	int WallId = 0; // gotta do something new.
	CWall* m_pWall = nullptr;
};
class CHeistCelebController {
	enum eStateMachine {
		eSM_INVALID,
		eSM_SETUP_DATA,
		eSM_RUN_DATA,
		eSM_CLEANUP,
		eSM_DONE,
		eSM_MAX
	};
public:
	void Init() {
		m_pRenderer = new CHeistCelebRenderer();
	}
	void Destroy() {
		delete m_pRenderer;
	}
	CIncrementalCashAnim* CreateCashAnimation(int id) {
		return m_pRenderer->GetWall()->CreateCashAnimation(id);
	}
	CStatTable* CreateStatTable(int id) {
		return m_pRenderer->GetWall()->CreateStatTable(id);
	}
	bool isFirstRun = true;
	void Update(HeistCelebData* data) {
		//if (data == nullptr) return; // we are null and don't care about progressionAdd commentMore actions
		if (data == nullptr) {
			//LAGInterface::Writeln("data is nullptr! quick return triggered");
			if (m_State != eSM_INVALID) {
				//yikes we got a problem! Scaleform data was just randomly changed to nullptr! this is a problem!
				m_pRenderer->GetWall()->Clean();
				LAGInterface::Writeln("data is nullptr! but our state is not invalid! quick return triggered");
				m_State = eSM_INVALID;
			}
			return;
		}
		if (data->m_Base.IsScaleformDone) {
			//LAGInterface::Writeln("data->m_Base.IsScaleformDone is true! quick return triggered");
			return;
		}
		if (m_State == eSM_INVALID) { // this is gonna be rough but I don't really care smh.
			LAGInterface::Writeln("SetupData Triggered");
			//if(!isFirstRun)
			m_pRenderer->GetWall()->CreateWall("HUD_COLOUR_HSHARD");
			m_pRenderer->GetWall()->AddToWall(data->MissionResult);
			m_pRenderer->GetWall()->AddToWall(data->m_pCompleteMessage);
			m_pRenderer->GetWall()->AddToWall(data->m_pDeduction);
			m_pRenderer->GetWall()->AddToWall(data->CashAnimation);
			m_pRenderer->GetWall()->AddToWall(data->StatTable);
			m_pRenderer->GetWall()->ShowStatTable(data->StatTable);
			m_pRenderer->GetWall()->AddToWall(data->m_pCashWon);
			m_pRenderer->GetWall()->AddToWall(data->m_pCashWall);
			m_pRenderer->GetWall()->AddToWall(data->JobPointData);
			m_pRenderer->GetWall()->AddToWall(data->RepWall);
			m_pRenderer->GetWall()->AddBackground(75);
			m_pRenderer->GetWall()->Show();
			m_ScaleformPredictedEndTimer = MISC::GET_GAME_TIMER() + data->Timer;
			//DATA = data; // might not be necessary
			m_State = eSM_RUN_DATA;
			return;
		}
		if (m_State == eSM_RUN_DATA) {
			if (MISC::GET_GAME_TIMER() >= m_ScaleformPredictedEndTimer) {
				LAGInterface::Writeln("MISC::GET_GAME_TIMER() >= m_ScaleformPredictedEndTimer! Setting state to Cleanup!");
				m_State = eSM_CLEANUP;
			}
			m_pRenderer->Update();
		}
		if (m_State == eSM_CLEANUP) {
			m_pRenderer->GetWall()->Clean();
			LAGInterface::Writeln("CLEANUP Triggered");
			data->m_Base.IsScaleformDone = true;
			isFirstRun = false;
			m_State = eSM_DONE;
		}
		if (m_State == eSM_DONE) {
			LAGInterface::Writeln("eSM_DONE Triggered");
			m_State = eSM_INVALID;
		}
	}
private:
	int m_ScaleformPredictedEndTimer = 0;
	CHeistCelebRenderer* m_pRenderer = nullptr;
	//HeistCelebData* m_Data = nullptr;
	eStateMachine m_State;
};
class CHud {
public:
	CHud() {
		celeb->Init();
	}
	CIncrementalCashAnim* CreateCash(int id) {
		return celeb->CreateCashAnimation(id);
	}
	CStatTable* CreateStat(int id) {
		return celeb->CreateStatTable(id);
	}
	void SetHeistData(HeistCelebData* data) {
		this->Data = data;
	}
	void Update() {
		celeb->Update(Data); 
	}
	~CHud() {
		celeb->Destroy();
		delete celeb;
	}
private:
	CHeistCelebController* celeb = new CHeistCelebController();
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
	void OnTick() override
	{
		if (IsKeyJustUp(VK_F13)) {
			LAGInterface::Writeln("%d", NETWORK::NETWORK_LEAVE_TRANSITION());
		}
		if (IsKeyJustUp(VK_F14)) {
			//TestFunc();
			CarWash();
		}
		if (m_bIsInvincible) {
			HUD::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
			HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME("Player is Invulnerable");
			HUD::END_TEXT_COMMAND_DISPLAY_TEXT(0.2, 0.1, 1);
		}
		if (IsKeyJustUp(VK_F15)) {
			//TestOp();
			timeStarted = MISC::GET_GAME_TIMER();
			if (data.m_Base.IsScaleformDone) {
				data.m_Base.IsScaleformDone = false;
			}
			hud->SetHeistData(&data);
			//GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(Scaleforms[0], "GET_TOTAL_WALL_DURATION");
			//int val = GRAPHICS::END_SCALEFORM_MOVIE_METHOD_RETURN_VALUE();
			//WHILE(!GRAPHICS::IS_SCALEFORM_MOVIE_METHOD_RETURN_VALUE_READY(val));
			//int mxs = GRAPHICS::GET_SCALEFORM_MOVIE_METHOD_RETURN_VALUE_INT(val);

			reqTimer = MISC::GET_GAME_TIMER() + 19000; // 600~ per-segment unless paused?
		}
		hud->Update();
		if (IsKeyJustUp(VK_F16)) {
			/*
				hs4f_drp_off cutscene where lootbag gets dropped off. AFTER FIRST HEIST NOT MADRAZO CUT
				hs4f_drp_cel cutscene where player drinks and throws lime off of balcony
			*/
			hud->SetHeistData(nullptr);
			//const char* cutname = "hs4f_drp_cel";
			//if (!isSceneRunning) {
			//	CUTSCENE::REQUEST_CUTSCENE(cutname, 8);
			//	CUTSCENE::START_CUTSCENE(0);
			//	isSceneRunning = true;
			//}
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
	CHud* hud = nullptr;
	int timeStarted = 0;
	int reqTimer = 0;
	void TestOp() {
		ScaleformInit();
		for (int i = 0; i < 3; i++) {
			int s = Scaleforms[i];
			CScaleform scaleform = CScaleform(s);
			int WallId = 1;
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
	HeistCelebData data;
	HeistCelebJobWall jpData;
	HeistCelebRepWall repWall;
	HeistCelebMissionResult missionresult;
	//HeistCelebStatTable statTable;
	void OnInit() override
	{
		LAGInterface::Writeln("Init");
		CScaleformManager::Init();
		hud = new CHud();
		jpData.points = 15;
		jpData.xAlign = false;
		repWall.CurrentRank = 112;
		repWall.MaxRepOfRank = 1500;
		repWall.MinRepOfRank = 0;
		repWall.NextRank = 113;
		repWall.repGained = 204;
		repWall.startRepPoints = 1295;
		missionresult.missionTextLabel = "SETUP";
		missionresult.passFailTextLabel = "failed";
		missionresult.messageLabel = "Vanoss died";
		auto* pCash = hud->CreateCash(1);
		sIncrementalCashStep step = sIncrementalCashStep();
		step.startDollars = 0;
		step.finishDollars = 10000;
		step.rightHandStat = "StatTest";
		step.rightHandIcon = 1;
		step.soundType = 3;
		step.topText = "TestTop";
		step.bottomText = "TestBtm";
		pCash->AddStep(step);

		//statTable.stepId = 1;
		//HeistCelebStat stat1;
		//strncpy_s(stat1.name, "Name1", 7);
		//strncpy_s(stat1.value, "Value", 7);
		//HeistCelebStat stat2 = stat1;
		//strncpy_s(stat2.name, "Name2", 7);
		//statTable.stats.push_back(stat1);
		//statTable.stats.push_back(stat2);
		data.RepWall = &repWall;
		data.JobPointData = &jpData;
		data.MissionResult = &missionresult;
		data.CashAnimation = pCash;
		//data.StatTable = &statTable;
	}
	CTest() {

	}
	void OnShutdown() override
	{
		delete hud;
		LAGInterface::Writeln("[CTest] Shutdown called");
		m_bIsInvincible = false;
		ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), false);
	}
};