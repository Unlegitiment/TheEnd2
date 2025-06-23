#include "ScriptHeader.h"
#include "LegitProject1\UScript.h"
#include "ShittySituationsDog.h"
void CreateInstances()
{
	//CREATE_SCR_INSTANCE(CTheEndScript);
	//CREATE_SCR_INSTANCE(CTest);
	//fwScriptMgr::Get().DbgPrntAll();
}

void CInitial::OnInit()
{
    LAGInterface::Writeln("Init called For CInitial");
    CScriptRuntime::Get()->AddScriptToRegistry<CTest>(true); // this gotta be stupiid
    LAGInterface::Writeln("Funky!");
    CScriptRuntime::Get()->TerminateScript(this);
    LAGInterface::Writeln("Funky2!");
}
