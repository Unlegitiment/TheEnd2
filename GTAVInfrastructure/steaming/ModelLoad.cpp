#include "ModelLoad.h"
#include "GTAVInfrastructure\SHV\natives.h"
void CModelLoading::LoadModel(Hash hash)
{
	STREAMING::REQUEST_MODEL(hash);
	this->m_vHashLoaded.push_back(hash);
}

void CModelLoading::UnloadModel(Hash hash)
{
	if (!this->IsModelLoaded(hash)) { return; }
	auto it = Find(hash);
	this->m_vHashLoaded.erase(it);
	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
}
