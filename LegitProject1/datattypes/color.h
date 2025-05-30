#pragma once
template<typename T>
class _Color {
public:
	_Color() : m_R(0), m_G(0), m_B(0), m_A(0) {}
	_Color(T r, T g, T b, T a) {
		this->m_R = r;
		this->m_G = g;
		this->m_B = b;
		this->m_A = a;
	}
	T GetR() const { return m_R; }
	T GetG() const { return m_G; }
	T GetB() const { return m_B; }
	T GetA() const { return m_A; }
	void SetR(T r) { m_R = r; }
	void SetG(T G) { m_G = G; }
	void SetB(T B) { m_B = B; }
	void SetA(T A) { m_A = A; }
	_Color<T> Normalize() { return (m_R/255, m_G/255, m_B/255, m_A/255); } // template specialize for double + float
	bool IsEmpty() const { return m_R == 0 && m_G == 0 && m_B == 0 && m_A == 0; }
private:
	T m_R, m_G, m_B, m_A;
};
typedef _Color<float> Color32;
typedef _Color<double>Color64;