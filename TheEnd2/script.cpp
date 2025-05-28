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
struct CDumpUtil {
    static void DumpWeatherHash() {
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
    CreateInstances();
    //CPlayer Player = CPlayer(); 
    fwScriptMgr::Get().InitAll();
    while (true)
    {
        g_App->Update();
        
        WAIT(0);
    }
    g_App->Destroy(); // this line will never get reached in debug. 
}

void ScriptMain()
{
    srand(GetTickCount());
    main();
    
}
