#pragma once
#include "baseEntity.h"
class CVehicle : public baseEntity{
public:
	explicit CVehicle(int& handle);
	static CVehicle* CreateVehicle(Hash modelHash, CVector3 pos, float heading, BOOL isNetwork, BOOL bScriptHostVeh);
	static void DeleteVehicle(CVehicle* vehcle);
private:
};