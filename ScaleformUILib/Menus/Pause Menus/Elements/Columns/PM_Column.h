#pragma once
#include <ScaleformUILib\Menus\Pause Menus\PauseMenu\TabView.h>
#define USE_LIST
#include "..\..\..\..\Scaleform\common.h"
#include <ScaleformUILib\Menus\Pause Menus\Items\PauseMenuItem.h>
#include <ScaleformUILib\Menus\Pause Menus\Items\TabLeftItem.h>
enum PM_COLUMNS : int {
	LEFT,
	MIDDLE,
	RIGHT,
	EXTRA3,
	EXTRA4,
	LEFT_MIDDLE,
	MIDDLE_RIGHT,
};
class SettingsItem; // @TODO: Define


using IndexChanged = void(*)(int index);
using ItemSelect = void(*)(TabView* menu, TabLeftItem* item,  int leftItemIndex);
using RightItemSelect = void(*)(TabView* menu, SettingsItem* item, int leftItemIndex, int rightItemIndex);


class PM_Column {
private:
	PM_COLUMNS position;
	int index =0;
	int type = -1;
	bool columnVisible;
	bool visible = Parent != nullptr && Parent.Visible && Parent.Parent != nullptr && Parent.Parent.Visible;
	List<PauseMenuItem*>* Items = new List<PauseMenuItem*>();
public:
};