#pragma once
#include "GTAVInfrastructure\SHV\natives.h" // Remove once we move sources to header

class fwStream {
public:
	virtual void Request() = 0;
	virtual void Remove() = 0;
	virtual ~fwStream() = default;
private:

};
class IPL : public fwStream {
public:
	static constexpr int MAX_IPL_TRIES = 10;
	IPL(const char* ipl) :m_Buff(ipl){

	}
	void Request() override
	{
		STREAMING::REQUEST_IPL(m_Buff);
		int attempts = 0;
		while (!STREAMING::IS_IPL_ACTIVE(m_Buff) && attempts < MAX_IPL_TRIES) { STREAMING::REQUEST_IPL(m_Buff); WAIT(0); attempts++; }
	}

	void Remove() override
	{
		STREAMING::REMOVE_IPL(m_Buff); // this will always work
	}
	const char* GetIPLName() {
		return m_Buff;
	}
private: 
	const char* m_Buff;
};
class CModel : public fwStream {
public:
	static constexpr int MAX_MODEL_RETRY_ATTEMPTS = 10; // Fetch from some sort of a Global Config Item.
	CModel(Hash hash) :m_Hash(hash){}
	// Inherited via fwStream
	void Request() override
	{
		STREAMING::REQUEST_MODEL(m_Hash);
		int attempts = 0;
		while (!STREAMING::HAS_MODEL_LOADED(m_Hash) && attempts < MAX_MODEL_RETRY_ATTEMPTS) STREAMING::REQUEST_MODEL(m_Hash); WAIT(0); attempts++;
	}
	void Remove() override
	{
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(m_Hash);
	}
private:
	Hash m_Hash;
};
#include <unordered_map>
class CIPLManager {
public:
	IPL* CreateIPL(const char* ipl) {
		if (auto* found = Find(ipl)) return found;
		IPL* _ipl = new IPL(ipl);
		m_Ipls.insert({ ipl, _ipl });
		return _ipl;
	}
	IPL* Find(const char* ipl) {
		auto it = m_Ipls.find(ipl);
		if (it != m_Ipls.end()) { return it->second; } // found;
		return nullptr;
	}
private:
	std::unordered_map<const char*, IPL*> m_Ipls;
};
class CModelCache {
public:
	CModel* CreateModel(Hash modelName) {
		if (auto* model = Find(modelName)) return model;
		CModel* model = new CModel(modelName);
		m_Models.insert({ modelName, model });
		return model;
	}
	void RemoveModel(Hash modelName) {
		auto model = m_Models.find(modelName);
		if (model == m_Models.end()) return; // we did not find it. 
		delete model->second;
		model->second = nullptr;
		m_Models.erase(model);
	}
	CModel* Find(Hash modelName) {
		auto it = this->m_Models.find(modelName);
		if (it != m_Models.end()) return it->second;
		return nullptr;
	}
private:
	std::unordered_map<unsigned int, CModel*> m_Models;
};
class Streamer {
public:
	static void Init() {
		sm_pIPLManager = new CIPLManager();
		sm_pModelCache = new CModelCache();
	}
	static CModelCache* GetModels() { return sm_pModelCache; }
	static CIPLManager* GetIPLManager() { return sm_pIPLManager; }
	static void Shutdown() {
		delete sm_pModelCache;
		sm_pModelCache = nullptr;
		delete sm_pIPLManager;
		sm_pIPLManager = nullptr;
	}
private:
	static inline CModelCache* sm_pModelCache = nullptr;
	static inline CIPLManager* sm_pIPLManager = nullptr;
};
class CMapData {
public:
	CMapData();
	void AddItem(const char* item) {
		this->m_List.push_back(Streamer::GetIPLManager()->CreateIPL(item));
	}
	void RequestAll() {
		for (auto* ipl : m_List) {
			ipl->Request();
		}
	}
	void RemoveAll() {
		for (auto* ipl : m_List) {
			ipl->Remove();
		}
	}
private:
	std::vector<IPL*> m_List;
};
class CMapDataBuilder {
public:
	CMapDataBuilder& Add(const char* item) {
		m_MapData.AddItem(item);
		return *this;
	}
	CMapData Finalize(bool request) {
		if (request) m_MapData.RequestAll();
		return std::move(m_MapData);
	}
private:

	CMapData m_MapData;
};