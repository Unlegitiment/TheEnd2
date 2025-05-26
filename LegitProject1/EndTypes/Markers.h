#pragma once
#include "LegitProject1/datattypes/MyHndlGen.h"
#include "Marker.h"
#include "../LInfras.h"
typedef int MarkerHandle;
class CMarkerMgr : public MyMgr<MarkerHandle, CMarker> {
public:
	CMarkerMgr() : MyMgr<MarkerHandle, CMarker>(new CRand<>()) {

	}
	void Update() {
		for (auto& [key,m] : m_Map) {
			m->Render();
		}
	}
	void Destroy() {
		delete m_HandleGen;
	}
	~CMarkerMgr() {
		LAGInterface::Writeln("CMarker Destructor called");

		Destroy();
	}
};