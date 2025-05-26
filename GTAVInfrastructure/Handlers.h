#pragma once
class CPed;
class CVehicle;
class baseEntity;
class CPedHeadshotMgr;
typedef unsigned long Hash;
enum ePedType;
#include "LegitProject1\datattypes\vector.h"
#include <vector>
class CEntityHandler {
public:
	CEntityHandler();
	CPed* CreatePed(ePedType type, Hash modelHash, CVector3 position, float heading);
	CVehicle* CreateVehicle(Hash modelHash, CVector3 pos, float heading, bool isNetwork, bool bScriptHostVeh);
	CPedHeadshotMgr& GetHeadshotMgr() { return *m_PedHeadshotMgr; }
	CPed* GetLocalPlayer() { return m_pPlayer; }
	void Destroy();
	~CEntityHandler() { Destroy(); }
private:
	CPedHeadshotMgr* m_PedHeadshotMgr = nullptr;
	std::vector<baseEntity*> m_Entities;
	CPed* m_pPlayer = nullptr;
};