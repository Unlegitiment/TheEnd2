#pragma once
#include <unordered_map>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <array>
#include "CelebTypes.h"
#include <LegitProject1\ui\Scaleform\ScaleformMgr.h>
class CHeistCelebRenderer {
public:
	CHeistCelebRenderer() { Init(); }
	void Init() {
		SetupInstances();
		m_pWall = new CWall(m_ScaleformInstances, m_MapData, WallId);
	}
	void Update() {
		RenderData();
	}
	CWall* GetWall() { /*LAGInterface::Writeln(__FUNCTION__ " 0x%p", m_pWall);*/ return m_pWall; }
private:
	HashMap<int, MapEntry_HeistCeleb> m_MapData;
	void RenderData() {
		GRAPHICS::DRAW_SCALEFORM_MOVIE_FULLSCREEN_MASKED(m_ScaleformInstances[BACKGROUND]->GetHandle(), m_ScaleformInstances[FOREGROUND]->GetHandle(), 255, 255, 255, 255);
		GRAPHICS::DRAW_SCALEFORM_MOVIE_FULLSCREEN(m_ScaleformInstances[BASE]->GetHandle(), 255, 255, 255, 255, 1); // yokay!
	}
	enum eScaleformIndex {
		BASE = 0,
		BACKGROUND = 1,
		FOREGROUND = 2,
		MAX_SCALEFORM_INSTANCES
	};
	bool SetupInstances() {
		const char* scl_base = "HEIST_CELEBRATION";
		const char* scl_base_bg = "HEIST_CELEBRATION_BG";
		const char* scl_base_fg = "HEIST_CELEBRATION_FG";
		m_ScaleformInstances[BASE] = CScaleformManager::Get()->GetScaleform(scl_base);
		m_ScaleformInstances[BACKGROUND] = CScaleformManager::Get()->GetScaleform(scl_base_bg);
		m_ScaleformInstances[FOREGROUND] = CScaleformManager::Get()->GetScaleform(scl_base_fg);
		return true;
	}
private:
	std::array<CScaleform*, MAX_SCALEFORM_INSTANCES> m_ScaleformInstances;
	int WallId = 0; // gotta do something new.
	CWall* m_pWall = nullptr;
};