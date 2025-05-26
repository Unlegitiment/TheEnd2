#pragma once
class fwScriptEnv {
public:

    virtual void OnTick() = 0;
    virtual void OnInit() = 0;
    virtual void OnShutdown() = 0;
    char* GetScriptName() { return this->m_ScriptName; }
protected:
    static constexpr int MAX_SCR_NAME = 64;
    fwScriptEnv(const char* SCRNAME);
    char m_ScriptName[MAX_SCR_NAME];
};
#include "ScriptProc.h"
#include <typeinfo>
template<typename ScriptName>
class ScriptableBase : public fwScriptEnv {
public:
    static ScriptName* GetInstance() { static ScriptName instance = ScriptName(); return &instance; };
protected:
    ScriptableBase() : fwScriptEnv(typeid(ScriptName).name()){
        fwScriptMgr::Get().RegisterScript(this); // okie
    };
};

