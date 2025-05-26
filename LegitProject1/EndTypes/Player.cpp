#include "Player.h"
#include "GTAVInfrastructure\Hud\HudComponents.h"
CPlayer::CPlayer() : m_RankData{}, m_RankController(m_RankData), m_GadgetHash("NULL")
{
}

void CPlayer::EquipGadget(const StringHash& hash)
{
	static StringHash NullHash = StringHash("NULL");
	if (hash.GetHash() == NullHash.GetHash()) {
		WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), this->m_GadgetHash.GetHash());
		this->m_GadgetHash = NullHash;
	}
	WEAPON::SET_PED_GADGET(PLAYER::PLAYER_PED_ID(), hash.GetHash(), 1);
	this->m_GadgetHash = hash; // I hope this copies lmao. 
}


void CPlayer::AddPlayerXP(unsigned long long XP)
{
	CRankBar::SetColor();
	bool HasPlayerLeveledUp = m_RankController.AddXp(XP);
	CRankBar::SetRankScores(m_RankData.GetXPLowRange(), m_RankData.GetXPHighRange(), m_RankData.GetPreviousXP(), m_RankData.GetCurrentXP(), m_RankData.GetRank(), m_RankData.GetRank() + 1);
	if (HasPlayerLeveledUp) { // we already do Rank Adjustments;
		
	}
	return;
}
