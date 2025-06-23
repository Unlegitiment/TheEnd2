#pragma once
#include <unordered_map>
#include <string>
#include "Scaleform.h"
#include <GTAVInfrastructure\macros\WHILE.h>
//class CScaleformManager;
//extern CScaleformManager* g_pScaleformManager;
class CScaleformManager {
public:
	static inline CScaleformManager* sm_pInstance = nullptr;
	static void Init() {
		LAGInterface::Write(__FUNCTION__ " called ");
		//g_pScaleformManager = new CScaleformManager();
		sm_pInstance = new CScaleformManager();
		LAGInterface::Writeln(" g_pScaleformManager = 0x%p", sm_pInstance);
	}
	static CScaleformManager* Get() {
		return sm_pInstance;
	}
	static void Destroy() {
		delete sm_pInstance;
	}
	CScaleform* GetScaleform(const std::string& name) {
		auto it = m_Scaleforms.find(name);
		CScaleform* retScaleform = nullptr;
		if (it == m_Scaleforms.end()) { // we didn't find it.
			retScaleform = AddScaleform(name);
			if (!retScaleform) {
				logf("Failed to create Scaleform!");
				return nullptr;
			}
			logf("Scaleform not found adding : %s, id = %d", name.c_str(), retScaleform->GetHandle());
		}
		else {
			it->second.bIsInUse = true;
			retScaleform = it->second.pScaleform;
		}
		return retScaleform;
	}
	void Release(const std::string& name) {
		auto it = m_Scaleforms.find(name);
		if (it == m_Scaleforms.end()) {
			logf("Couldn't find Scaleform. Did you actually create it using CScaleform* CScaleformManager::GetScaleform(const std::string&)?");
			return;
		}
		it->second.bIsInUse = false;
	}
private:
	CScaleform* AddScaleform(const std::string name) {
		int handle = GRAPHICS::REQUEST_SCALEFORM_MOVIE(name.c_str());
		int m_Tries = 0;
		WHILE(!GRAPHICS::HAS_SCALEFORM_MOVIE_LOADED((int)handle)) {
			handle = GRAPHICS::REQUEST_SCALEFORM_MOVIE(name.c_str());
			m_Tries++;
			if (m_Tries > 10) {
				break;
			}
		}
		if (m_Tries >= 10) {
			logf("Failed to Add Scaleform. Maximum tries reached! Name: %s", name.c_str());
			return nullptr;
		}
		CScaleform* scaleform = new CScaleform(handle);
		m_Scaleforms.insert({ name, sScaleform(scaleform, true) });
		return scaleform;
	}
	struct sScaleform {
		sScaleform(CScaleform* scaleform, bool bUse) {
			this->pScaleform = scaleform;
			this->bIsInUse = bUse;
		}
		CScaleform* pScaleform = nullptr;
		bool bIsInUse = false;
	};
	std::unordered_map<std::string, sScaleform> m_Scaleforms;
};