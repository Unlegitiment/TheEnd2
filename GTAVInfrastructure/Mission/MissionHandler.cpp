//#include "MissionHandler.h"
//
//void CMarkerHandler::AddEntry(sMarker* pMrk, fwMission* pMiss)
//{
//    this->m_MarkerToMission.insert({ pMrk, pMiss });
//}
//
//fwMission* CMarkerHandler::FindAtLocation(CVector3 pos)
//{
//    for (auto& [key, val] : m_MarkerToMission) {
//        if (key->RangeCheck(pos)) { 
//            return val;
//        }
//    }
//    return nullptr; // ts tragic!
//}
//
//int CMissionList::AddEntry(fwMission* miss)
//{
//    this->m_AllMissions.push_back(miss);
//    return m_AllMissions.size() - 1;
//}
//
//bool CMissionList::HasEntry(fwMission* pMiss)
//{
//    auto it = std::find(m_AllMissions.begin(), m_AllMissions.end(), pMiss);
//    if (it == m_AllMissions.end()) {
//        return false;
//    }
//    return true;
//}
//
//int CMissionList::GetEntryIndex(fwMission* pMiss)
//{
//    for (int i = 0; i < m_AllMissions.size(); i++) {
//        if (m_AllMissions[i] == pMiss) {
//            return i;
//        }
//    }
//    return -1;
//}
//
//void CMissionHandler::Init()
//{
//    this->m_pMarkerHandler = new CMarkerHandler();
//    this->m_pMissionList = new CMissionList();
//}
//
//void CMissionHandler::Update()
//{
//}
//
//int CMissionHandler::CreateMission(sMarker* pMarker, fwMission* pMission)
//{
//    if (pMarker == nullptr || pMission == nullptr) {
//        return -1; // invalid. Until I can figure out some way to actually do this lmao.
//    }
//    this->m_pMarkerHandler->AddEntry(pMarker, pMission);
//    this->m_pMissionList->AddEntry(pMission);
//}
//
//int CMissionHandler::GetMissionAtPosition(const CVector3& pos)
//{
//    if (auto rawMission = m_pMarkerHandler->FindAtLocation(pos)) { // if found at location
//        int index = m_pMissionList->GetEntryIndex(rawMission);
//        return index; // guess ill die lmao?
//    }
//}
//
//void CMissionHandler::LaunchMission(fwMission* pMission)
//{
//    if (m_pMissionList->HasEntry(pMission)) {
//        this->m_pMission = pMission;
//        pMission->Init();
//    } else { // well now I need some way to actually add identifiers to missions? Like mission info. idk how to do that given the current setup. 
//        m_pMissionList->AddEntry(pMission);
//        m_pMission = pMission;
//        pMission->Init(); // ts pmo
//        return;
//    }
//}
//
//bool CMissionHandler::IsOnMission()
//{
//    return false;
//}
//
//void CMissionHandler::Shutdown()
//{
//}
//
//int CRand::GenerateNumber(iRange range)
//{
//    std::uniform_int_distribution<> dist(range.GetLowerBound(), range.GetUpperBound());
//    return dist(CRand::generator);
//}
//
//float CRand::GenerateNumber(fRange range)
//{
//    std::uniform_real_distribution<float> dist(range.GetLowerBound(), range.GetUpperBound());
//    
//    return dist(generator);
//}
