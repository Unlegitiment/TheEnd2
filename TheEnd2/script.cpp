#include "GTAVInfrastructure\SHV\natives.h"
#include "keyboard.h"
#include "GTAVInfrastructure\Application.h"
#include <string>
#include <ctime>
#include "GTAVInfrastructure\CGame.h"
#include "LegitProject1\LInfras.h"
#include <type_traits>
#pragma warning(disable : 4244 4305) // double <-> float conversions
// @todo: logger completely breaks game right now. not sure why?

#include "ScriptHeader.h"
#include <map>
#include "GTAVInfrastructure\Hud\HudComponents.h"
#include "GTAVInfrastructure\macros\WHILE.h"

namespace CDumpUtil {
    void DumpWeatherHash() {
        std::vector<const char*> v = {
            "EXTRASUNNY",
            "CLEAR       ",
            "CLOUDS      ",
            "SMOG        ",
            "FOGGY       ",
            "OVERCAST    ",
            "RAIN        ",
            "THUNDER     ",
            "CLEARING    ",
            "NEUTRAL     ",
            "SNOW        ",
            "BLIZZARD    ",
            "SNOWLIGHT   ",
            "XMAS        ",
            "HALLOWEEN   ",
            "RAIN_HALLOWEEN",
            "SNOW_HALLOWEEN" 
        };
        for (int i = 0; i < v.size(); i++) {
            LAGInterface::Writeln("%s : %lu", v[i], MISC::GET_HASH_KEY(v[i]));
        }
    }
};

static void NewInit() {
    CScriptRuntime::Get()->AddScriptToRegistry<CInitial>(true);
}
void main()
{
    //CRankBar::GetRawBar().Load();    
    CApplication* g_App = new CApplication();
    g_App->RegisterFuncStart(LAGInterface::Init);
    g_App->RegisterFuncStart(CGame::Init);
    g_App->RegisterFuncUpdate(CGame::Update);
    g_App->RegisterFuncDestroy(CGame::Destroy);
    g_App->Init();
    LAGInterface::GetFileLogger()->Write("test\n");
    CDumpUtil::DumpWeatherHash();
    //CreateInstances();
    CScriptRuntime::Get()->Init(); // init before.
    NewInit(); // depends on CScriptRuntime
    CScriptRuntime::Get()->PrintAll();
    //CPlayer Player = CPlayer(); 
    //fwScriptMgr::Get().InitAll();
    WHILE(true)
    {
        //LAGInterface::Writeln("Test");
        CScriptRuntime::Get()->Update();
        g_App->Update(); 
    }
    CScriptRuntime::Get()->Shutdown(); // I don't think this will get called inside of this object?
    g_App->Destroy(); // this line will never get reached in debug. 
}

void ScriptMain()
{
    srand(GetTickCount());
    main();
    
}
