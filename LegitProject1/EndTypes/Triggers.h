#pragma once
#include "LegitProject1/datattypes/vector.h"
#include "LegitProject1/datattypes/color.h"
#include "LegitProject1/datattypes/Bit.h"
#include "Markers.h"
#include "../LInfras.h"
enum eTriggerState : int{
	WITHIN_BOUNDS = BIT(0), // @Define - Object is within Marker bounds. This can be used to display HelpText if necessary.
	KEY_DOWN = BIT(1), // @Define - Key is pressed. Use KeyCodeTriggered member variable to get the corresponding KeyCode to perform an action.
	KEY_JUST_UP = BIT(2), // @Define - Key is released. Use KeyCodeTriggered to get corresponding Key-Code that was released
};
class CTriggerMarker;
struct TriggerCtx { // @Warn - Do not cache object. Object is altered every frame for markers.
	CTriggerMarker* Marker = nullptr;
	bool m_bIsWithinBounds = false;
	bool m_bIsKeyDown = false;
	bool m_bIsKeyJustUp = false;
	int KeyCodeTriggered = -1; // @Todo - Expand to all KeyCode values. Getting their states. 
};
class CTriggerMarker {
private:
	typedef void(*Trigger)(TriggerCtx*);
public:
	CTriggerMarker(Trigger trigger, CVector3 initPosition, CVector3 Scale);
	void SetTrigger(Trigger trigger) {
		this->m_T = trigger;
	}
	TriggerCtx* GetContext() { return &m_TriggerCtx; }
	void TriggerAction() {
		m_T(this->GetContext());
	}
	MarkerHandle GetMarker() { return this->m_Marker; }
	//@Nullsub - Does Nothing right now, As the marker doesn't need a specific render context.
	void RenderVisualMarker();
private:
	TriggerCtx m_TriggerCtx;
	MarkerHandle m_Marker;
	Trigger m_T;
};
typedef int TriggerHandle;
class CTriggers : public MyMgr<TriggerHandle, CTriggerMarker> {
public:
	CTriggers() : MyMgr<TriggerHandle, CTriggerMarker>(new CRand<TriggerHandle>()) {

	}
	void Update();
	void Destroy() {
		LAGInterface::Writeln("Deleting CTriggers instance");
		delete m_HandleGen;
	}
	~CTriggers() { Destroy(); }
};