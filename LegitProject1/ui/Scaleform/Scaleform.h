#pragma once
#include <GTAVInfrastructure\SHV\natives.h>
#include <LegitProject1\LInfras.h>
#include <type_traits>
class CScaleform {
public:
	explicit CScaleform(int Handle) :m_Handle(Handle) {} // fuck shit 
	template<typename... T> bool CallScaleform(const char* method, T&&... args) {
		bool bRes = GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(m_Handle, method);
		(Call(std::forward<T>(args)), ...); // this syntax is just so weird. 
		GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
		return bRes;
	}
	operator int() {
		return m_Handle;
	}
	int GetHandle() const { return m_Handle; }
	template<typename T> void Call(T data) {
		if constexpr (std::is_enum_v<T>) {
			Call((int)data);
			return;
		}
		LAGInterface::Writeln("Call<T>: Unhandled template specialization type: %s", typeid(T).name());
	}
	template<> void Call<int>(int data) {
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(data);
	}
	template<> void Call<const char*>(const char* data) {
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING(data);
	}
	template<> void Call<char*>(char* data) {
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING(data);
	}
	template<> void Call<char[]>(char data[]) {
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING(data);
	}
	template<> void Call<float>(float data) {
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_FLOAT(data);
	}
	template<> void Call<bool>(bool data) {
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_BOOL(data);
	}
private:
	int m_Handle = 0;
};
