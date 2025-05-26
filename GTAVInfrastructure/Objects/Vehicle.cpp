#include "Vehicle.h"
#include "GTAVInfrastructure\SHV\natives.h"
CVehicle::CVehicle(int& handle)
{
    this->m_iHandle = handle; // i wonder if this opens to weird behavior?
}

CVehicle* CVehicle::CreateVehicle(Hash modelHash, CVector3 pos, float heading, BOOL isNetwork, BOOL bScriptHostVeh)
{
    int handle = VEHICLE::CREATE_VEHICLE(modelHash, pos.GetX(), pos.GetY(), pos.GetZ(), heading, isNetwork, bScriptHostVeh, 1); // p7 is a guess?
    CVehicle* pVeh = new CVehicle(handle);
    
    return pVeh;
}

void CVehicle::DeleteVehicle(CVehicle* vehcle)
{
    VEHICLE::DELETE_VEHICLE(&vehcle->m_iHandle);
    delete vehcle;
}
