#pragma once
#include <vector>
#include <functional>
#define Register(x) void RegisterFunc##x(Action<void> pfunc) { this->m_##x.push_back(pfunc);}
class CApplication {
	template<typename T> using Action = std::function<T(T)>;
public:
	CApplication() { 
		this->m_Start.reserve(10);
	}
	void Init() {
		for (auto& func : m_Start) {
			func();
		}
	}
	void Update() {
		for (auto& func : m_Update) {
			func();
		}
	}
	void Destroy() {
		for (auto& func : m_Destroy) {
			func();
		}
	}
	void RegisterFuncStart(Action<void> pfunc) {
		this->m_Start.push_back(pfunc);
	};
	void RegisterFuncUpdate(Action<void> pfunc) {
		this->m_Update.push_back(pfunc);
	};
	void RegisterFuncDestroy(Action<void> pfunc) {
		this->m_Destroy.push_back(pfunc);
	};
private:
	std::vector<Action<void>> m_Start;
	std::vector<Action<void>> m_Update;
	std::vector<Action<void>> m_Destroy;
};