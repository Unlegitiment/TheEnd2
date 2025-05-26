#pragma once
#include <vector>
//#include "LegitProject1\LInfras.h"
class fwScriptEnv;
class fwScriptMgr {
public:
	static fwScriptMgr& Get() { 
		static fwScriptMgr instance;  
		return instance; 
	}; 
	void RegisterScript(fwScriptEnv* pEnv);
	void InitAll();
	void TickAll();
	void ShutdownAll();
	void DbgPrntAll();
private:
	std::vector<fwScriptEnv*> m_Scripts;;
	fwScriptMgr() = default;
	~fwScriptMgr() = default;
	fwScriptMgr(const fwScriptMgr&) = delete;
	fwScriptMgr& operator=(const fwScriptMgr&) = delete;

	
};
