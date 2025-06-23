#pragma once
#include "HeistCelebration\CelebTypes.h"
#include "HeistCelebration\HeistCelebration.h"
#include "NewHud\Hud_Testing_Env.h"
class CHud {
public:
	CHud() {
		celeb->Init();
		hudTest->bRequiresHudRestart = true;
		hudTest->Init();
		
	}
	CIncrementalCashAnim* CreateCash(int id) {
		return celeb->CreateCashAnimation(id);
	}
	CStatTable* CreateStat(int id) {
		return celeb->CreateStatTable(id);
	}
	void SetHeistData(HeistCelebData* data) {
		this->Data = data;
	}
	void Update() {
		celeb->Update(Data);
		//logf("hudTest: 0x%p, values : { %d, 0x%p } ", hudTest, hudTest->bRequiresHudRestart, hudTest->scaleform);
		hudTest->Update();
	}
	~CHud() {
		celeb->Destroy();
		hudTest->Shutdown();
		delete celeb;
		delete hudTest;
	}
private:
	CHeistCelebController* celeb = new CHeistCelebController();
	CNewHudTest* hudTest = new CNewHudTest();
	HeistCelebData* Data = nullptr;
};