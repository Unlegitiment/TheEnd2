#pragma once
#include <GTAVInfrastructure\SHV\enums.h>
#include <ScaleformUILib\Scaleform\common.h>
#include <ScaleformUILib\Menus\Pause Menus\PauseMenu\TabView.h>
#include <ScaleformUILib\Menus\Pause Menus\Elements\Columns\PM_Column.h>
class PauseMenuItem {
public:

	eFont Font =  FontChaletLondon;
	string Label;
	TabLeftItem* ParentLeftItem;
	BaseTab* ParentTab;
	PM_Column* ParentColumn;
	bool Selected;

	PauseMenuItem(string label) {
		Label = label;
	}
	PauseMenuItem(string label, eFont font) {
		Label = label;
		Font = font;
	}
private:

};