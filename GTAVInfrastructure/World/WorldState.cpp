#include "WorldState.h"
#include "GTAVInfrastructure\SHV\natives.h"
CWorldState::CWorldState()
{ 

}

void CWorldState::EnableMPMap(bool b)
{
    if (b) {
        DLC::ON_ENTER_MP();
        this->m_bIsMultiplayerMapActive = b;
    }
    else {
        DLC::ON_ENTER_SP();
        this->m_bIsMultiplayerMapActive = b;
    }
}

void CWorldState::SetWorldState(eWorldState state)
{
    
    STREAMING::SET_ISLAND_ENABLED("HeistIsland", state);
    m_State = state;
}

int CWorldState::GetState()
{
    return m_State;
}

const char* CWorldState::GetStateStr()
{
    if (m_State == LOS_SANTOS) {
        return "LOS_SANTOS";
    }
    return "CAYO_PERICO";
}
