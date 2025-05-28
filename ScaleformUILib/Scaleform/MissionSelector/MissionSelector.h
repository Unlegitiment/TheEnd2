#pragma once
#include <ScaleformUILib\Scaleform\ScaleformExtensions.h>
#include <ScaleformUILib\Scaleform\common.h>
#include <vector>
enum JobSelectionCardIcon
{
	NONE = -1,
	CUSTOM_MISSION,
	DEATHMATCH,
	RACE,
	SURVIVAL,
	TEAM_DEATHMATCH,
	UNUSED1,
	GANG_ATTACK,
	UNUSED2,
	BASE_JUMPING,
	VEHICLE_DEATHMATCH,
	RACE_LAND,
	RACE_FOOT,
	RACE_BICYCLE,
	RACE_WATER,
	RACE_AIR,
	LAST_TEAM_STANDING,
	CAPTURE_THE_FLAG,
	HEIST_PREPARATION,
	HEIST,
	RACE_STUNT,
};
#pragma message ("MissionSelectorHandler does not have full implementation yet. Wait")
class JobSelectionTitle;
class JobSelectionCard;
class JobSelectionButton;
class MissionSelectorHandler {
private:
	ScaleformWideScreen _sc;
	bool enabled;
	bool alreadyVoted;
public:
	int Votes[9] = {0};
	int VotedFor = -1;
	int MaxVotes = 0;
	int SelectedCard = 0;
	HudColour VotesColor = HUD_COLOUR_BLUE;
	JobSelectionTitle* JobTitle; // Not defined yet
	std::vector<JobSelectionCard*> Cards;
	std::vector<JobSelectionButton*> Buttons; 

};