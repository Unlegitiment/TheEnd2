#pragma once
#include "ScaleformUILib/Scaleform/BigFeed/BigFeedHandler.h"
#include "ScaleformUILib/Scaleform/BigMessage/BigMessage.h"
#include "ScaleformUILib/Scaleform/Countdown/CountdownHandler.h"
//#include "ScaleformUILib/Scaleform/Instructional_Buttons/InstructionalButtons.h"
#include "ScaleformUILib/Scaleform/MediumMessage/MediumMessage.h"
//#include "ScaleformUILib/Scaleform/Minimap/MinimapOverlays.h"
//#include "ScaleformUILib/Scaleform/MissionSelector/MissionSelector.h"
#include "ScaleformUILib/Scaleform/MultiplayerChat/MultiplayerChatHandler.h"
//#include "ScaleformUILib/Scaleform/PopupWarning/PopupWarning.h"
#include "ScaleformUILib/Scaleform/Rankbar/RankBarHandler.h"




class CTest {
public:
	BigFeedHandler FeedHandler{};
	BigMessageHandler BigMsgHandler{};
	CountdownHandler CountDownHandler{};
	MediumMesageHandler MedMsgHandler{};
	//MissionSelectorHandler MissionSelectorHandler = MissionSelectorHandler();
	MultiplayerChatHandler MPChatHandler{};
	RankBarHandler RankBarHandler{};
	void Update() {
		LAGInterface::Writeln(__FUNCTION__" Called");
		FeedHandler.Update();
		LAGInterface::Writeln(__FUNCTION__" Called 1");
		BigMsgHandler.Update();
		LAGInterface::Writeln(__FUNCTION__" Called 2");
		MedMsgHandler.Update();
		LAGInterface::Writeln(__FUNCTION__" Called 3");
		MPChatHandler.Update();
		LAGInterface::Writeln(__FUNCTION__" Called 4");
	}
private:

};