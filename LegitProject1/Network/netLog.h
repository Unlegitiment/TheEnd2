#pragma once
#include "obj\basesocket.h"
class CNetLogger {
public:
	void Init();
	bool IsConnected();
	void Write(const char* fmt, ...);
	void Shutdown();
private:
	CNetSocket* m_pSocket = new CNetSocket();
};