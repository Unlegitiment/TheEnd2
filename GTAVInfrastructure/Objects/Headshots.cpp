#include "Headshots.h"
#include "GTAVInfrastructure\SHV\natives.h"
#include "Ped.h"
#include "LegitProject1\LInfras.h"
const char* CPedHeadshotMgr::GetTexture(CPed* pPed)
{
	if (IsValid(pPed) && IsReady(pPed)) {
		sHeadshotDetails* details = GetHeadFromPed(pPed);
		const char* ped = PED::GET_PEDHEADSHOT_TXD_STRING(details->Id);
		strncpy_s(details->txd, ped, CPedHeadshotMgr::TEXTURE_NAME_MAX);
		strncpy_s(details->txn, ped, CPedHeadshotMgr::TEXTURE_NAME_MAX); // fire
		return ped;
	}
	LAGInterface::Write("Invalid or Not ready Headshot :/");
	return nullptr;
}
void CPedHeadshotMgr::RegisterPed(CPed* pPed)
{
	sHeadshotDetails details{};
	details.Id = PED::REGISTER_PEDHEADSHOT(pPed->GetHandle());
	m_PedToHeadshot.insert({ pPed, details });
}

bool CPedHeadshotMgr::IsReady(CPed* pPed)
{
	auto it  = m_PedToHeadshot.find(pPed);
	if (it != m_PedToHeadshot.end()) {
		RegisterPed(pPed);
		return false;
	}
	bool bFinal = PED::IS_PEDHEADSHOT_READY(it->second.Id);
	it->second.isShotReady = bFinal;
	return bFinal;
}

bool CPedHeadshotMgr::IsValid(CPed* pPed)
{
	sHeadshotDetails* pShot = GetHeadFromPed(pPed);
	if (pShot == nullptr) {
		RegisterPed(pPed);
		return false;
	}
	bool bFinal = PED::IS_PEDHEADSHOT_VALID(pShot->Id);
	pShot->isShotReady = bFinal;
	return bFinal;
}

void CPedHeadshotMgr::DeregisterPed(CPed* pPed)
{
	if (!IsValid(pPed)) {
		LAGInterface::Write("Invalid Headshot attempted to be unregistered." RRed"Blocked.\n" RNorm);
		return;
	}
	sHeadshotDetails* headshot = GetHeadFromPed(pPed);
	PED::UNREGISTER_PEDHEADSHOT(headshot->Id);
	m_PedToHeadshot.erase(pPed); // didn't even need the iterator babyyyyy :))
}

CPedHeadshotMgr::sHeadshotDetails* CPedHeadshotMgr::GetHeadFromPed(CPed* pPed)
{
	auto it = m_PedToHeadshot.find(pPed);
	if (it == m_PedToHeadshot.end()) { return nullptr; }
	return &it->second;
}


