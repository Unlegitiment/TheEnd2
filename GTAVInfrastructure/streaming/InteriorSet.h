#pragma once
#include "ModelLoad.h"
#include "IPL.h"
#include "LegitProject1/LInfras.h"
#include "LegitProject1/datattypes/allocs/memtrack.h"
//Sometimes whole interiors need to be loaded through the use of single IPLS. This is typically seen in more massive interiors. For example the Doomsday Bunker, Arena, etc.


class CInterior : public fwStreamObject{
	ALLOC_OVERRIDES(CInterior);
	CInterior() {

	}
	void Add(const fwIPL ipl) {
		this->m_IPL.push_back(ipl);
	}
	bool DoesExist(fwIPL ipl) {
		LAGInterface::Writeln(__FUNCTION__" called with ipl: %s", ipl.GetIPLName());

		return std::find(m_IPL.begin(), m_IPL.end(), ipl) != m_IPL.end();
	}
	std::vector<fwIPL>::iterator GetAt(const fwIPL ipl) {
		LAGInterface::Writeln(__FUNCTION__" called with ipl: %s", ipl.GetIPLName());
		return std::find(m_IPL.begin(), m_IPL.end(), ipl);
	}
	void Remove(const fwIPL ipl) {
		LAGInterface::Writeln(__FUNCTION__" called with ipl: %s", ipl.GetIPLName());
		if (!DoesExist(ipl)) {
			LAGInterface::Writeln(__FUNCTION__" called with ipl: %s : IPL Does not exist inside of interior set", ipl.GetIPLName());
			return;
		}
		m_IPL.erase(GetAt(ipl));

	}
	// Inherited via fwStreamObject
	void Request() override;
	bool IsLoaded() override;
	void Derequest() override;
	~CInterior() {
		LAGInterface::Writeln(__FUNCTION__" called");
		Derequest();
	}
private:
	std::vector<fwIPL> m_IPL;
};
class InteriorBuilder {
public:
	InteriorBuilder& Add(const fwIPL ipl) {
		this->interior->Add(ipl);
		return *this;
	}
	CInterior* Build() { return interior; } // I think this copies? 
private:
	CInterior* interior = new CInterior();
};