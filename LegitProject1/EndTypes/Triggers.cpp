#include "Triggers.h"
#include <GTAVInfrastructure/SHV/enums.h>
#include "EndWorld.h"
#include "LegitProject1/LInfras.h"
#include "TheEnd2/keyboard.h"
CTriggerMarker::CTriggerMarker(Trigger trigger, CVector3 initPosition, CVector3 Scale) : m_T(trigger)
{
	//int MarkerType, CVector3 Pos, CVector3 Dir, CVector3 Rot, CVector3 Scale, Color32 rgba,const char* TextureDict, const char* TextureName
	this->m_Marker = CEndWorld::GetMarkerMgr()->Allocate(
		MarkerTypeVerticalCylinder, // MarkerType -int 
		initPosition,  // Pos - V3
		CVector3{ 0.0f,0.0f,0.0f }, //Dir -V3
		CVector3{ 0.0f,0.0f,0.0f }, //Rot -V3
		Scale, //Scale -V3
		Color32{ 93, 182, 229, 255 }, //Color -Col32
		nullptr, nullptr // TextDict -o 
	);
}

void CTriggerMarker::RenderVisualMarker()
{}

void CTriggers::Update()
{
	CVector3 playerPos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
	for (auto& [key, v] : m_Map) {
		auto* Marker = CEndWorld::GetMarkerMgr()->Get(v->GetMarker());
		bool isPlayerWithinMarker = Marker->IsWithin(playerPos); 
		v->GetContext()->Marker = v;
		if (isPlayerWithinMarker == true) {
			v->GetContext()->m_bIsWithinBounds = true; // Ideally we'd fire off an Event but right now we should be ok :/
		}
		for (int i = 0; i < 255; i++) {
			if (IsKeyJustUp(i)) {
				v->GetContext()->m_bIsKeyJustUp = true;
				v->GetContext()->KeyCodeTriggered = i;
			}
			else if (IsKeyDown(i)) {
				v->GetContext()->m_bIsKeyDown = true;
				v->GetContext()->KeyCodeTriggered = i;
			}
		}
		v->TriggerAction();
		v->GetContext()->KeyCodeTriggered = -1;
		v->GetContext()->m_bIsKeyDown = false;
		v->GetContext()->m_bIsKeyJustUp= false;
		v->GetContext()->m_bIsWithinBounds = false;
		v->GetContext()->Marker = nullptr; // convert these to Reset inside of context();

	}
}
