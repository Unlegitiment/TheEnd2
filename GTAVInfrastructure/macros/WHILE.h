#pragma once
bool __WAITONG(unsigned long ms = 0x0);
#define WHILE_EX(ms, condition) while(condition && __WAITONG(ms))
#define WHILE(condition) while(condition && __WAITONG())
