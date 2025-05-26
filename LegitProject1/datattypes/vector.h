#pragma once
#include "GTAVInfrastructure\SHV\types.h"
#include <cmath>
template<typename T>
class _CVector3 {
public:
	_CVector3(Vector3 vec) {
		this->x = static_cast<T>(vec.x);
		this->y = static_cast<T>(vec.y);
		this->z = static_cast<T>(vec.z);
	}
	_CVector3(T x, T y, T z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	void SetX(const T x) {
		this->x = x;
	}
	void SetY(const T y) {
		this->y = y;
	}
	void SetZ(const T z) {
		this->z = z;
	}
	Vector3 GetAsSHV() { return { x,0,y,0,z,0 }; } // have to include padding lmao
	T GetX() const { return x; }
	T GetY() const { return y; }
	T GetZ() const { return z; }
	bool operator==(const _CVector3<T> oth) { return this->x == oth.x && this->y == oth.y && this->z = oth.z; }
	T Dist(const _CVector3<T>& oth) {
		return std::sqrt(std::pow(oth.x - this->x, 2) + std::pow(oth.y - this->y, 2) + std::pow(oth.z - this->z, 2));
	}; // wtf
	T DistNoZ(const _CVector3<T>& oth) {
		return std::sqrt(std::pow(oth.x - this->x, 2) + std::pow(oth.y - this->y, 2));
	}
private:
	T x, y, z;
};
using CVector3 = _CVector3<float>;