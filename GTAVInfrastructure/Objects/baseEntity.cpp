#include "baseEntity.h"
#include "GTAVInfrastructure\SHV\natives.h"
CVector3 baseEntity::GetPosition()
{
    return ENTITY::GET_ENTITY_COORDS(this->m_iHandle, 1); // typically the ped is alive lmaop its a debug parameter kekw
}

float baseEntity::GetHeading()
{
    return ENTITY::GET_ENTITY_HEADING(this->m_iHandle);
}

int baseEntity::GetHandle()
{
    return this->m_iHandle;
}
