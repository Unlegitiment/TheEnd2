#include <vector>
#include "ModelLoad.h"
#include <LegitProject1\LInfras.h>
// In this we actually couple to memory allocation stuff. 
// The responsibility shouldn't fall on this object to actually delete it. 
// I lwk hate my design but it should basically work as a way around the type.
class CStreamingMgr {
public:
	void Add(fwStreamObject* data) { // = new fwIPL() || new fwStreamObject(); // always dynamic alloc
		this->m_Object.push_back(data);
		data->Request();
	}
	int Find(fwStreamObject* data) {
		auto it = std::find(m_Object.begin(), m_Object.end(), data);
		if (it != m_Object.end()) {
			return (int)std::distance(m_Object.begin(), it); // i hate C++ mate.
		}
		return -1;
	}
	fwStreamObject* Get(int access) {
		return m_Object[access];
	}
	void Remove(fwStreamObject* objWhere) {
		int iSlot = Find(objWhere);
		if (iSlot == -1) {
			LAGInterface::Writeln("iSlot == -1 objWhere: 0x%p", objWhere);
			return;
		}
		m_Object.erase(m_Object.begin() + iSlot); // idk if this actually works like this?
	}
	void Remove(int where) {
		if (where > m_Object.size()) {
			LAGInterface::Writeln("where > m_Object.size() : %d > %d", where, m_Object.size());
			return;
		}
		m_Object.erase(m_Object.begin() + where);
	}
	void ClearAll() {
		for (int i = 0; i < m_Object.size(); i++) {
			Remove(i);
		}
		m_Object.clear();
	}
	~CStreamingMgr() {
		ClearAll();
	}
private:
	std::vector<fwStreamObject*> m_Object;
};