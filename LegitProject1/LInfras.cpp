#include "LInfras.h"
CNetLogger* LAGInterface::sm_pNetLogger = nullptr;
CLogger* LAGInterface::sm_pLogger = nullptr;
#include <stdio.h>
#include <stdarg.h>
void LAGInterface::Init()
{
    sm_pLogger = new CLogger();
    sm_pNetLogger = new CNetLogger();
    sm_pNetLogger->Init();
    
}

CNetLogger* LAGInterface::GetNetLogger()
{
    return sm_pNetLogger;
}

CLogger* LAGInterface::GetFileLogger()
{
    return sm_pLogger;
}

void LAGInterface::Destroy()
{
    sm_pNetLogger->Shutdown(); delete sm_pNetLogger; sm_pNetLogger = nullptr;
    sm_pLogger->Close(); delete sm_pLogger; sm_pLogger = nullptr;
}
void LAGInterface::Write(const char* msg, ...)
{
    if (sm_pLogger == nullptr || sm_pNetLogger == nullptr) return;
    va_list args;
    va_start(args, msg);
    char buffer[1028] = { 0 };
    _vsnprintf_s(buffer, sizeof(buffer), msg, args);
    va_end(args);
    if (sm_pNetLogger->IsConnected()) {
        sm_pNetLogger->Write(buffer);
    }
    else {
        sm_pLogger->Write(buffer);
    }
}
void LAGInterface::Writeln (const char* msg, ...)
{
	if (sm_pLogger == nullptr || sm_pNetLogger == nullptr) return;

    va_list args;
    va_start(args, msg);
    char buffer[1028] = { 0 };
    _vsnprintf_s(buffer, sizeof(buffer), msg, args);
    va_end(args);
    if (sm_pNetLogger->IsConnected()) {
        sm_pNetLogger->Write(buffer);
        sm_pNetLogger->Write("\n");
    }
    else {
        sm_pLogger->Write(buffer);
        sm_pLogger->Write("\n");
    }
    
}