#pragma once
#include "Rand.h"
#include <unordered_map>
template<typename Hndl, typename Val> class MyMgr {
public:
	MyMgr(CRand<Hndl>* pHandleGen) : m_HandleGen(pHandleGen) {}
	template<typename... Args>
	Hndl Allocate(Args&&... args) {
		Hndl handle = Gen();
		this->m_Map[handle] = new Val(std::forward<Args>(args)...);
		return handle;
	}
	Val* Get(Hndl h) {
		auto pImpl = this->m_Map.find(h);
		return (pImpl != m_Map.end()) ? pImpl->second : nullptr;
	}
	void DestroyHandle(Hndl h) {
		delete this->m_Map[h];
		this->m_Map.erase(h);
	}
protected:
	Hndl Gen() { return m_HandleGen->operator()(); }
	CRand<Hndl>* m_HandleGen = nullptr;
	std::unordered_map<Hndl, Val*> m_Map;
private:

};