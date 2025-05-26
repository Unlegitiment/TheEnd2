#pragma once
enum eWorldState  : int{
	LOS_SANTOS,
	CAYO
};
class CWorldState {
public:
	CWorldState();
	void SetWorldState(eWorldState state);
	int GetState();
	const char* GetStateStr();
private:
	int m_State = LOS_SANTOS;
};