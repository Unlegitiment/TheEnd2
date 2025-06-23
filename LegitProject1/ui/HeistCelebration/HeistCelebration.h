#pragma once
#include "Renderer.h"
class CHeistCelebController {
	enum eStateMachine {
		eSM_INVALID,
		eSM_SETUP_DATA,
		eSM_RUN_DATA,
		eSM_CLEANUP,
		eSM_DONE,
		eSM_MAX
	};
public:
	void Init() {
		m_pRenderer = new CHeistCelebRenderer();
	}
	void Destroy() {
		delete m_pRenderer;
	}
	CIncrementalCashAnim* CreateCashAnimation(int id) {
		return m_pRenderer->GetWall()->CreateCashAnimation(id);
	}
	CStatTable* CreateStatTable(int id) {
		return m_pRenderer->GetWall()->CreateStatTable(id);
	}
	bool isFirstRun = true;
	void Update(HeistCelebData* data) {
		//if (data == nullptr) return; // we are null and don't care about progressionAdd commentMore actions
		if (data == nullptr) {
			//LAGInterface::Writeln("data is nullptr! quick return triggered");
			if (m_State != eSM_INVALID) {
				//yikes we got a problem! Scaleform data was just randomly changed to nullptr! this is a problem!
				m_pRenderer->GetWall()->Clean();
				LAGInterface::Writeln("data is nullptr! but our state is not invalid! quick return triggered");
				m_State = eSM_INVALID;
			}
			return;
		}
		if (data->m_Base.IsScaleformDone) {
			//LAGInterface::Writeln("data->m_Base.IsScaleformDone is true! quick return triggered");
			return;
		}
		if (m_State == eSM_INVALID) { // this is gonna be rough but I don't really care smh.
			LAGInterface::Writeln("SetupData Triggered");
			//if(!isFirstRun)
			m_pRenderer->GetWall()->CreateWall(data->m_Base.bgColour);
			m_pRenderer->GetWall()->AddToWall(data->MissionResult);
			m_pRenderer->GetWall()->AddToWall(data->m_pCompleteMessage);
			m_pRenderer->GetWall()->AddToWall(data->m_pDeduction);
			m_pRenderer->GetWall()->AddToWall(data->CashAnimation);
			m_pRenderer->GetWall()->AddToWall(data->StatTable);
			m_pRenderer->GetWall()->ShowStatTable(data->StatTable);
			m_pRenderer->GetWall()->AddToWall(data->m_pCashWon);
			m_pRenderer->GetWall()->AddToWall(data->m_pCashWall);
			m_pRenderer->GetWall()->AddToWall(data->JobPointData);
			m_pRenderer->GetWall()->AddToWall(data->RepWall);
			m_pRenderer->GetWall()->AddBackground(75);
			m_pRenderer->GetWall()->Show();
			m_ScaleformPredictedEndTimer = MISC::GET_GAME_TIMER() + data->Timer;
			//DATA = data; // might not be necessary
			m_State = eSM_RUN_DATA;
			return;
		}
		if (m_State == eSM_RUN_DATA) {
			if (MISC::GET_GAME_TIMER() >= m_ScaleformPredictedEndTimer) {
				LAGInterface::Writeln("MISC::GET_GAME_TIMER() >= m_ScaleformPredictedEndTimer! Setting state to Cleanup!");
				m_State = eSM_CLEANUP;
			}
			m_pRenderer->Update();
		}
		if (m_State == eSM_CLEANUP) {
			m_pRenderer->GetWall()->Clean();
			LAGInterface::Writeln("CLEANUP Triggered");
			data->m_Base.IsScaleformDone = true;
			isFirstRun = false;
			m_State = eSM_DONE;
		}
		if (m_State == eSM_DONE) {
			LAGInterface::Writeln("eSM_DONE Triggered");
			m_State = eSM_INVALID;
		}
	}
private:
	int m_ScaleformPredictedEndTimer = 0;
	CHeistCelebRenderer* m_pRenderer = nullptr;
	eStateMachine m_State = eSM_INVALID;
};
