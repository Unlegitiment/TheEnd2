#pragma once
#include <LegitProject1\LInfras.h>
class fwScriptEnv {
public:
    virtual void OnTick() = 0;
    virtual void OnInit() = 0;
    virtual void OnShutdown() = 0;
#ifdef USE_OLD_SCRIPT
    const char* GetScriptName() { return this->m_ScriptName; }
protected:
    static constexpr int MAX_SCR_NAME = 64;
    fwScriptEnv(const char* SCRNAME);
    char m_ScriptName[MAX_SCR_NAME];
#endif
    virtual const char* GetScriptName() = 0;
    virtual ~fwScriptEnv() {
        if (m_bFreed) {
            LAGInterface::Writeln("Double free happened 0x%p", this);
            return;
        }
        m_bFreed = true;
    }
private:
    bool m_bFreed = false;

};
#include "ScriptProc.h"
#include <typeinfo>

 template<typename ScriptName>

class [[deprecated("ScriptableBaseV1 is no longer encouraged for use. For Scripts, use the Macro: SCRIPT_INIT()")]] ScriptableBaseV1 : public fwScriptEnv {
public:
    static ScriptName* GetInstance() { static ScriptName instance = ScriptName(); return &instance; };
protected:
    ScriptableBaseV1() 
#ifdef USE_OLD_SCRIPT 
        : fwScriptEnv(typeid(ScriptName).name()) 
#endif // USE_OLD_SCRIPT
    {
        
    };
};

#define SCRIPT_INIT(__class_name__) public: static const char* GetScriptName_STATIC() { return #__class_name__ ; } template<typename... Args>static fwScriptEnv* CREATE(Args&&... args) { LAGInterface::Writeln(#__class_name__":: static CREATE method called"); return new __class_name__(std::forward<Args>(args)...); } const char* GetScriptName() { return GetScriptName_STATIC();}
