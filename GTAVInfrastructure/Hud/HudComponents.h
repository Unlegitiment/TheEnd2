#pragma once
#include "Obj/Scaleform.h"
// This class does not hold state. Honestly would be nice to just make it static. 
class CRankBar {
private:
	static constexpr int RANK_BAR_ID = 19;
	inline static CHudComponent componentBar = CHudComponent(RANK_BAR_ID);
public:
	static CHudComponent& GetRawBar() { return componentBar; }
	static void SetColor(int HudColor = 116);
	static void SetRankScores(int XPStartLimit, int XPEndLimit, int xpPlayersPrevious, int xpPlayerCurrent, int CurrentPlayerLevel, int NextRank, int opacity = 100);
	static void StayOnScreen();
	static void OverrideDuration(int speed, int duration);
};
