#pragma once
#include "ModelLoad.h"
#include "LegitProject1/datattypes/allocs/memtrack.h"
class fwIPL : public fwStreamObject {
public:
	ALLOC_OVERRIDES(fwIPL);
	fwIPL(const char* Name) : m_aName(Name) {}
	// Inherited via fwStreamObject
	void Request() override;
	void Derequest() override;
	bool IsLoaded() override;
	const char* GetIPLName() const { return m_aName; }
	bool operator==(const fwIPL& other);
	~fwIPL() {
		Derequest();
	}
private:
	const char* m_aName;
};