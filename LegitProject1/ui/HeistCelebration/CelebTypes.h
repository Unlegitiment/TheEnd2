#pragma once
#include <LegitProject1\ui\Scaleform\Scaleform.h>
#include <unordered_map>
#include <LegitProject1\LInfras.h>
#include <unordered_map>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <typeinfo>
struct HeistCelebSetup {
	const char* bgColour = "HUD_COLOUR_BLACK";
	const int sfxId = 3;
	const int alpha = 100;
	const int moneyMesh = 1; // check val
	bool IsScaleformDone = false;
};
struct HeistCelebRepWall {
	int repGained, startRepPoints, MinRepOfRank, MaxRepOfRank, CurrentRank, NextRank;
	char* SmallText = (char*)"NICE";
	char* BigText = (char*)"RANKUP!"; //on average
};
struct HeistCelebJobWall {
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
	char* missionTextLabel, * passFailTextLabel, * messageLabel;
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
	explicit CStatTable(int statId);
	void AddStat(const sStat& stat);
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
	CIncrementalCashAnim(int id);
	void AddStep(const sIncrementalCashStep& step);
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
	static void Init();
	static void Destroy();
	static CStatTableMgr* Get();
#endif
	CStatTableMgr(HashMap<int, MapEntry_HeistCeleb>& map);
	CStatTable* Get(int id);
	~CStatTableMgr();
private:
	void InternalDestroy();
	CStatTable* Create(int Id);
	HashMap<int, MapEntry_HeistCeleb>& m_Map;
#ifdef STAT_TABLE_USE_SINGLETON
	static inline CStatTableMgr* sm_pInstance = nullptr;
#endif
};
class CCashAnimMgr {
public:
	CCashAnimMgr(HashMap<int, MapEntry_HeistCeleb>& rMap) : m_rMap(rMap) { }
	CIncrementalCashAnim* Get(int id);
	~CCashAnimMgr();
private:
	CIncrementalCashAnim* Create(int id);
	HashMap<int, MapEntry_HeistCeleb>& m_rMap;
};

class CWall {
	int m_iWallId = 0;
public:
	template<typename T, size_t Size>
	using Array = std::array<T, Size>;
	CWall(Array<CScaleform*, 3>& scalef, HashMap<int, MapEntry_HeistCeleb>& data, int WallId);
	void CreateWall(const char* HUDCOLOUR, int sfxId = 3);
	// @TODO: Create Id generation system so user doesn't have to provide an ID. (Ease of Use)
	CStatTable* CreateStatTable(int StatId);
	CIncrementalCashAnim* CreateCashAnimation(int Id);
	template<typename T>
	void AddToWall(T*);
	using JP = HeistCelebJobWall;
	template<> void AddToWall<JP>(JP* jobWall);
	using RepAndRank = HeistCelebRepWall;
	template<> void AddToWall<RepAndRank>(RepAndRank* wall);
	using MissionResult = HeistCelebMissionResult;
	template<> void AddToWall<MissionResult>(MissionResult* wall);
	using StatTbl = CStatTable;
	template<> void AddToWall<StatTbl>(StatTbl* table);
	using CashAnim = CIncrementalCashAnim;
	template<> void AddToWall<CashAnim>(CashAnim* anim);
	template<> void AddToWall<HeistCelebCashWon>(HeistCelebCashWon* won);
	template<> void AddToWall<HeistCelebCashDeduction>(HeistCelebCashDeduction* won);
	template<> void AddToWall<HeistCelebCashWall>(HeistCelebCashWall* won);
	template<> void AddToWall<HeistCelebCompleteMessage>(HeistCelebCompleteMessage* won);
	void ShowCashAnim(CashAnim* anim);
	void ShowStatTable(StatTbl* table);
	void TestMethod();
	void Show();
	void AddBackground(int alpha = 96, int moneyMesh = 3);
	void Clean();
	~CWall();
private:
	template<typename...Args> void AllCall(const char* method, Args&&... args);
	HashMap<int, MapEntry_HeistCeleb>& m_Entries;
	CStatTableMgr* m_pManager = new CStatTableMgr(m_Entries); // this is a per-scaleform item. Might get moved out of here, not sure.
	CCashAnimMgr* m_pCashManager = new CCashAnimMgr(m_Entries);
	Array<CScaleform*, 3>& m_rScaleforms;
};
#include <iostream>
#include <string>
#include <typeinfo>
template<typename T> void CWall::AddToWall(T*) {
	//LAGInterface::Writeln(__FUNCTION__"test");
	LAGInterface::Writeln("AddToWall<%s(T*)> is not valid, no default specialization.", typeid(T).name());
	return;
}

