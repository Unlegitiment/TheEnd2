#pragma once
#include "ModelLoad.h"
typedef unsigned long Hash;
class CStreamingModel : public fwStreamObject {
public:
	CStreamingModel(Hash _hash) : hash(_hash){}
	Hash GetHash() { return this->hash; } // hash?
	// Inherited via fwStreamObject
	void Request() override;
	bool IsLoaded() override;
	void Derequest() override;
private:
	Hash hash;


};