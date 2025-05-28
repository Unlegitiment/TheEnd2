#pragma once
enum eWorldState  : int{
	LOS_SANTOS,
	CAYO
};
class CWorldState {
public:
	CWorldState();
	void EnableMPMap(bool b);
	void SetWorldState(eWorldState state);
	int GetState();
	const char* GetStateStr();
	bool IsMPMapActive() { return m_bIsMultiplayerMapActive; }
private:
	int m_State = LOS_SANTOS;
	bool m_bIsMultiplayerMapActive = false;
};