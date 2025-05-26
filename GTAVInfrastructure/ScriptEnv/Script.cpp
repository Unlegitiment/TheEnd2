#include "Script.h"
#include <string>
fwScriptEnv::fwScriptEnv(const char* SCRNAME) : m_ScriptName{ "" }
{
	std::string scr(SCRNAME);
	auto final = scr.substr(6);
	strncpy_s(m_ScriptName, final.c_str(), MAX_SCR_NAME);
}
