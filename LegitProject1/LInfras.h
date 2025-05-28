#pragma once
#include "network/netLog.h"
#include "fi/logger.h"
class LAGInterface {
public:
	static void Init();
	static CNetLogger* GetNetLogger();
	static CLogger* GetFileLogger();
	static void Write(const char* msg, ...);
	static void Writeln(const char* msg, ...);
	static void Destroy();
private:
	static CNetLogger* sm_pNetLogger; // make these ptrs because we want to control their lifetime.
	static CLogger* sm_pLogger;
};