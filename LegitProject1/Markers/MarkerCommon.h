#pragma once
#include <GTAVInfrastructure\SHV\natives.h>
#include <GTAVInfrastructure\SHV\enums.h>
#include <LegitProject1\datattypes\vector.h>
#include <LegitProject1\datattypes\color.h>
struct TextureGTAV {
	const char* Dictionary;
	const char* Name;
	TextureGTAV(const char* Dict, const char* Name) :Dictionary(Dict), Name(Name) {
		GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT(Dictionary, 1); // we need this immediately. 
	}
	const char* GetDictionary() { return Dictionary; }
	const char* GetName() { return Name; }
};
void DrawMarker(eMarkerType marker, CVector3 position, CVector3 Direction, CVector3 Rotation, CVector3 Scale, Color32 col, TextureGTAV* text, bool bobUpDwn = false, bool faceCam = false, bool rotate = false, bool drawOnEnter = false);
void DrawFreemodeMarker(CVector3 position, CVector3 scale = { 0.75f,0.75f,0.75f }, Color32 colour = { 93, 182, 229, 255 });