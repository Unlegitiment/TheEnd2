#pragma once
#include <vector>
#include <unordered_map>
#include <LegitProject1\datattypes\Rand.h>
#include "Script.h"
//#include "LegitProject1\LInfras.h"
#ifdef USE_OLD_SCRIPT
class [[deprecated("fwScriptMgr has been replaced by CScriptRuntime.")]] fwScriptMgr {
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
#endif
//Registry of scripts
class CScriptRegistry {
public:
	friend class CScriptRuntime;
	using Functor = fwScriptEnv* (*)();
	bool AppendScript(const char* Script, Functor CreationFunc, bool overrideExisting = false) {
		auto it = m_Scripts.find(Script);
		if (it != m_Scripts.end()) { // this script already has a creation func
			if (overrideExisting) {
				it->second = CreationFunc;
				return true;
			}
			return false;
		}
		this->m_Scripts.insert({ Script, CreationFunc });
		return true;
	}
	Functor GetCreationFromName(const char* name) {
		auto it = m_Scripts.find(name);
		if (it == m_Scripts.end()) {
			LAGInterface::Writeln(__FUNCTION__"erm oopsies we didn't find it :3");
			return nullptr; // oopsie!
		}
		LAGInterface::Writeln(__FUNCTION__" found script");
		return it->second;
	}
	bool IsScriptInRegister(const char* script) {
		return m_Scripts.find(script) != m_Scripts.end();
	}
	void RemoveScriptFromRegistry(const char* script) {

	}
private:
	std::unordered_map<const char*, Functor> m_Scripts;
};

class CScriptEnv {
public:
	friend class CScriptRuntime;
	int AddScriptToRuntime(fwScriptEnv* scriptInstance) {
		LAGInterface::Writeln(__FUNCTION__" called 0x%p", scriptInstance);
		int id = CRand<int>().GenerateRandom(); // yk this might spit the same number out lmao because device gets init'd everytime. I wonder.
		
		m_Script.push_back({id, scriptInstance});
		scriptInstance->OnInit();
		return id;
	}
	fwScriptEnv* GetScriptFromId(int id) {
		for (auto& sc : m_Script) {
			if (sc.Id == id) {
				return sc.script;
			}
		}
		return nullptr;
	}
	void Tick() {
		for (auto& script : m_Script) {
			script.script->OnTick();
		}
	}
	bool DoesIdExist(int Id) {
		for (auto& m : m_Script) { // performance bullshit be damned. I don't care im just testing.
			if (m.Id == Id) {
				return true;
			}
		}
		return false;
	}
	void TerminateScript(fwScriptEnv* Id) {
		LAGInterface::Writeln(__FUNCTION__"called");
		for (auto it = m_Script.begin(); it != m_Script.end(); it++) {
			if (it->script == Id) {
				LAGInterface::Writeln(__FUNCTION__" script found");
				m_Script.erase(it);
				LAGInterface::Writeln(__FUNCTION__" script found");
				return;
			}
		}
	}
	void TerminateScript(int Id) {
		for (auto it = m_Script.begin(); it != m_Script.end(); it++) {
			if (it->Id == Id) {
				m_Script.erase(it);
			}
		}
	}
	~CScriptEnv() {
		for (auto& script : m_Script) {
			TerminateScript(script.Id);
		}
	}
private:
	struct sScript {
		int Id = 0;
		fwScriptEnv* script;
	};
	std::vector<sScript> m_Script;
};
class CScriptRuntime {
public:
	template<typename T>
	void AddScriptToRegistry(bool create = false) {
		static_assert(std::is_base_of<fwScriptEnv, T>::value && "[SCRIPT_RUNTIME] " __FUNCTION__ ": Script Must be a Sub-type of fwScriptEnv!");
		m_ScriptRegistry->AppendScript(T::GetScriptName_STATIC(), T::CREATE, true);
		if (create) {
			LAGInterface::Writeln("Create specified as true");
			CreateScript(T::GetScriptName_STATIC());
			LAGInterface::Writeln("Create specified as true");

		}
	}
	fwScriptEnv* CreateScript(const char* script) {
		LAGInterface::Writeln("[NEW_SCRIPT_ENV] Create Script: %s", script);

		auto func = m_ScriptRegistry->GetCreationFromName(script);
		if (func == nullptr) {
			LAGInterface::Writeln("[NEW_SCRIPT_ENV] Script not registered. Impossible to find creation: %s", script);
			return nullptr;
		}
		//LAGInterface::Writeln("[NEW_SCRIPT_ENV] Script Exists: %s fNPtr: 0x%p", script, func);
		fwScriptEnv* ptr = func();
		//LAGInterface::Writeln("[NEW_SCRIPT_ENV] Script Created: 0x%p", ptr);
		this->m_Scripts->AddScriptToRuntime(ptr);
		return ptr;
	}
	void TerminateScript(fwScriptEnv* script, bool removeFromRegistry = false) {
		m_Scripts->TerminateScript(script);
		//LAGInterface::Writeln(__FUNCTION__" after terminate ");

		if (removeFromRegistry) {
			m_ScriptRegistry->RemoveScriptFromRegistry(script->GetScriptName());
		}
		delete script; // I think that this is a sufficient problem
	}
	void PrintAll() {
		LAGInterface::Writeln("Script Registry:");
		for (auto& [key, val] : m_ScriptRegistry->m_Scripts) {
			LAGInterface::Writeln("\t%s", key);
		}
		LAGInterface::Writeln("Script Instances: Size = %d", m_Scripts->m_Script.size());
		for (auto& val : m_Scripts->m_Script) {
			LAGInterface::Writeln("\tsScript { id: %d fwScriptEnv: 0x%p { ScrName: %s } } ", val.Id, val.script, val.script->GetScriptName());
		}
	}
	void Update() {
		this->m_Scripts->Tick();
	}
	~CScriptRuntime() {
		delete m_ScriptRegistry;
		for (auto& scr : m_Scripts->m_Script) {
			delete scr.script;
		}
		delete m_Scripts;
		
	}
	static void Init() {
		sm_pRuntime = new CScriptRuntime();
	}
	static CScriptRuntime* Get() { return sm_pRuntime; }
	static void Shutdown() {
		delete sm_pRuntime;
	}
private:
	CScriptEnv* m_Scripts = new CScriptEnv();
	CScriptRegistry* m_ScriptRegistry = new CScriptRegistry();
	static inline CScriptRuntime* sm_pRuntime = nullptr;
};