#pragma once
#include <vector>
typedef unsigned long Hash;
class CModelLoading {
public:
	void LoadModel(Hash hash);
	void UnloadModel(Hash hash);
	bool IsModelLoaded(Hash model) { return std::find(m_vHashLoaded.begin(), m_vHashLoaded.end(), model) != m_vHashLoaded.end(); }
	auto Find(Hash h) { return std::find(m_vHashLoaded.begin(), m_vHashLoaded.end(), h);  }
private:
	std::vector<Hash> m_vHashLoaded;
};