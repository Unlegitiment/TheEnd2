#pragma once
//#include "LegitProject1\EndTypes\Triggers.h"
#ifdef USE_OLD_SCRIPT
#include <type_traits>
#define as(x, y) static_assert(x && y) 

#define CREATE_SCR_INSTANCE(TYPE) static_assert(std::is_base_of<fwScriptEnv, TYPE>::value && "fwScriptEnv not base of " #TYPE); auto* p##TYPE##Script = new TYPE; fwScriptMgr::Get().RegisterScript(p##TYPE##Script)
void CreateInstances();
#endif // USE_OLD_SCRIPT
#include "GTAVInfrastructure\ScriptEnv\Script.h"
class CInitial : public fwScriptEnv {
public:
    SCRIPT_INIT(CInitial);
    // Inherited via fwScriptEnv
    void OnTick() override
    {

    }
    void OnInit() override;
    void OnShutdown() override
    {
        LAGInterface::Writeln("Shutdown");
    }
};