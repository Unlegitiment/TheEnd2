#pragma once
#include <array>
#include "GTAVInfrastructure/SHV/natives.h"
class CRankData {
public:
	void SetRank(int rank) { m_iRank = rank; }
	int GetRank() { return this->m_iRank; }
	void SetCurrentXP(int x) { this->m_iCurrentXP = x; }
	int GetCurrentXP() { return this->m_iCurrentXP; }
	void SetPreviousXP(int x) { this->m_iPreviousXP = x; }
	int GetPreviousXP() { return this->m_iPreviousXP; }
	void SetXPHighRange(int x) { this->m_iXPHighRange = x; }
	int GetXPHighRange() { return this->m_iXPHighRange; }
	void SetXPLowRange(int x) { this->m_iXPLowRange = x; }
	int GetXPLowRange() { return this->m_iXPLowRange; }
private:
	int m_iRank = 1;
	int m_iCurrentXP = 0, m_iPreviousXP = 0;
	int m_iXPHighRange = 1000, m_iXPLowRange = 0;
};
class CRankController {
public:
	CRankController(CRankData& data) : m_rRankData(data){ }
	// returns if Player has Leveled Up from XP Addition;
	bool AddXp(unsigned long long x) {
		m_rRankData.SetPreviousXP(m_rRankData.GetCurrentXP());
		m_rRankData.SetCurrentXP(m_rRankData.GetCurrentXP() + (int)x); // cast
		if (m_rRankData.GetCurrentXP() >= m_rRankData.GetXPHighRange()) {
			m_rRankData.SetXPLowRange(XP(m_rRankData.GetRank()));
			m_rRankData.SetXPHighRange(XP(m_rRankData.GetRank() + 1));
			m_rRankData.SetRank(m_rRankData.GetRank() + 1);
			m_rRankData.SetPreviousXP(m_rRankData.GetCurrentXP());
			return true;
		}
		m_rRankData.SetPreviousXP(m_rRankData.GetCurrentXP());
		return false;
	}
	bool AddLevel(unsigned short x) {
		int Rank = XP(m_rRankData.GetRank()) + m_rRankData.GetCurrentXP();
		int Target = XP(x);
		return AddXp(Target - Rank);
	}
	void RemoveXP(unsigned long long x);
	void RemoveLevel(unsigned short x); // 64353?
	int GetCurrentLevel() { return m_rRankData.GetRank(); }
	
	int XP(int rank) {
		return (1000 + (rank - 1) * 125) * rank;
	};
private:
	CRankData& m_rRankData;
};
class StringHash {
public:
	StringHash(const char* moduleName) : m_Name(moduleName){}
	unsigned long GetHash() const { return MISC::GET_HASH_KEY(this->m_Name); }
	const char* GetName() { return this->m_Name; }
private:
	const char* m_Name;
};



class CPlayer {
public:
	CPlayer();
	void EquipGadget(const StringHash& hash);
	void AddPlayerXP(unsigned long long XP);
	int GetCurrentLevel();
private:
	CRankData m_RankData;
	CRankController m_RankController;
	StringHash m_GadgetHash;  
};