#pragma once
#include <typeinfo>
#include <GTAVInfrastructure\SHV\natives.h>
#include <ScaleformUILib\DataTypes\Point.h>
#include "..\DataTypes\Screen.h"
#include <LegitProject1\datattypes\vector.h>
class ScaleformWideScreen {
public:
	ScaleformWideScreen(const char* scaleformId) {

	}
	virtual ~ScaleformWideScreen() {
		Dispose();
	}
	void Dispose() {
		if (IsLoaded()) {
			GRAPHICS::SET_SCALEFORM_MOVIE_AS_NO_LONGER_NEEDED(&_handle);
		}
		// GC.SuppressFinalize(this) -- Not needed C++ no Garbage Collector.
	}
	int GetHandle() { return this->_handle; }
	bool IsValid() {
		return _handle != 0;
	}
	bool IsLoaded() {
		GRAPHICS::HAS_SCALEFORM_MOVIE_LOADED(_handle);
	}
	template<typename... Args>
	void CallFunction(const char* function, Args&&... args) {
		GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(_handle, function);
		Call(std::forward(args)...); // I hate C++ :/
		GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
	}
	template<typename T> void Call(T data) {
		LAGInterface::Writeln(__FUNCTION__"<%s> Unknown Type! Cannot perform Call Operation", typeid(T).name()); // @Todo: Move away or keep typeid(T).name?
		return;
	};
	void Call<int>(int data){
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(data);
	}
	void Call<const char*>(const char* data){ // @Todo ScaleformUI C# does a custom action if it begins with "b_" || "t_" fix.
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING(data);
	}
	void Call<bool>(bool data){
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_BOOL(data);
	}
	void Call<float>(float data){
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_FLOAT(data);
	}
	void Call<double>(double data) {
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_FLOAT(static_cast<float>(data)); // no valid double parametsr/
	}
	template<typename... Args>
	int CallFunctionReturnValueInt(const char* function, Args&&... args) { // Warn: No async, this will hijack the thread.
		int ret = CallFunctionReturnInternal(function, args);
		while (!GRAPHICS::IS_SCALEFORM_MOVIE_METHOD_RETURN_VALUE_READY(ret)) WAIT(0);
		return GRAPHICS::GET_SCALEFORM_MOVIE_METHOD_RETURN_VALUE_INT(ret);
	}
	template<typename... Args>
	bool CallFunctionReturnValueBool(const char* function, Args&&... args) { // Warn: No async, this will hijack the thread.
		int ret = CallFunctionReturnInternal(function, args);
		while (!GRAPHICS::IS_SCALEFORM_MOVIE_METHOD_RETURN_VALUE_READY(ret)) WAIT(0);
		return GRAPHICS::GET_SCALEFORM_MOVIE_METHOD_RETURN_VALUE_BOOL(ret);
	}
	template<typename... Args>
	const char* CallFunctionReturnValueString(const char* function, Args&&... args) { // Warn: No async, this will hijack the thread.
		int ret = CallFunctionReturnInternal(function, args);
		while (!GRAPHICS::IS_SCALEFORM_MOVIE_METHOD_RETURN_VALUE_READY(ret)) WAIT(0);
		return GRAPHICS::GET_SCALEFORM_MOVIE_METHOD_RETURN_VALUE_STRING(ret);
	}
	void Render2D() {
		GRAPHICS::DRAW_SCALEFORM_MOVIE_FULLSCREEN(_handle, 255, 255, 255, 255, 0);
	}
	void Render2DScreenSpace(PointF location, PointF size) {
		float x = location.X() / Screen::Width();
		float y = location.Y() / Screen::Height();
		float width = size.X() / Screen::Width();
		float height = size.Y() / Screen::Height();
		GRAPHICS::DRAW_SCALEFORM_MOVIE(_handle, x + (width /2.0f), y + (height /2.0f), width, height, 255, 255, 255, 255, 0);
	}
	void Render3D(CVector3 position, CVector3 rotation, CVector3 scale) {
		GRAPHICS::DRAW_SCALEFORM_MOVIE_3D_SOLID(_handle, position.GetX(), position.GetY(), position.GetZ(), rotation.GetX(), rotation.GetY(), rotation.GetZ(), 2.0, 2.0, 1.0, scale.GetX(), scale.GetY(), scale.GetZ(), 2);
	}
	void Render3DAdditive(CVector3 position, CVector3 rotation, CVector3 scale) {
		GRAPHICS::DRAW_SCALEFORM_MOVIE_3D(_handle, position.GetX(), position.GetY(), position.GetZ(), rotation.GetX(), rotation.GetY(), rotation.GetZ(), 2.0, 2.0, 1.0, scale.GetX(), scale.GetY(), scale.GetZ(), 2);
	}
private:
	template<typename... Args>
	int CallFunctionReturnInternal(const char* func, Args&&... args) {
		GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(_handle, func);
		Call(std::forward(args)...);
		return GRAPHICS::END_SCALEFORM_MOVIE_METHOD_RETURN_VALUE();
	}
	int _handle;
};