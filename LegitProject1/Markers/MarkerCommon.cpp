#include "MarkerCommon.h"

void DrawMarker(eMarkerType marker, CVector3 position, CVector3 Direction, CVector3 Rotation, CVector3 Scale, Color32 col, TextureGTAV* text, bool bobUpDwn, bool faceCam, bool rotate, bool drawOnEnter) {
	GRAPHICS::DRAW_MARKER(marker, position.GetX(), position.GetY(), position.GetZ(), Direction.GetX(), Direction.GetY(), Direction.GetZ(), Rotation.GetX(), Rotation.GetY(), Rotation.GetZ(), Scale.GetX(), Scale.GetY(), Scale.GetZ(), (int)col.GetR(), (int)col.GetG(), (int)col.GetB(), (int)col.GetA(), bobUpDwn, faceCam, 0, rotate, text ? text->GetDictionary() : nullptr, text ? text->GetName() : nullptr, drawOnEnter);
}

void DrawFreemodeMarker(CVector3 position, CVector3 scale, Color32 colour) {
	DrawMarker(MarkerTypeVerticalCylinder, position, { 0,0,0 }, { 0,0,0 }, scale, colour, nullptr);
}
