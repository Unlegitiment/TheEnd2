#include "CGame.h"
#include "GTAVInfrastructure\SHV\natives.h"
#include "./TheEnd2/keyboard.h"
#include "LegitProject1\LInfras.h"
#include "GTAVInfrastructure\Hud\Obj\Scaleform.h"
#include "Objects\Headshots.h"
#include "ScriptEnv\ScriptProc.h"

void CGame::Init(void)
{
	CGame::sm_pEntityHandler = new CEntityHandler();
	CGame::sm_pWorld = new CGameWorld();
	CGame::sm_pHudHandler = new CHudHandler(); sm_pHudHandler->Init();
	CGame::sm_pGameScriptMgr = new CGameScriptMgr(); sm_pGameScriptMgr->LoadScripts();
	CGame::sm_pCheats = new CCheatMgr();
	CGame::sm_pCameraManager = new CCameraManager();
	LoadCheats();
}

void CGame::Update() {
	CGame::sm_pHudHandler->Update();
	CGame::sm_pCheats->Update();
	CGame::sm_pWorld->Update();
	CGame::sm_pCameraManager->Update();
	fwScriptMgr::Get().TickAll();
	CGame::DeathCheck();
	static bool isMenuOpen = false;
	if (IsKeyJustUp(VK_MULTIPLY)) {
		// which means were ripping from old projects lmao :/
		//chat I just fucking hate FrontEnd Menus
#if 0 
		CScaleform* scaleform = CGame::GetScaleformHandler().CreateScaleform("GTAV_ONLINE");
		if (!isMenuOpen) {
			scaleform->BeginMethod("SETUP_BIGFEED");
			scaleform->PushArgument(1); // rightAlign
			scaleform->EndMethod();
			scaleform->BeginMethod("HIDE_ONLINE_LOGO");
			scaleform->EndMethod();
			scaleform->BeginMethod("SET_BIGFEED_INFO");
			scaleform->PushArguments("footer", "textblk", 0, "", "", "Subtitle", "URL", "title", 0);
			scaleform->EndMethod();
			isMenuOpen = true;
		}
		else {
			GRAPHICS::DRAW_SCALEFORM_MOVIE_FULLSCREEN(scaleform->GetHandle(), 255, 255, 255, 255, 1);
		}
#endif
#if 0
		CFrontendScaleform* fntScaleform = CGame::GetScaleformHandler().CreateFrontScaleform(FE_MENU_VERSION_CORONA);
		if (isMenuOpen) {
			fntScaleform->CloseMenu();
			isMenuOpen = false;
			return;
		}
		isMenuOpen = true;
		fntScaleform->BeginMethodHeader("SHIFT_CORONA_DESC");
		fntScaleform->PushArguments(true, false);
		fntScaleform->EndMethod();
		fntScaleform->BeginMethodHeader("SET_HEADER_TITLE");
		fntScaleform->PushArguments("TITLE", false, "Subtitle", false);
		fntScaleform->EndMethod();
		fntScaleform->BeginMethodHeader("SHOW_HEADING_DETAILS");
		fntScaleform->PushArguments(false);
		fntScaleform->EndMethod();
		fntScaleform->BeginMethodHeader("SET_HEADING_DETAILS");
		fntScaleform->PushArguments("STR1", "STR2", "STR3", false);
		fntScaleform->EndMethod();
		fntScaleform->BeginMethodHeader("BUILD_MENU");
		fntScaleform->EndMethod();
		fntScaleform->BeginMethodHeader("SHOW_MENU");
		fntScaleform->PushArgument(true);
		fntScaleform->EndMethod(); 


		fntScaleform->BeginMethod("DISPLAY_DATA_SLOT");
		fntScaleform->PushArguments(3); // idk what the slot is
		fntScaleform->EndMethod();
		fntScaleform->BeginMethod("DISPLAY_DATA_SLOT");
		fntScaleform->PushArguments(0); // idk what the slot is
		fntScaleform->EndMethod();
		fntScaleform->BeginMethod("DISPLAY_DATA_SLOT");
		fntScaleform->PushArguments(1); // idk what the slot is
		fntScaleform->EndMethod();
#endif
	}
}

void CGame::Destroy()
{
	fwScriptMgr::Get().ShutdownAll();
	delete CGame::sm_pEntityHandler	;
	delete CGame::sm_pWorld			;
	delete CGame::sm_pHudHandler	;
	delete CGame::sm_pGameScriptMgr	;
	delete CGame::sm_pCheats		; 
	delete CGame::sm_pCameraManager ;
}

void CGame::DeathCheck()
{
	if (PLAYER::IS_PLAYER_PLAYING(PLAYER::PLAYER_ID()) && !PLAYER::IS_PLAYER_DEAD(PLAYER::PLAYER_ID())) {
		return; // this is if the player isn't actually dead.
	}
	else {
		DeathState state{};
		state.PLAYER_PED_ID = PLAYER::PLAYER_PED_ID();
		if(CGame::sm_pfnDeathScript) CGame::sm_pfnDeathScript(&state);
	}
}

void CGame::LoadCheats()
{
	sGameCommand deloadAllScr = { MISC::GET_HASH_KEY("ULOAD_SCR"),[&]()->void {
		CGame::GetScriptMgr().TerminateAllNonSHVThreads();
	} };
	sGameCommand reloadScripts = { MISC::GET_HASH_KEY("LOAD_SCR"), [&]()->void {
			CGame::GetScriptMgr().LoadScript("initial", eStackSize::FRIEND);
		} };
	sm_pCheats->GetBase()->Add(deloadAllScr);
	sm_pCheats->GetBase()->Add(reloadScripts);
}

void CGame::DefaultDeathHandler(const DeathState* pDeathState)
{
	sm_pGameScriptMgr->LoadScript("respawn_controller", eStackSize::DEFAULT); // I think its default? check maybe :)
}

void CGame::SetDeathHandler(DeathScript pFn)
{
	if (pFn == nullptr) {
		sm_pfnDeathScript = DefaultDeathHandler;
	}
	else {
		sm_pfnDeathScript = pFn;
	}
}
