#pragma once
#include <ScaleformUILib\Scaleform\Instructional_Buttons\InstructionalButtons.h>
#include <GTAVInfrastructure\SHV\natives.h>
#define USE_LIST
#include "ScaleformUILib\Scaleform\common.h"
class PauseMenuBase{
public:
	//Borrow ownership
	PauseMenuBase(List<InstructionalButton*>* _instructBtn) :InstructionalButtons(_instructBtn){

	}
	virtual bool IsVisible() {
		return this->visible;
	}
	virtual void SetVisible(bool value) {
		this->visible = value;
		//MenuHandler::ableToDraw = value; I'm not quite sure what this actually comes from yet.
	}
	List<InstructionalButton*>* GetInstructionalButtons() { return InstructionalButtons; } //return underlying ptr. allow for deletion if necessary
	void SetInstructionalButtons(List<InstructionalButton*>* pButtons) {
		this->InstructionalButtons = pButtons;
	}
	virtual void ProcessControls(){}
	virtual void ProcessMouse(){}
	virtual void Draw() {
		PAD::DISABLE_CONTROL_ACTION(0, 199, true);
		PAD::DISABLE_CONTROL_ACTION(0, 200, true);
		PAD::DISABLE_CONTROL_ACTION(1, 199, true);
		PAD::DISABLE_CONTROL_ACTION(1, 200, true);
		PAD::DISABLE_CONTROL_ACTION(2, 199, true);
		PAD::DISABLE_CONTROL_ACTION(2, 200, true);
	}
	virtual ~PauseMenuBase() = default;
public:
	bool CanPlayerCloseMenu = true;
private:

	bool visible = false;
	List<InstructionalButton*>* InstructionalButtons;
};