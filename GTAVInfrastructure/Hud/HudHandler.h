#pragma once
#include "Obj\HudDrawable.h"
#include <vector>
class CRect;
class CText;
class CScaleform;
class CScaleformHandler;
class CGameTextHelper;
class CHudHandler {
public:
	void Init();
	CRect* CreateRect(CVector3 pos, Color32 col, float width, float height);
	CText* CreateText(const char* string, CVector3 pos, Color32 col, void* justification, float scale, bool hasOutline, int font);
	CScaleformHandler* GetScaleformHandler();
	CGameTextHelper* GetGameText();
	void Update();
	void Destroy();
private:
	CScaleformHandler* m_ScaleformHandler;
	CGameTextHelper* m_TextHelper;
	std::vector<fwHudDrawable*> m_vDrawList;
};