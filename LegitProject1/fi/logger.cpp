#include "logger.h"
#include <stdio.h>
#include <stdarg.h>

void CLogger::Write(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	char buffer[1028] = { "\0" };
	_vsnprintf_s(buffer, sizeof(buffer), fmt, args);
	va_end(args);
	size_t size = strnlen_s(buffer, 1028);
	fwrite(buffer, sizeof (char), size, file);
	fflush(file);
}
