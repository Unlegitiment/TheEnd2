/*
	THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
				http://dev-c.com			
			(C) Alexander Blade 2015
*/

#include "GTAVInfrastructure\SHV\main.h"
#include "script.h"
#include "keyboard.h"
#include "LegitProject1\LInfras.h"
#include "GTAVInfrastructure\ScriptEnv\ScriptProc.h"
#include "GTAVInfrastructure\Application.h"
BOOL APIENTRY DllMain(HMODULE hInstance, DWORD reason, LPVOID lpReserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		scriptRegister(hInstance, ScriptMain);
		keyboardHandlerRegister(OnKeyboardMessage);
		break;
	case DLL_PROCESS_DETACH:
		CScriptRuntime::Shutdown();
		LAGInterface::Destroy();
		scriptUnregister(hInstance);
		keyboardHandlerUnregister(OnKeyboardMessage);
		break;
	}		
	return TRUE;
}