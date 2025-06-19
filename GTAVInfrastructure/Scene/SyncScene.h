#pragma once
#include <LegitProject1\datattypes\vector.h>
#include <vector>
class CSynchronizedScene {
private:
	bool m_bIsSynchronizedSceneSetup = false;
	bool m_bCanSceneRun = false;
	bool m_bIsSynchronizedSceneDone = false;
	bool m_bIsSceneRunning = false;
	bool m_bIsSceneInitalized = false;
	const char* m_AnimDictionary = "";
	struct sAnimationData {
		float fAnimProgression = 0.0f;
		const char* pAnimName;
		Entity ObjectInScene;
		float blendIn = 1.0f, blendOut = 1.0f, moverBlendDelta = 0.0f;
		int flags = 0, ragdollBlockingFlags = 0, ikFlags = 0;
	};
	float m_fSceneProgression = 0.0f;
	int m_iSceneId = -1;
	CVector3 Position = { 0,0,0 };
	CVector3 Rotation = { 0,0,0 };
	std::vector<sAnimationData> m_Animations;
public:
	const char* GetAnimDictionary();
	void SetupScene(const char* animdictionary, CVector3 pos,  CVector3 rot);
	void QueueAnimation(int entityHandle, const char* name); // we need something better here. QueueAnimation isn't the best for what I want to do. Especially with a lack of description. 
	sAnimationData* GetAnimation(int index);
	void RunScene();
	float GetSceneProgression();
	void ReportAllInformationToLogger();
	void Destroy();
	int GetSceneId();
};

