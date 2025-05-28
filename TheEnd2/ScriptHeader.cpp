#include "ScriptHeader.h"
#include "LegitProject1\UScript.h"
void CreateInstances()
{
	CREATE_SCR_INSTANCE(CTheEndScript);
	fwScriptMgr::Get().DbgPrntAll();
}