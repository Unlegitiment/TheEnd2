#include "CelebTypes.h"
#include <LegitProject1\LInfras.h>

CStatTable::CStatTable(int statId) /* m_Scaleforms(scaleforms)*/ {
	this->m_iStatId = statId;
}

void CStatTable::AddStat(const sStat& stat) {
	m_Stats.push_back(stat);
}

CIncrementalCashAnim::CIncrementalCashAnim(int id) : m_iId(id) { }

void CIncrementalCashAnim::AddStep(const sIncrementalCashStep& step) {
	this->m_Steps.push_back(step);
}
#ifdef STAT_TABLE_MGR_USE_SINGLETON

void CStatTableMgr::Init() {
	sm_pInstance = new CStatTableMgr();
}

void CStatTableMgr::Destroy() {
	delete sm_pInstance;
}

CStatTableMgr* CStatTableMgr::Get() {
	return sm_pInstance;
}
#endif // STAT_TABLE_MGR_USE_SINGLETON
CStatTableMgr::CStatTableMgr(HashMap<int, MapEntry_HeistCeleb>& map) : m_Map(map) {

}

CStatTable* CStatTableMgr::Get(int id) {
	auto it = m_Map.find(id);
	if (it == m_Map.end()) { // did not find
		return Create(id);
	}
	return it->second.pTable;
}

CStatTableMgr::~CStatTableMgr() {
	InternalDestroy();
}

void CStatTableMgr::InternalDestroy() {
	for (auto& stat : m_Map) {
		if (stat.second.pTable) {
			delete stat.second.pTable;
		}
	}
	//m_Map.clear(); // remove all.
}

CStatTable* CStatTableMgr::Create(int Id) {
	CStatTable* table = new CStatTable(Id);
	MapEntry_HeistCeleb ent = MapEntry_HeistCeleb();
	ent.pTable = table;
	m_Map.insert({ Id, ent });
	return table;
}

CIncrementalCashAnim* CCashAnimMgr::Get(int id) {
	auto it = m_rMap.find(id);
	if (it == m_rMap.end()) { // did not find.
		LAGInterface::Writeln("Couldn't find the id. Creation triggered");
		return Create(id);
	}
	return it->second.pCash;
}

CCashAnimMgr::~CCashAnimMgr() {
	for (auto& m : m_rMap) {
		delete m.second.pCash;
	}
}

CIncrementalCashAnim* CCashAnimMgr::Create(int id) {
	MapEntry_HeistCeleb ent{};
	ent.pCash = new CIncrementalCashAnim(id);
	m_rMap.insert({ id, ent });
	return ent.pCash;
}

CWall::CWall(Array<CScaleform*, 3>& scalef, HashMap<int, MapEntry_HeistCeleb>& data, int WallId) : m_rScaleforms(scalef), m_iWallId(WallId), m_Entries(data) {
	logf("0x%p, 0x%p, 0x%p", m_pCashManager, m_pManager, &m_Entries);
	//CreateWall("HUD_COLOUR_HSHARD");
}

void CWall::CreateWall(const char* HUDCOLOUR, int sfxId) {
	AllCall("CREATE_STAT_WALL", m_iWallId, HUDCOLOUR, sfxId); // Obviously we want to try to rebind these. But for now it should be good. 
}

// @TODO: Create Id generation system so user doesn't have to provide an ID. (Ease of Use)

CStatTable* CWall::CreateStatTable(int StatId) {
	return m_pManager->Get(StatId);
}

CIncrementalCashAnim* CWall::CreateCashAnimation(int Id) {
	CIncrementalCashAnim* anim = m_pCashManager->Get(Id);
	logf("Id passed: %d Anim value: 0x%p", Id, anim);
	return anim;
}

void CWall::ShowCashAnim(CashAnim* anim) {
	if (!anim) return;
	logf("CashAnim: 0x%p \t\tid: %d, sizeOfData: %d", anim, anim->m_iId, anim->m_Steps.size());
	AllCall("ADD_INCREMENTAL_CASH_ANIMATION_TO_WALL", this->m_iWallId, anim->m_iId);
}

void CWall::ShowStatTable(StatTbl* table) {
	if (!table) return;
	AllCall("ADD_STAT_TABLE_TO_WALL", this->m_iWallId, table->m_iStatId);
}

void CWall::TestMethod() {
	AllCall("ADD_CASH_DEDUCTION", m_iWallId, "Title", "Desc", 8000);
	AllCall("ADD_CASH_WON_TO_WALL", m_iWallId, "statLabel", 8000, 0, false, true);
	AllCall("ADD_CASH_TO_WALL", m_iWallId, 8000, false);
}

void CWall::Show() {
	AllCall("SHOW_STAT_WALL", this->m_iWallId);
}

void CWall::AddBackground(int alpha, int moneyMesh) {
	AllCall("ADD_BACKGROUND_TO_WALL", this->m_iWallId, alpha, alpha);
}

void CWall::Clean() {
	AllCall("CLEANUP", m_iWallId);
}

CWall::~CWall() {
	delete m_pCashManager;
	delete m_pManager;
	m_Entries.clear();
}
