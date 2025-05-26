#pragma once
#include <map>
class CPed;
class CPedHeadshotMgr {
public:
	static constexpr unsigned int TEXTURE_NAME_MAX = 32;
	static constexpr int INVALID_HEADSHOT = -1;
	struct sHeadshotDetails {
		int Id = INVALID_HEADSHOT;
		bool isShotReady = false, isShotValid = false;
		char txn[TEXTURE_NAME_MAX] = "", txd[TEXTURE_NAME_MAX] = ""; // idk lmao its all like mugshot_01 i think :/
	};
	const char* GetTexture(CPed* pPed);
	void RegisterPed(CPed* pPed);
	bool IsReady(CPed* pPed);
	bool IsValid(CPed* pPed);
	void DeregisterPed(CPed* pPed);
private:
	sHeadshotDetails* GetHeadFromPed(CPed* pPed); // love name <3
	std::map<CPed*, sHeadshotDetails> m_PedToHeadshot;
};