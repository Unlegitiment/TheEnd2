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
	using Functor = fwScriptEnv * (*)();

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

		m_Script.push_back({ id, scriptInstance });
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
		for (auto it = m_Script.begin(); it != m_Script.end(); it++) {
			if (it->script == Id && it->script != nullptr) {
				//it->script->OnShutdown();
				//delete it->script; -- You can't delete here because the iterator state gets changed. Do not call in a loop lmao. 
				m_Script.erase(it);
				break;
			}
		}
		LAGInterface::Writeln(__FUNCTION__" called 0x%p", Id);
	}
	//Method is broken. Not sure why?
	void TerminateScript(int Id) {
		for (auto it = m_Script.begin(); it != m_Script.end(); it++) {
			if (it->Id == Id) {
				m_Script.erase(it);
				return; // I don't return thats why lmao
			}
		}
	}
	~CScriptEnv() {
		LAGInterface::Writeln("~CScriptEnv");
		for (auto& script : m_Script) {
			TerminateScript(script.script);
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
	template<typename T, typename... Args>
	fwScriptEnv* AddScriptAndCreate(Args&&... args) {
		static_assert(std::is_base_of<fwScriptEnv, T>::value && "[SCRIPT_RUNTIME] " __FUNCTION__ ": Script Must be a Sub-type of fwScriptEnv!");
		fwScriptEnv* retVal = nullptr;
		if (sizeof...(Args) < 0) {
			m_ScriptRegistry->AppendScript(T::GetScriptName_STATIC(), T::CREATE, true);
			retVal = CreateScriptNoArgs(T::GetScriptName_STATIC());
			LAGInterface::Writeln("Script created with no VarArgs");
		}
		else {
			m_ScriptRegistry->AppendScript(T::GetScriptName_STATIC(), nullptr, true);
			retVal = CreateScript<T>(std::forward<Args>(args)...);
			LAGInterface::Writeln("Script created with VarArgs");
		}
		return retVal;
	}
	template<typename T, typename... Args> fwScriptEnv* CreateScript(Args&&... args) {
		fwScriptEnv* ptr = T::CREATE(std::forward<Args>(args)...);
		this->m_Scripts->AddScriptToRuntime(ptr);
		return ptr;
	}
	fwScriptEnv* GetScriptWithId(int Id) {

	}
	fwScriptEnv* CreateScriptNoArgs(const char* script) {
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
		if (!script) return;
		m_Scripts->TerminateScript(script);
		LAGInterface::Writeln(__FUNCTION__" after terminate ");
		if (removeFromRegistry) {
			m_ScriptRegistry->RemoveScriptFromRegistry(script->GetScriptName());
		}
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
	void ShutdownAllScripts() {
		std::vector<fwScriptEnv*> m_KillList;
		for (auto& scr : m_Scripts->m_Script) {
			m_KillList.push_back(scr.script);
			scr.script->OnShutdown();
		}
		for (auto* scr : m_KillList) {
			delete scr;
		}
		return;
	}
	~CScriptRuntime() {
		//LAGInterface::Writeln(__FUNCTION__" deconstructor start");
		ShutdownAllScripts();
		//LAGInterface::Writeln(__FUNCTION__" deconstructor start 1");
		delete m_ScriptRegistry;
		//LAGInterface::Writeln(__FUNCTION__" deconstructor start 2");
		delete m_Scripts;
		//LAGInterface::Writeln(__FUNCTION__" deconstructor start 3");
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