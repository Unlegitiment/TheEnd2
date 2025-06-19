#pragma once
#include "LegitProject1\datattypes\vector.h"
#include <GTAVInfrastructure\SHV\natives.h>
class CCameraShake {
	friend class CCamera;
public:
	CCameraShake(const char* Name, float fAmp) :  m_Name(Name), m_fAmplitude(fAmp){}

	const char* GetName() { return m_Name; }
	float GetAmp() { return m_fAmplitude; }
private:
	const char* m_Name;
	float m_fAmplitude;
};
class CCamera {
protected: 
	friend class CCameraManager;
	CCamera(const char* CameraName, CVector3 postiion, CVector3 rotation, float FOV) : m_Position(postiion), m_Rotation(rotation), m_CameraName(CameraName), m_fFov(FOV){
		this->m_iCameraIdx = CAM::CREATE_CAM(CameraName, 0);
		CAM::SET_CAM_ROT(m_iCameraIdx, rotation.GetX(), rotation.GetY(), rotation.GetZ(), 2);
		CAM::SET_CAM_COORD(m_iCameraIdx, postiion.GetX(), postiion.GetY(), postiion.GetZ());
		CAM::SET_CAM_FOV(m_iCameraIdx, FOV);
	}
public:
	const char* GetCameraName() { return this->m_CameraName; }
	CVector3 GetCameraPos() { return m_Position; }
	CVector3 GetCameraRot() { return m_Rotation; }
	float GetCameraFOV() { return m_fFov; }
	void SetCameraPos(CVector3 v) {
		m_Position = v;
		CAM::SET_CAM_COORD(m_iCameraIdx, v.GetX(), v.GetY(), v.GetZ());
	}
	void SetCameraRot(CVector3 v) {
		m_Rotation = v;
		CAM::SET_CAM_ROT(m_iCameraIdx, v.GetX(), v.GetY(), v.GetZ(), 2);
	}
	void SetCameraFOV(float FOV) {
		this->m_fFov = FOV;
		CAM::SET_CAM_FOV(m_iCameraIdx, FOV);
	}
	void SetActive(bool b) {
		this->m_bIsActive = b;
	}
	bool IsActive() { return m_bIsActive; }
	int GetCameraHandle() { return this->m_iCameraIdx; }
	//explicit operator int() {
	//	return m_iCameraIdx;
	//}
	operator Cam() {
		return m_iCameraIdx;
	}
	void AppleCamShake(const char* name, float fAmp) {
		if (m_pCameraShake) {
			delete m_pCameraShake; m_pCameraShake = nullptr;
		}
		m_pCameraShake = new CCameraShake(name, fAmp);
	}
	~CCamera() {
		if (m_pCameraShake) { delete m_pCameraShake; m_pCameraShake = nullptr; }
		
	}
	CCamera() = delete; // GET OUT OF HERE!
private:
	CCameraShake* m_pCameraShake = nullptr;
	int m_iCameraIdx;
	const char* m_CameraName;
	CVector3 m_Position;
	CVector3 m_Rotation;
	float m_fFov;
	bool m_bIsActive = false;
};

//#include <unordered_map>
#include <array>
#include <LegitProject1\LInfras.h>
class CCameraManager {
enum eCameraType : int{
	DEFAULT_ANIMATED_CAMERA,
	DEFAULT_SCRIPTED_CAMERA,
	DEFAULT_SCRIPTED_FLY_CAMERA,
	DEFAULT_SPLINE_CAMERA,
	MAX_CAMERA
};
static constexpr int MAX_CAM_TYPES = 4;
const char* CAM_TYPES[MAX_CAM_TYPES] = {
	"DEFAULT_ANIMATED_CAMERA",
	"DEFAULT_SCRIPTED_CAMERA",
	"DEFAULT_SCRIPTED_FLY_CAMERA",
	"DEFAULT_SPLINE_CAMERA"
};
CCamera* m_pCurrentCamera = nullptr;
public:
	CCamera* CreateCamera(CVector3 postiion, CVector3 rotation, float FOV) {
		auto pair = GetFirstAvailableCamera();
		CCamera* camera = new CCamera(pair.second, postiion, rotation, FOV);
		m_aCameras[pair.first] = camera;
		return camera;
	}
	void SetCameraAsActive(eCameraType camera) {
		if (m_aCameras[camera] == nullptr) {
			LAGInterface::Writeln("[CAMERA_INTERFACE] There is no camera that exists at %d", camera);
			return; 
		}
		m_pCurrentCamera = m_aCameras[camera];
		m_pCurrentCamera->SetActive(true);
	}
	void Update() {
		if (m_pCurrentCamera) {
			if (!m_pCurrentCamera->IsActive()) {
				return;
			}
			CAM::SET_CAM_ACTIVE(*m_pCurrentCamera, m_pCurrentCamera->m_bIsActive);
		}
	}
	void DestroyCamera(CCamera* camera) {
		auto it = std::find(m_aCameras.begin(), m_aCameras.end(), camera);
		if (it != m_aCameras.end()) { // found
			if (*it) {
				delete* it;
				*it = nullptr;
			}
		}
		return;
	}
	eCameraType GetCamTypeFromName(const char* CamType) {
		for (int i = 0; i < MAX_CAM_TYPES; i++) {
			if (strncmp(CAM_TYPES[i], CamType, 64) == 0) {
				return (eCameraType)i;
			}
		}
		return MAX_CAMERA;
	}
private:
	std::pair<eCameraType, const char*> GetFirstAvailableCamera() {
		for (int i = 0; i < m_aCameras.size(); i++) {
			if (m_aCameras[i])continue;
			const char* camname = GetCameraName((eCameraType)i);
			return { (eCameraType)i, camname };
		}
		return { MAX_CAMERA, "MAX_CAM" };
	}
	const char* GetAvailableCameraName() {
		for (int i = 0; i < m_aCameras.size(); i++) {
			if (m_aCameras[i] == nullptr) {
				return GetCameraName((eCameraType)i);
			}
		}
		return ""; // throw std::exception :/
	}
	const char* GetCameraName(eCameraType camType) {
		return CAM_TYPES[camType];
	}

	std::array<CCamera*, MAX_CAMERA> m_aCameras = {};
};