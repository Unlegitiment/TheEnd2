#include "Model.h"
#include "GTAVInfrastructure/SHV/natives.h"
#include <LegitProject1\LInfras.h>
void CStreamingModel::Request()
{
    int m_Tries = 0;
    static constexpr int MAX_TRIES = 10;
    while (!IsLoaded() && m_Tries <= MAX_TRIES) {
        status = LOADING;
        STREAMING::REQUEST_MODEL(hash);
        m_Tries++;
        WAIT(0);
    }
    if (m_Tries == MAX_TRIES) {
        LAGInterface::Writeln("Failed to Load Ped of Hash: %lu", hash);
        status = FAILED;
        return;
    }
    status = LOADED;
}

bool CStreamingModel::IsLoaded()
{
    return STREAMING::HAS_MODEL_LOADED(hash);
}

void CStreamingModel::Derequest()
{
    STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
}
