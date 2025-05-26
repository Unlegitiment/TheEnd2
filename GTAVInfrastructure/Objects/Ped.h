#pragma once

class CVehicle;
enum ePedType;
#include "baseEntity.h"
class CPed : public baseEntity{
public:
	friend class CPedBuilder;
	explicit CPed(int handle);
	static CPed* CreatePed(ePedType type, Hash modelHash, CVector3 position, float heading);
	static CPed* CreatePedEx(ePedType type, Hash modelHash, CVector3 position, float heading, bool isNetPed, bool bShouldDespawn);
	int& GetHandle() { return this->m_iHandle; }
	CVector3 GetPos();
	Hash GetModelHash() { return m_iModelHash; }
	CVehicle* GetVehicle();
	bool IsPlayer() { return m_bIsPlayer; }
	void SetPlayer(bool t) { m_bIsPlayer = t; }
private:
	Hash m_iModelHash;  
	int m_iHandle;
	bool m_bIsPlayer;
};
class CPedBuilder {
public:
	CPedBuilder& SetPedSkin(Hash h) {
		m_pPed->m_iModelHash = h;
		return *this;
	}
	CPedBuilder& SetPedRelationship() {

	}
private:
	CPed* m_pPed;
};