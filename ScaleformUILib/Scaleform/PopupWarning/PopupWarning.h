#pragma once
#include <ScaleformUILib\Scaleform\Instructional_Buttons\InstructionalButtons.h>
#error "Popup Warning not fully implemented! @Dev Req: InstructBtn, Glbl IBtnInstance"
enum WarningPopupType
{
	Classic,
	Serious
};
#include <ScaleformUILib\Scaleform\ScaleformExtensions.h>

#define USE_LIST
#include <ScaleformUILib\Scaleform\common.h>

typedef void(*PopupWarningEvent)(InstructionalButton* button);

class PopupWarning
{
private:
	ScaleformWideScreen* _sc = nullptr;
	bool _disableControls;
	List<InstructionalButton*>* _buttonList;
public:

	bool IsShowing()
	{
		return _sc != nullptr;
	}

	bool IsShowingWithButtons()
	{
		return _disableControls;
	}

	PopupWarningEvent OnButtonPressed;

private:
	void Load()
	{
		if (_sc != nullptr) return;
		_sc = new ScaleformWideScreen("POPUP_WARNING");
		int timeout = 1000;
		int start = MISC::GET_GAME_TIMER();
		while (!_sc->IsLoaded() && MISC::GET_GAME_TIMER() - start < timeout) WAIT(0);
	}

public:
	void Dispose()
	{
		if (_sc == nullptr) return;
		_sc->CallFunction("HIDE_POPUP_WARNING", 1000);
		_sc->Dispose();
		_sc = nullptr;
		delete _sc;
		_disableControls = false;
	}

	void ShowWarning(string title, string subtitle, string prompt = "", string errorMsg = "", WarningPopupType type = WarningPopupType.Classic, bool showBackground = true)
	{
		Load();
		_sc->CallFunction("SHOW_POPUP_WARNING", 1000, title, subtitle, prompt, showBackground, (int)type, errorMsg);
	}

	void UpdateWarning(string title, string subtitle, string prompt = "", string errorMsg = "", WarningPopupType type = WarningPopupType.Classic, bool showBackground = true)
	{
		if (!_sc->IsLoaded()) return;
		_sc->CallFunction("SHOW_POPUP_WARNING", 1000, title, subtitle, prompt, showBackground, (int)type, errorMsg);
	}

	void ShowWarningWithButtons(string title, string subtitle, string prompt, List<InstructionalButton*>* buttons, string errorMsg = "", WarningPopupType type = WarningPopupType.Classic, bool showBackground = true)
	{
		Load();
		_disableControls = true;
		_buttonList = buttons;
		if (buttons == nullptr || buttons->size() == 0) return;
		//Main.InstructionalButtons.UseMouseButtons = true;
		//Main.InstructionalButtons.SetInstructionalButtons(_buttonList);
		//Main.InstructionalButtons.ControlButtons.ForEach(x = > x.OnControlSelected += OnControlSelected);
		_sc->CallFunction("SHOW_POPUP_WARNING", 1000, title, subtitle, prompt, showBackground, (int)type, errorMsg);
	}

private:
	void OnControlSelected(InstructionalButton* control)
	{
		Dispose();
		OnButtonPressed(control);
		/*Main.InstructionalButtons.ClearButtonList();
		Main.InstructionalButtons.UseMouseButtons = false;*/
		OnButtonPressed = nullptr;
		//if (MenuHandler.CurrentMenu != null)
		//{
		//	if (MenuHandler.CurrentMenu.InstructionalButtons.Count > 0)
		//		Main.InstructionalButtons.SetInstructionalButtons(MenuHandler.CurrentMenu.InstructionalButtons);
		//}
		//else if (MenuHandler.currentBase != null)
		//{
		//	if (MenuHandler.currentBase.InstructionalButtons.Count > 0)
		//		Main.InstructionalButtons.SetInstructionalButtons(MenuHandler.currentBase.InstructionalButtons);
		//}
	}

	void Update()
	{
		_sc->Render2D();
	}
};