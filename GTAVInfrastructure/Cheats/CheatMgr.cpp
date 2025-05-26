#include "CheatMgr.h"
#include <GTAVInfrastructure\SHV\natives.h>
void CCheatMgr::Update()
{
	for (auto& cheat : Commands.GetList()) {
		if (MISC::HAS_PC_CHEAT_WITH_HASH_BEEN_ACTIVATED(cheat.Command)) {
			cheat.Fire();
		}
	}
}
