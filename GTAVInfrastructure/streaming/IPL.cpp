#include "IPL.h"
#include "GTAVInfrastructure/SHV/natives.h"
#include <LegitProject1\LInfras.h>
void fwIPL::Request()
{
	STREAMING::REQUEST_IPL(this->m_aName);
	this->status = REQUESTED;
	int NumTries = 0;
	static constexpr int MAX_TRIES = 10;
	while (!STREAMING::IS_IPL_ACTIVE(this->m_aName) && NumTries <= MAX_TRIES) {
		this->status = LOADING;
		STREAMING::REQUEST_IPL(this->m_aName);
		NumTries++;
		WAIT(0);
	}
	if (NumTries >= MAX_TRIES) {
		LAGInterface::Writeln("IPL: %s Failed to load max retries reached", this->m_aName);
		this->status = FAILED;
		return;
	}
	this->status = LOADED;
}

void fwIPL::Derequest()
{
	if (!IsLoaded()) {
		this->status = INACTIVE;
	}
	STREAMING::REMOVE_IPL(this->m_aName);
	this->status = INACTIVE;
}

bool fwIPL::IsLoaded()
{
	return STREAMING::IS_IPL_ACTIVE(this->m_aName);
}
#include <string.h>
bool fwIPL::operator==(const fwIPL& other)
{
	return strncmp(this->m_aName, other.m_aName, 256) == 0; 
}
