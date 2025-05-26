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
		LAGInterface::Writeln("CRand::~CRand() Destructor Called");
		
	}
private:
	std::mt19937 rng; // this might be a problem. Ideally inst inside of constructor.
	std::uniform_int_distribution<T> dist;
};