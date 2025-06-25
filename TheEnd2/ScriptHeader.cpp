#include "ScriptHeader.h"
//#include "LegitProject1\UScript.h"
#include "ShittySituationsDog.h"
void CreateInstances()
{
}
class Test : public fwScriptEnv {
    SCRIPT_INIT(Test);
    // Inherited via fwScriptEnv
    void OnTick() override
    {

    }
    void OnInit() override
    {
    }
    void OnShutdown() override
    {
        LAGInterface::Writeln("Help");
    }
};
void CInitial::OnInit()
{
    LAGInterface::Writeln("Init called For CInitial");
    CScriptRuntime::Get()->AddScriptAndCreate<Test>();
    CScriptRuntime::Get()->AddScriptAndCreate<CTest>(); // this gotta be stupiid
    LAGInterface::Writeln("Funky!");
    CScriptRuntime::Get()->TerminateScript(this);
    LAGInterface::Writeln("Funky2!");
}
