#pragma once
#include "LegitProject1\EndTypes\Triggers.h"

#include <type_traits>
#define as(x, y) static_assert(x, y) 

#define CREATE_SCR_INSTANCE(TYPE) as(std::is_base_of<fwScriptEnv, TYPE>::value, "fwScriptEnv not base of " #TYPE); auto* p##TYPE##Script = new TYPE; fwScriptMgr::Get().RegisterScript(p##TYPE##Script)
void CreateInstances();