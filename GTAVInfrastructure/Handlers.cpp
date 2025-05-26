#include "Handlers.h"
#include "Objects\Vehicle.h"
#include "Objects\Ped.h"
#include "Objects\Headshots.h"
#include "GTAVInfrastructure\SHV\natives.h"
CEntityHandler::CEntityHandler()
{
    this->m_PedHeadshotMgr = new CPedHeadshotMgr();
    CPed* pPed = new CPed(PLAYER::PLAYER_PED_ID());
    pPed->SetPlayer(true);
    this->m_pPlayer = pPed;
    // we don't add player to list of entities handled because all of those get cleaned at end of proc. thus generating a issue where the player gets deleted from the world needing a restart.
}

CPed* CEntityHandler::CreatePed(ePedType type, Hash modelHash, CVector3 position, float heading)
{
    CPed* pPed = nullptr;
    pPed = CPed::CreatePed(type, modelHash, position, heading);
    this->m_Entities.push_back(pPed);
    return pPed;
}

CVehicle* CEntityHandler::CreateVehicle(Hash modelHash, CVector3 pos, float heading, bool isNetwork, bool bScriptHostVeh)
{
    CVehicle* pVeh{};
    pVeh = CVehicle::CreateVehicle(modelHash, pos, heading, isNetwork, bScriptHostVeh);
    this->m_Entities.push_back(pVeh);
    return pVeh;
}

void CEntityHandler::Destroy()
{
    for (auto it : m_Entities) {
        if (ENTITY::DOES_ENTITY_EXIST(it->GetHandle())) {
            ENTITY::DELETE_ENTITY(it->GetEntityPtr());
        }
        delete it; // we default to calling delete but we al
    }
    delete m_pPlayer; // delete player memory last. 
}
