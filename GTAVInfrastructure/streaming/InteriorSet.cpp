#include "InteriorSet.h"

void CMapData::Request()
{
    this->status = LOADING;
    for (auto& r : m_IPL) {
        LAGInterface::Writeln("Loading IPL: %s", r.GetIPLName());
        r.Request();
    }
    this->status = LOADED;
}

bool CMapData::IsLoaded()
{
    for (auto& r : m_IPL) {
        if (!r.IsLoaded()) {
            return false;
        }
    }
    return true;
}

void CMapData::Derequest()
{
	for (auto& r : m_IPL) {
		r.Derequest();
	}
    this->status = INACTIVE;
}
