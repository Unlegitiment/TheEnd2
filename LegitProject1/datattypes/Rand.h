#pragma once 
#include <random>
#include "..\LInfras.h"
template<typename T = int>
class CRand {
    T GenerateRandom() {
        LAGInterface::Writeln("god damn it! T is not implemented you will be going toward a crash implement CRand for <%s>", typeid(T).name());
        return 0;
    }
};
template<>
class CRand<int> {
public:
    static int GenerateRandom() {
        static std::mt19937 rng{ std::random_device{}() };
        static std::uniform_int_distribution<int> dist;
        return dist(rng);
    }
};