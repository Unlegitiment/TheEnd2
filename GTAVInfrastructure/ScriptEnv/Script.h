#pragma once
class fwScriptEnv {
public:
    virtual void OnTick() = 0;
    virtual void OnInit() = 0;
    virtual void OnShutdown() = 0;
    const char* GetScriptName() { return this->m_ScriptName; }
protected:
    static constexpr int MAX_SCR_NAME = 64;
    fwScriptEnv(const char* SCRNAME);
    char m_ScriptName[MAX_SCR_NAME];
};
#include "ScriptProc.h"
#include <typeinfo>


 template<typename ScriptName>

class [[deprecated("ScriptableBaseV1 is no longer encouraged for use. For Scripts, use the Macro: SCRIPT_INIT()")]] ScriptableBaseV1 : public fwScriptEnv {
public:
    static ScriptName* GetInstance() { static ScriptName instance = ScriptName(); return &instance; };
protected:
    ScriptableBaseV1() : fwScriptEnv(typeid(ScriptName).name()){
    };
};

#define SCRIPT_INIT(__class_name__) public: static const char* GetScriptName_STATIC() { return #__class_name__ ; } static fwScriptEnv* CREATE() { LAGInterface::Writeln(#__class_name__":: static CREATE method called"); return new __class_name__(); } __class_name__() : fwScriptEnv(GetScriptName_STATIC()) {}
