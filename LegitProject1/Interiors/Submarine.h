#pragma once
#include <GTAVInfrastructure/SHV/natives.h>
#include <LegitProject1\EndTypes\Player.h>
#include <LegitProject1\datattypes\vector.h>
#include <array>
class CSubInterior {
public:
    enum eMissileSetAction : int {
        EMSA_OFFLINE,
        EMSA_READY,
        EMSA_ACTIVE,
        EMSA_RELOADING,
        EMSA_MAX
    };
    enum eSonarSetAction : int {
        ESSA_OFFLINE,
        ESSA_ONLINE,
        ESSA_MAX
    };
    struct SonarState {
        struct {
            StringHash Hashes[ESSA_MAX] = {
                "h4_prop_sub_screen_top_offline",
                "h4_prop_screen_top_sonar"
            };
        }Top;
        struct {
            StringHash Hashes[ESSA_MAX] = {
                "h4_prop_screen_btm_offline",
                "h4_prop_screen_bottom_sonar"
            };
        }Bottom;
    };
    struct MissileState {
        struct Top {
            StringHash Hashes[EMSA_MAX] = {
                "h4_prop_sub_screen_top_offline",
                "h4_prop_screen_top_missile_ready",
                "h4_prop_screen_top_missile_active",
                "h4_prop_screen_top_missile_active"
            };
        }Top;
        struct Bottom {
            StringHash Hashes[EMSA_MAX] = {
                "h4_prop_screen_btm_offline",
                "h4_prop_screen_btm_missile_ready",
                "h4_prop_screen_btm_missile_active",
                "h4_prop_screen_btm_missile_reload",
            };
        }Bottom;
    };
    enum eSubScreens {
        MISSILE_SEAT_LEFT,
        MISSILE_SEAT_RIGHT,
        SONAR,
        ESS_MAX
    };
    enum eSubScreensMISSILE {
        LEFT,
        RIGHT
    };
    static inline MissileState mStateHashes = MissileState();
    static inline SonarState mSonarHashes = SonarState();
    struct CSubScreenPositions {
        CVector3 m_BottomScreen;
        CVector3 m_TopScreen;
        CSubScreenPositions(CVector3 top, CVector3 btm) : m_TopScreen(top), m_BottomScreen(btm) {}
    };
    //By default this class its initialized with Kosatka interior locations.
    CSubInterior(std::array<CSubScreenPositions, ESS_MAX> positionOfModelHide = { {CSubInterior::CSubScreenPositions({ 1557.5453, 387.2964, -48.7999 }, { 1557.5453, 387.2964, -49.4132 }), {{1557.5453, 388.6938, -48.7999},{1557.5459, 388.6898, -49.4132}}, {{1557.5447, 385.9, -48.7999},{1557.5447, 385.9, -49.4132}} } }) : m_Position(positionOfModelHide) {
        m_InteriorId = INTERIOR::GET_INTERIOR_AT_COORDS(m_InteriorPosition.GetX(), m_InteriorPosition.GetY(), m_InteriorPosition.GetZ());
        INTERIOR::REFRESH_INTERIOR(m_InteriorId);
        GRAPHICS::REMOVE_DECALS_IN_RANGE(m_InteriorPosition.GetX(), m_InteriorPosition.GetY(), m_InteriorPosition.GetZ(), 100);
    }
    void SetMissileChairState(eSubScreensMISSILE chair, eMissileSetAction emsa) {
        this->m_CurrentMissileAction = emsa;
        HandleMissileStateChange(chair);
    }
    void SetSonarState(eSonarSetAction sonarSetAction) {
        m_CurrentSonarState = sonarSetAction;
        HandleSonarChange();
    }
private:
    void HandleMissileStateChange(eSubScreensMISSILE chair) {
        //Init default state.
        for (int i = 0; i < eMissileSetAction::EMSA_MAX; i++) {
            ENTITY::CREATE_MODEL_HIDE(m_Position[chair].m_BottomScreen.GetX(), m_Position[chair].m_BottomScreen.GetY(), m_Position[chair].m_BottomScreen.GetZ(), 0.1, mStateHashes.Bottom.Hashes[i].GetHash(), TRUE);
            ENTITY::CREATE_MODEL_HIDE(m_Position[chair].m_TopScreen.GetX(), m_Position[chair].m_TopScreen.GetY(), m_Position[chair].m_TopScreen.GetZ(), 0.1, mStateHashes.Top.Hashes[i].GetHash(), TRUE);
        }
        //Handle State Change
        ENTITY::REMOVE_MODEL_HIDE(m_Position[chair].m_TopScreen.GetX(), m_Position[chair].m_TopScreen.GetY(), m_Position[chair].m_TopScreen.GetZ(), 0.1, mStateHashes.Top.Hashes[m_CurrentMissileAction].GetHash(), false);
        ENTITY::REMOVE_MODEL_HIDE(m_Position[chair].m_BottomScreen.GetX(), m_Position[chair].m_BottomScreen.GetY(), m_Position[chair].m_BottomScreen.GetZ(), 0.1, mStateHashes.Bottom.Hashes[m_CurrentMissileAction].GetHash(), false);
    }
    void HandleSonarChange() {
        //Init default state.
        for (int i = 0; i < eSonarSetAction::ESSA_MAX; i++) {
            ENTITY::CREATE_MODEL_HIDE(m_Position[SONAR].m_BottomScreen.GetX(), m_Position[SONAR].m_BottomScreen.GetY(), m_Position[SONAR].m_BottomScreen.GetZ(), 0.1, mSonarHashes.Bottom.Hashes[i].GetHash(), TRUE);
            ENTITY::CREATE_MODEL_HIDE(m_Position[SONAR].m_TopScreen.GetX(), m_Position[SONAR].m_TopScreen.GetY(), m_Position[SONAR].m_TopScreen.GetZ(), 0.1, mSonarHashes.Top.Hashes[i].GetHash(), TRUE);
        }
        //Handle State Change
        ENTITY::REMOVE_MODEL_HIDE(m_Position[SONAR].m_TopScreen.GetX(), m_Position[SONAR].m_TopScreen.GetY(), m_Position[SONAR].m_TopScreen.GetZ(), 0.1, mSonarHashes.Top.Hashes[m_CurrentSonarState].GetHash(), false);
        ENTITY::REMOVE_MODEL_HIDE(m_Position[SONAR].m_BottomScreen.GetX(), m_Position[SONAR].m_BottomScreen.GetY(), m_Position[SONAR].m_BottomScreen.GetZ(), 0.1, mSonarHashes.Bottom.Hashes[m_CurrentSonarState].GetHash(), false);
    }
    CVector3 m_InteriorPosition = CVector3(1560.00000000, 400.00000000, -50.00000000);
    Interior m_InteriorId = 0;
    eMissileSetAction m_CurrentMissileAction;
    eSonarSetAction m_CurrentSonarState;
    std::array<CSubScreenPositions, ESS_MAX> m_Position;
};