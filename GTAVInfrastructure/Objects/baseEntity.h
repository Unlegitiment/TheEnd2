#pragma once
#include "LegitProject1\datattypes\vector.h"
#include "GTAVInfrastructure\SHV\natives.h"
using namespace ENTITY;
class baseEntity {
public:
	CVector3 GetPosition();
	float GetHeading();
	bool DoesEntityExist() { return DOES_ENTITY_EXIST(m_iHandle); }
	bool DoesEntityBelongToThisScript(bool p1) { return DOES_ENTITY_BELONG_TO_THIS_SCRIPT(this->m_iHandle, p1); }
	bool DoesEntityHaveDrawable() { return DOES_ENTITY_HAVE_DRAWABLE(this->m_iHandle); }
	bool DoesEntityHavePhysics() { return DOES_ENTITY_HAVE_PHYSICS(m_iHandle); }
	bool DoesEntityHaveAnimDirector() { return DOES_ENTITY_HAVE_ANIM_DIRECTOR(m_iHandle); }
	bool HasEntityAnimFinished(const char* animDict, const char* animName, int p3) { return HAS_ENTITY_ANIM_FINISHED(m_iHandle, animDict, animName, p3); }
	bool HasEntityBeenDamagedByAnyObject()	{ return HAS_ENTITY_BEEN_DAMAGED_BY_ANY_OBJECT(m_iHandle); }
	bool HasEntityBeenDamagedByAnyPed()		{ return HAS_ENTITY_BEEN_DAMAGED_BY_ANY_PED(m_iHandle); }
	

	int GetHandle();
	int* GetEntityPtr() { return &m_iHandle; }
private:
protected:
	int m_iHandle;
};