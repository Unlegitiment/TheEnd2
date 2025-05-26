#pragma once
#include <iostream>
class CLogger {
public:
	CLogger() {
		auto test = fopen_s(&file, "Test.log", "w+");
		this->Write("Opened log\n");
	}
	void Write(const char* fmt, ...);
	void Close() {
		fclose(file);
	}
private:
	FILE* file = nullptr;
};