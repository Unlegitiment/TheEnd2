#include "ScriptProc.h"
#include "Script.h"
#include "LegitProject1\LInfras.h"
void fwScriptMgr::InitAll()
{

	for (auto* scr : m_Scripts) {
		scr->OnInit();
	}
}

void fwScriptMgr::RegisterScript(fwScriptEnv* pEnv) {
	if (pEnv == nullptr) {
		LAGInterface::Write("Failed to Register Script Script is Null!\n"); 
		return;
	}
	LAGInterface::Write("Regist Script 1 : 0x%p\n", pEnv);
	this->m_Scripts.push_back(pEnv);
	LAGInterface::Write("Regist Script 2\n");
	//pEnv->OnInit();
	LAGInterface::Write("Regist Script 3\n");

}

void fwScriptMgr::TickAll()
{
	for (auto* scr : m_Scripts) {
		scr->OnTick();
	}
}

void fwScriptMgr::ShutdownAll()
{
	for (auto* scr : m_Scripts) {
		scr->OnShutdown();
	}
	for (auto* scr : m_Scripts) {
		delete scr;
	}
}

void fwScriptMgr::DbgPrntAll()
{
	for (auto* scr : m_Scripts) {
		LAGInterface::Write(scr->GetScriptName());
		LAGInterface::Write("\n");
	}
}
