#include "SyncScene.h"
#include "GTAVInfrastructure/SHV/natives.h"
#include <LegitProject1\LInfras.h>
#include <LegitProject1\Network\obj\terminalColors.h>
const char* CSynchronizedScene::GetAnimDictionary() {
	return this->m_AnimDictionary;
}

void CSynchronizedScene::SetupScene(const char* animdictionary, CVector3 pos, CVector3 rot) {
	if (!STREAMING::DOES_ANIM_DICT_EXIST(animdictionary)) {
		LAGInterface::Writeln(RYellow "[SYNCHRONIZED_SCENE] %s Does not exist!", animdictionary);
		return;
	}
	while (!STREAMING::HAS_ANIM_DICT_LOADED(animdictionary)) {
		STREAMING::REQUEST_ANIM_DICT(animdictionary);
		WAIT(0);
	}
	if (!this->m_bIsSceneInitalized) {
		this->m_AnimDictionary = animdictionary;
		this->Position = pos;
		this->Rotation = rot;
		this->m_bIsSceneInitalized = true;
	}
}

void CSynchronizedScene::QueueAnimation(int entityHandle, const char* name) {
	this->m_Animations.push_back({ 0.0f, name, entityHandle });
}

CSynchronizedScene::sAnimationData* CSynchronizedScene::GetAnimation(int index) {
	if (index > this->m_Animations.size()) return nullptr;
	return &this->m_Animations[index];
}

void CSynchronizedScene::RunScene() {
	if (!this->m_bIsSynchronizedSceneSetup) {
		this->m_iSceneId = PED::CREATE_SYNCHRONIZED_SCENE(this->Position.GetX(), this->Position.GetY(), this->Position.GetZ(), this->Rotation.GetX(), this->Rotation.GetY(), this->Rotation.GetZ(), 2);
		this->m_bCanSceneRun = true;
		this->m_bIsSynchronizedSceneSetup = true;
	}
	if (this->m_bCanSceneRun && !this->m_bIsSceneRunning) {
		LAGInterface::Writeln("[SYNCHRONIZE SCENE] Scene ID: %d(AnimDict: %s): can run\n", this->m_iSceneId, this->m_AnimDictionary);
		for (int i = 0; i < this->m_Animations.size(); i++) {
			sAnimationData* curAnim = &this->m_Animations[i];

			if (i == this->m_Animations.size() - 1) {
				ENTITY::PLAY_SYNCHRONIZED_ENTITY_ANIM(curAnim->ObjectInScene, this->m_iSceneId, curAnim->pAnimName, this->GetAnimDictionary(), 1000.f, 0.0f, 0.0f, 1000.f);
			}
			else {
				TASK::TASK_SYNCHRONIZED_SCENE(curAnim->ObjectInScene, this->m_iSceneId, this->GetAnimDictionary(), curAnim->pAnimName, curAnim->blendIn, curAnim->blendOut, curAnim->flags, curAnim->ragdollBlockingFlags, curAnim->moverBlendDelta, curAnim->ikFlags);
			}
		}
		this->m_bIsSceneRunning = true;
	}
	if (this->m_bIsSceneRunning) {
		if (this->GetSceneProgression() >= 1.0f) {
			LAGInterface::Writeln("[SYNCHRONIZED_SCENE] %s has finished running.\n", this->m_AnimDictionary);
			this->m_bIsSynchronizedSceneDone = true;
			this->m_bIsSceneRunning = false;
		}
	}
}

float CSynchronizedScene::GetSceneProgression() {
	return PED::GET_SYNCHRONIZED_SCENE_PHASE(this->m_iSceneId);
}

void CSynchronizedScene::ReportAllInformationToLogger() {
}

void CSynchronizedScene::Destroy() {
	this->m_Animations.clear();
	this->m_AnimDictionary = "";
	this->m_bCanSceneRun = false;
	this->m_bIsSceneInitalized = false;
	this->m_bIsSceneRunning = false;
	this->m_bIsSynchronizedSceneDone = false;
	this->m_bIsSynchronizedSceneSetup = false;
	this->m_fSceneProgression = 0.0f;
	this->m_iSceneId = -1;
	this->Position = { 0,0,0 };
	this->Rotation = { 0,0,0 };
}

int CSynchronizedScene::GetSceneId() {
	return this->m_iSceneId;
}