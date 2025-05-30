#include "script.h"
#include "GTAVInfrastructure/SHV/natives.h"
#include "TestClass.h"
#include "LegitProject1/LInfras.h"
#include "keyboard.h"
//All work except for MPChatHandler and CountDown. Message is a no-show. Check SCL SRC to ensure it doesn't run a Network Cmd. (Why would it lmao but check anyways) 
// Countdown could be an issue with hijacking the main exec loop and then not putting it back. Could be solved with a little *engineering*
void main() {
	LAGInterface::Init();
	CTest test{};
	LAGInterface::Writeln("Test constructed");
	while (true) {
		if (IsKeyJustUp(VK_ADD)) {
			test.RankBarHandler.SetScores(0, 1000, 0, 1001, 100);
			test.BigMsgHandler.ShowRankupMessage("RANK UP", "", 100);
			test.FeedHandler.SetBodyText("Value");
			test.FeedHandler.SetTitle("Test");
			test.FeedHandler.SetEnabled(true);
			LAGInterface::Writeln("Right before chatmsg");
			test.MPChatHandler.Load(); 
			test.MPChatHandler.AddMessage("Staeni92", "1-0", Global, false, HudColour::HUD_COLOUR_WHITE);
			test.MPChatHandler.StartTyping();
			test.MPChatHandler.Show();
			LAGInterface::Writeln("Right after chatmsg");

			//test.CountDownHandler.Start();
		}
		test.Update();
		WAIT(0);
	}
}

void ScriptMain() {
	srand(GetTickCount());
	main();
}