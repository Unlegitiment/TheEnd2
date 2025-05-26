#pragma once
#include "LegitProject1\datattypes\vector.h"
#include "LegitProject1\datattypes\color.h"
#include <GTAVInfrastructure\SHV\enums.h>
#include "..\LInfras.h"
class CMarker {
public:
	CMarker(int MarkerType, CVector3 Pos, CVector3 Dir, CVector3 Rot, CVector3 Scale, Color32 rgba, const char* TextureDict, const char* TextureName) : mMarkerType(MarkerType), m_Col(rgba), m_Pos(Pos), m_Dir(Dir), m_Rot(Rot), m_Scale(Scale), TextureDict(TextureDict), TextureName(TextureName) {
		LAGInterface::Writeln("CMarker Constructor called");
	}
	void Render(bool bob = false, bool faceCam = true, bool drawOnEnter = false);
	int GetMarkerType() const { return mMarkerType; }
	void SetMarkerType(int val) { mMarkerType = val; }
	CVector3 Pos() const { return m_Pos; }
	void Pos(CVector3 val) { m_Pos = val; }
	CVector3 Rot() const { return m_Rot; }
	void Rot(CVector3 val) { m_Rot = val; }
	CVector3 Scale() const { return m_Scale; }
	void Scale(CVector3 val) { m_Scale = val; }
	CVector3 Dir() const { return m_Dir; }
	void Dir(CVector3 val) { m_Dir = val; }
	Color32 Col() const { return m_Col; }
	void Col(Color32 val) { m_Col = val; }
	const char* GetTextureDict() const { return TextureDict; }
	void SetTextureDict(const char* val) { TextureDict = val; }
	const char* GetTextureName() const { return TextureName; }
	void SetTextureName(const char* val) { TextureName = val; }
	bool IsWithin(const CVector3& vec) {
		return this->m_Pos.DistNoZ(vec) < m_Scale.GetX(); //not quite sure but meh. 
	}
private:
	int mMarkerType;
	CVector3 m_Pos, m_Rot, m_Scale, m_Dir;
	Color32 m_Col;
	const char* TextureDict, * TextureName;
};