#pragma once
#include "LegitProject1\EndTypes\Triggers.h"

#include <type_traits>
#define CREATE_SCR_INSTANCE(TYPE) static_assert(std::is_base_of<fwScriptEnv, TYPE>::value, "fwScriptEnv not base of " #TYPE); TYPE* p##TYPE##Script = new TYPE;
void CreateInstances();