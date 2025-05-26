#include "GameScripts.h"
#include "GTAVInfrastructure\SHV\natives.h"
#include "LegitProject1\LInfras.h"
void CGameScriptMgr::LoadScripts()
{
	int threadId = 0;
	SCRIPT::SCRIPT_THREAD_ITERATOR_RESET();
	threadId = SCRIPT::SCRIPT_THREAD_ITERATOR_GET_NEXT_THREAD_ID();
	while (SCRIPT::IS_THREAD_ACTIVE(threadId)) { // lemme check with old progress cause this is iterating scripts and I want to be thorough
		sScript* script = new sScript();
		strncpy_s(script->ScriptName, SCRIPT::GET_NAME_OF_SCRIPT_WITH_THIS_ID(threadId), 32); //TRUNC?
		script->id = threadId;
		this->m_Scripts.push_back(script); 
		threadId = SCRIPT::SCRIPT_THREAD_ITERATOR_GET_NEXT_THREAD_ID();
		WAIT(0);
	}
	for (auto& scr : m_Scripts) {
		LAGInterface::Write("Script Loaded %s\n", scr->ScriptName); // idk if this would work because the scripts were weird from last time :/
	}
}

void CGameScriptMgr::LoadGameScripts()
{
	int threadId = 0;
	SCRIPT::SCRIPT_THREAD_ITERATOR_RESET();
	threadId = SCRIPT::SCRIPT_THREAD_ITERATOR_GET_NEXT_THREAD_ID();
	while (SCRIPT::IS_THREAD_ACTIVE(threadId)) { // lemme check with old progress cause this is iterating scripts and I want to be thorough
		if (MISC::GET_HASH_KEY(SCRIPT::GET_NAME_OF_SCRIPT_WITH_THIS_ID(threadId))!=SCRIPT::GET_HASH_OF_THIS_SCRIPT_NAME()) {
			sScript* script = new sScript();
			strncpy_s(script->ScriptName, SCRIPT::GET_NAME_OF_SCRIPT_WITH_THIS_ID(threadId), 32); //TRUNC?
			script->id = threadId;
			this->m_Scripts.push_back(script);
		}
		threadId = SCRIPT::SCRIPT_THREAD_ITERATOR_GET_NEXT_THREAD_ID();
		WAIT(0);
	}
	for (auto& scr : m_Scripts) {
		LAGInterface::Write("Script Loaded %s\n", scr->ScriptName); // idk if this would work because the scripts were weird from last time :/
	}
}

sScript* CGameScriptMgr::LoadScript(const char* scrName, int stackSize)
{
	SCRIPT::REQUEST_SCRIPT(scrName);
	unsigned int tries = 0;
	while (!SCRIPT::HAS_SCRIPT_LOADED(scrName) && tries <= 15) {
		tries++;
		WAIT(0);
	}
	if (tries == 15) {
		LAGInterface::Write(RRed"Failed to Load Script : %s" RNorm, scrName);
		return nullptr;
	}
	int thread = SYSTEM::START_NEW_SCRIPT(scrName, stackSize);
	sScript* script = new sScript();
	script->id = thread;
	strncpy_s(script->ScriptName, scrName, 32);
	this->m_Scripts.push_back(script);
	return nullptr;
}

sScript* CGameScriptMgr::FindScript(const char* str)
{
	Hash h = MISC::GET_HASH_KEY(str);
	int val = SCRIPT::GET_NUMBER_OF_THREADS_RUNNING_THE_SCRIPT_WITH_THIS_HASH(h);
	if (val == 0) {
		return nullptr;
	}
	else {
		this->m_Scripts.clear(); // clear scripts so we don't add unnecessary  scripts onto the runtime lmao
		LoadScripts(); // verify we've collected all the scripts in the program then force scripts to load
		for (auto& scr : m_Scripts) {
			if (MISC::GET_HASH_KEY(scr->ScriptName) == h) {
				return scr;
			} // i kinda hate the name logic lmao
		}
		return nullptr;
	}
}

void CGameScriptMgr::TerminateAllNonSHVThreads()
{
	for (auto& it : m_Scripts) {
		if (MISC::GET_HASH_KEY(it->ScriptName) != SCRIPT::GET_HASH_OF_THIS_SCRIPT_NAME()) { // right now all SHV threads use "AudioTest" as a script pseudoname but else it could be bad lmao.
			SCRIPT::TERMINATE_THREAD(it->id);
			delete it;
		}
	}
}

void CGameScriptMgr::UnloadScript(int id)
{
	sScript* ptr = FindScript(SCRIPT::GET_NAME_OF_SCRIPT_WITH_THIS_ID(id));
	if (ptr == nullptr) {
		LAGInterface::Write("[SCRIPT] Script Doesn't exist lmao get fucked ;\\");
		return;
	}
	//auto it = std::find(m_Scripts.begin(), m_Scripts.end(), *ptr);
	//if (it == m_Scripts.end()) {
	//	idkinterfacesmth::GetNetLogger()->Write("[SCRIPT] Script Doesn't exist in list lmao get fucked ;\\");
	//	return;
	//}
	//m_Scripts.erase(it);
	//SCRIPT::TERMINATE_THREAD(id);
}

void CGameScriptMgr::UnloadScript(sScript* sc)
{
}

void CGameScriptMgr::UnloadScript(const char* scrName)
{
}

void CGameScriptMgr::Destroy()
{
	for (auto& scr : m_Scripts) {
		delete scr;
	}
}
