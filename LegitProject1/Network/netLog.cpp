#include "netLog.h"
#include <stdio.h>
#include <stdarg.h>

void CNetLogger::Init()
{
	this->m_pSocket->Open("127.0.0.1", "27015", m_pSocket->TCP);
	Write(RGreen"Socket accepted; Client online\n" RNorm);
}

bool CNetLogger::IsConnected()
{
	return this->m_pSocket->IsConnected();
}

void CNetLogger::Write(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	char buffer[1028] = { 0 };
	_vsnprintf_s(buffer, sizeof(buffer), fmt, args);
	va_end(args);
	this->m_pSocket->Write(buffer);
}

void CNetLogger::Shutdown()
{
	this->Write(RRed "Shutting down netLogger!" RNorm);
	this->m_pSocket->Close();
}
