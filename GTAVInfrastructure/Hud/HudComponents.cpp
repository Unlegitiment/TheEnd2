#include "HudComponents.h"
void CRankBar::SetColor(int HudColor)
{
	componentBar.BeginMethod("SET_COLOUR");
	componentBar.PushArguments(HudColor);
	componentBar.PushArgument(123);
	componentBar.EndMethod();
}

void CRankBar::SetRankScores(int XPStartLimit, int XPEndLimit, int xpPlayersPrevious, int xpPlayerCurrent, int CurrentPlayerLevel, int NextRank, int opacity)
{
	componentBar.BeginMethod("SET_RANK_SCORES");
	componentBar.PushArguments(XPStartLimit, XPEndLimit, xpPlayersPrevious, xpPlayerCurrent, CurrentPlayerLevel, opacity, NextRank);
	componentBar.EndMethod();
}
void CRankBar::StayOnScreen()
{
	componentBar.BeginMethod("STAY_ON_SCREEN");
	componentBar.EndMethod();
}

void CRankBar::OverrideDuration(int speed, int duration)
{
	componentBar.BeginMethod("OVERRIDE_ONSCREEN_DURATION");
	componentBar.PushArguments(duration);
	componentBar.EndMethod();
	componentBar.BeginMethod("OVERRIDE_ANIMATION_SPEED");
	componentBar.PushArguments(speed);
	componentBar.EndMethod();
}
