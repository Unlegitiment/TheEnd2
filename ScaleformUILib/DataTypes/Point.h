#pragma once
template<typename T> class Point {
public:
	Point(T __x, T __y) : _x(__x), _y(__y){ }
	T& X() { return _x; }
	T& Y() { return _y; }
private:
	T _x, _y;
};
using PointF = Point<float>;
