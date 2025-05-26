#include "Ped.h"
#include "LegitProject1\datattypes\vector.h"
#include "GTAVInfrastructure\SHV\natives.h"
CPed::CPed(int handle)
{
    this->m_iHandle = handle;
}

CPed* CPed::CreatePed(ePedType type, Hash modelHash, CVector3 position, float heading)
{
    return CreatePedEx(type, modelHash, position, heading, 1, 1);
}

CPed* CPed::CreatePedEx(ePedType type, Hash modelHash, CVector3 position, float heading, bool isNetPed, bool bShouldDespawn)
{
    CPed* pRet = nullptr;
    Ped Handle = PED::CREATE_PED(type, modelHash, position.GetX(), position.GetY(), position.GetZ(), heading, isNetPed, bShouldDespawn);
    pRet = new CPed(Handle);
    return pRet;
}

CVector3 CPed::GetPos()
{
    return ENTITY::GET_ENTITY_COORDS(m_iHandle, 1);
}
