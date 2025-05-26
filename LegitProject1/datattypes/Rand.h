#pragma once 
#include <random>
#include "..\LInfras.h"

template<typename T = int>
class CRand {
public:
	CRand() :rng{ std::random_device{}() } {

	}
	T operator()() {
		return dist(rng);
	}
	~CRand() {
		
	}
private:
	std::mt19937 rng; // This is inefficient. Move to static or global initialize a driver. This could also lead to conflict between inter-system communication? I'm not exactly 100% positive on this rand stuff. Right now.
	std::uniform_int_distribution<T> dist;
};