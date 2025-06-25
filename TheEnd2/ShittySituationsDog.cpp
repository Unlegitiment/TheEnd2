#include "ShittySituationsDog.h"

void SHVDrawText(const char* string, float x, float y, float scale) {
	HUD::SET_TEXT_FONT(0);
	HUD::SET_TEXT_PROPORTIONAL(1);
	HUD::SET_TEXT_SCALE(scale, scale);
	HUD::SET_TEXT_COLOUR(240, 240, 240, 255);
	HUD::SET_TEXT_OUTLINE();
	HUD::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
	HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(string);
	HUD::END_TEXT_COMMAND_DISPLAY_TEXT(x, y, 0);
}
