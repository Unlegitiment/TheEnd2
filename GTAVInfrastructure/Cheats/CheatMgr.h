#pragma once
#include <functional>
#include <vector>
struct fwCheat {
	using Func = std::function<void()>;
	fwCheat(Func f) : Fired(f){

	}
	virtual void Fire() {
		Fired();
	}
	Func Fired;
};
struct sGameCommand : public fwCheat{
	unsigned long Command;
	sGameCommand(unsigned long Command, std::function<void()> func) : fwCheat(func) {
		this->Command = Command;
	}
};
template<typename T> class fwCheatMgr {
public:
	static_assert(std::is_base_of<fwCheat, T>::value, "T is not a base of fwCheat");
	template<typename... Args>
	void Add(Args&&... args) {
		this->m_Cheats.push_back(T(std::forward<Args>(args)...)); // icl ts pmo
	}
	void Add(T& val) {
		this->m_Cheats.push_back(val);
	}
	void Remove(T& val) {
		this->m_Cheats.erase(std::find(m_Cheats.begin(), m_Cheats.end(), val));
	}
	std::vector<T>& GetList() { return this->m_Cheats; }
private:
	std::vector<T> m_Cheats;
};
class CCheatMgr {
public:
	void Update();
	fwCheatMgr<sGameCommand>* GetBase() { return &this->Commands; }
private:
	fwCheatMgr<sGameCommand> Commands;
};