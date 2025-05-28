#include "InteriorSet.h"

void CInterior::Request()
{
    this->status = LOADING;
    for (auto& r : m_IPL) {
        LAGInterface::Writeln("Loading IPL: %s", r.GetIPLName());
        r.Request();
    }
    this->status = LOADED;
}

bool CInterior::IsLoaded()
{
    for (auto& r : m_IPL) {
        if (!r.IsLoaded()) {
            return false;
        }
    }
    return true;
}

void CInterior::Derequest()
{
	for (auto& r : m_IPL) {
		r.Derequest();
	}
    this->status = INACTIVE;
}
