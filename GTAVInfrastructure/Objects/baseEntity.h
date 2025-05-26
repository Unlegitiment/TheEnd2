#pragma once
#include "LegitProject1\datattypes\vector.h"
class baseEntity {
public:
	CVector3 GetPosition();
	float GetHeading();
	int GetHandle();
	int* GetEntityPtr() { return &m_iHandle; }
private:
protected:
	int m_iHandle;
};