template<> inline void CWall::AddToWall(JP* jobWall) {
	if (!jobWall) return;
	LAGInterface::Writeln(__FUNCTION__"test");
	AllCall("ADD_JOB_POINTS_TO_WALL", m_iWallId, jobWall->points, jobWall->xAlign);
}

template<> inline void CWall::AddToWall(RepAndRank* wall) {
	if (!wall) return;
	LAGInterface::Writeln(__FUNCTION__"test");

	AllCall("ADD_REP_POINTS_AND_RANK_BAR_TO_WALL", this->m_iWallId,
		wall->repGained, wall->startRepPoints, wall->MinRepOfRank, wall->MaxRepOfRank, wall->CurrentRank, wall->NextRank, wall->SmallText, wall->BigText);
}

template<> inline void CWall::AddToWall(MissionResult* wall) {
	if (!wall) return;
	LAGInterface::Writeln(__FUNCTION__"test");
	AllCall("ADD_MISSION_RESULT_TO_WALL", this->m_iWallId,
		wall->missionTextLabel, wall->passFailTextLabel, wall->messageLabel, wall->isMessageRawText, wall->isPassFailRawText, wall->isMissionTextRawText);
}

template<> inline void CWall::AddToWall(StatTbl* table) {
	if (!table) return;
	AllCall("CREATE_STAT_TABLE", m_iWallId, table->m_iStatId);
	for (auto& it : table->m_Stats) {
		AllCall("ADD_STAT_TO_TABLE", this->m_iWallId, table->m_iStatId, it.name, it.value, it.isNameRawText, it.isValueRawText, it.isTotalRow, it.isStatValueTime, it.colourId);
	}
	ShowStatTable(table);
}

template<> inline void CWall::AddToWall(CashAnim* anim) {
	if (!anim) return;
	AllCall("CREATE_INCREMENTAL_CASH_ANIMATION", this->m_iWallId, anim->m_iId);
	for (auto& it : anim->m_Steps) {
		AllCall("ADD_INCREMENTAL_CASH_WON_STEP", this->m_iWallId, anim->m_iId, it.startDollars, it.finishDollars, it.topText, it.bottomText, it.rightHandStat, it.rightHandIcon, it.soundType);
		logf("%d %d [%d %d %s %s %s %d %d]", this->m_iWallId, anim->m_iId, it.startDollars, it.finishDollars, it.topText, it.bottomText, it.rightHandStat, it.rightHandIcon, it.soundType);
	}
	ShowCashAnim(anim);
	logf("CashAnim: 0x%p", anim);
}

template<> inline void CWall::AddToWall(HeistCelebCashWon* won) {
	if (!won) return;
	AllCall("ADD_CASH_WON_TO_WALL", this->m_iWallId, won->statLabel, won->finalValue, won->startValue, won->xAlign, won->isRawText);
}

template<> inline void CWall::AddToWall(HeistCelebCashDeduction* won) {
	if (!won) return;
	AllCall("ADD_CASH_DEDUCTION", this->m_iWallId, won->title, won->desc, won->value);
}

template<> inline void CWall::AddToWall(HeistCelebCashWall* won) {
	if (!won) return;
	AllCall("ADD_CASH_TO_WALL", this->m_iWallId, won->points, won->xAlign);
}

template<> inline void CWall::AddToWall(HeistCelebCompleteMessage* won) {
	if (!won) return;
	AllCall("ADD_COMPLETE_MESSAGE_TO_WALL", this->m_iWallId, won->missionTextLabel, won->completeTextLabel, won->messageLabel, won->isMessageRawText, won->isCompleteRawText, won->isMissionTextRawText);
}

template<typename ...Args> void CWall::AllCall(const char* method, Args && ...args) {
	for (auto& scl : m_rScaleforms) {
		if (!scl->CallScaleform(method, args...)) {
			logf("Scaleform Failed on %d", scl->GetHandle());
		}
	}
	return;
}
