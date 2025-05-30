#pragma once
template<typename T>
class PointerLike {
public:
	PointerLike() {
		this->m_Cleaner = new T();
	}
	template<typename... Args>
	PointerLike(Args&& args) {
		this->m_Cleaner = new T(std::forward<Args>(args)...);
	}
	T* Get() { return this->m_Cleaner; }
	~PointerLike() {
		delete m_Cleaner;
	}
private:
	T* m_Cleaner
};