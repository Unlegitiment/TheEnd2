#pragma once
#include "GTAVInfrastructure/SHV/natives.h"
#include "Point.h"
class Screen {
public:
	static int Width() { 
		return Point().X();
	}
	static int Height() {
		return Point().Y();
	}
	static Point<int> Point() {
		int x{}; int y{};
		GRAPHICS::GET_ACTUAL_SCREEN_RESOLUTION(&x, &y);
		return { x,y };
	}
private:

};