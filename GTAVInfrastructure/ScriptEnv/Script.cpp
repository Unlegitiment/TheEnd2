#include "Script.h"
#include <string>
#ifdef USE_OLD_SCRIPT
fwScriptEnv::fwScriptEnv(const char* SCRNAME) : m_ScriptName{ "" }
{
	//LAGInterface::Writeln("fwScriptEnv constructor called called");
	//std::string scr(SCRNAME);
	//auto final = scr.substr(6);
	strncpy_s(m_ScriptName, SCRNAME, MAX_SCR_NAME);
	//LAGInterface::Writeln("fwScriptEnv constructor called called");

}

#endif //USE_OLD_SCRIPT