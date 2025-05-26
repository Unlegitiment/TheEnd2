#pragma once
#include "GTAVInfrastructure\ScriptEnv\Script.h"
#include "TheEnd2\keyboard.h"
#include "GTAVInfrastructure\World\World.h"
#include "EndTypes\EndWorld.h"
#include "LInfras.h"
#include "GTAVInfrastructure\SHV\natives.h"
class CTheEndScript : public ScriptableBase<CTheEndScript> {
private: 
public: 
    CTheEndScript() : ScriptableBase() { // @TODO: Create some way to create instances more nicely. Since ScriptableBase basically just adds it to the list rn. 
        
    }
    static void DebugPrint(TriggerCtx* tx) {
        LAGInterface::Writeln("Keycode triggered: %d", tx->KeyCodeTriggered);
        LAGInterface::Writeln("IsKeyDown: %d, IsKeyUp: %d, IsWithin: %d", tx->m_bIsKeyDown, tx->m_bIsKeyJustUp, tx->m_bIsWithinBounds);
    }
    static void AptTrigger(TriggerCtx* ctx) {
        if (ctx->m_bIsWithinBounds) {
			HUD::BEGIN_TEXT_COMMAND_DISPLAY_HELP("STRING");
			HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME("Press ~INPUT_CONTEXT~ to enter.");
			HUD::END_TEXT_COMMAND_DISPLAY_HELP(0, 0, 0, -1);
        }
    }
    // Inherited via ScriptableBase
    void OnTick() override
    {
        CEndWorld::Update();
        int worldState = CGameWorld::GetWorldState();
        if (IsKeyJustUp(VK_ADD)) {
            if (worldState == CAYO) {
                CGameWorld::SetWorldState(LOS_SANTOS); // flip worldstate between ls and cayo
            }
            else {
                CGameWorld::SetWorldState(CAYO);
            }
        }
        if (IsKeyJustUp(VK_DIVIDE)) {
        }
    }

    void OnInit() override
    {
        CEndWorld::Init();
        TriggerHandle handle = CEndWorld::GetTriggers()->Allocate(AptTrigger, CVector3{ -778.6876, 312.9487, 84.6982 - .25 }, CVector3{ 0.75, 0.75, 0.75 });
        LAGInterface::Writeln("Init Called for CTheEndScript");
        DLC::ON_ENTER_MP();
    }

    void OnShutdown() override
    {
        CEndWorld::Destroy();
        LAGInterface::Writeln("Shutdown Called for CTheEndScript");
    }
private:
};


