//#pragma once
//#include "Missions.h"
//#include "LegitProject1\datattypes\vector.h"
//#include <map>
//
//struct sMarker {
//	CVector3 pos;
//	bool RangeCheck(const CVector3& vec);
//};
//struct sMarkerInfo {
//	char* name;
//	char* desc;
//	char* wtv;
//};
//template<typename T>
//struct Component {
//	Component();
//};
//#include <random>
//template<typename T>
//class _CRange {
//public:
//	_CRange(T min, T max) {
//		this->m_tpMin = min;
//		this->m_tpMax = max;
//	}
//	T GetUpperBound() { return this->m_tpMax; }
//	T GetLowerBound() { return this->m_tpMin; }
//private:
//	T m_tpMin, m_tpMax;
//};
//using iRange = _CRange<int>;
//using fRange = _CRange<float>;
//class CRand {
//public:
//	static int		GenerateNumber(iRange range);
//	static float	GenerateNumber(fRange range);
//private:
//	static std::random_device rd;
//	static std::mt19937_64 generator;
//};
//auto CRand::rd = std::random_device();
//auto CRand::generator(rd);
//struct UniqueId : public Component<UniqueId> {
//public:
//	typedef unsigned long long UUID;
//	static UniqueId Generate();
//	UUID& Get();
//protected:
//	UniqueId(UUID id) { this->iden = id; }
//	UUID iden;
//};
//struct MissionHandle {
//	MissionHandle();
//	sMarker* pMarker;
//	sMarkerInfo* pMissIfo; // passing everything as a ptr is risky because it opens to mem leaks. Or we could just yk handle memory some other way haha? 
//	UniqueId* pUUID;
//};
//#include <vector>
//class CMissionHandler {
//public:
//	CMissionHandler() { Init(); }
//	void Init();
//	void Update();
//	int CreateMission(sMarker* pMarker, fwMission* pMission);
//	int GetMissionAtPosition(const CVector3& pos);
//	void LaunchMission(fwMission* pMission);
//	bool IsOnMission();
//	void Shutdown();
//	~CMissionHandler() { Shutdown(); }
//private:
//	template<typename T, typename O> using Map = std::map<T, O>; 
//	Map<MissionHandle*, fwMission*> pMission; // well every mission is id'ed
//	fwMission* m_pMission = nullptr;
//};