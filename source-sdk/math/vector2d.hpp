#pragma once

#include "vector3d.hpp"
class Vector2 {
public:
	float x, y;

	Vector2() {
		x = 0; y = 0;
	};
	Vector2(float X, float Y) {
		x = X; y = Y;
	};
	Vector2(Vector3 vec) {
		x = vec.x; y = vec.y;
	}

	inline Vector2 operator*(const float n) const {
		return Vector2(x * n, y * n);
	}
	inline Vector2 operator+(const Vector2& v) const {
		return Vector2(x + v.x, y + v.y);
	}
	inline Vector2 operator-(const Vector2& v) const {
		return Vector2(x - v.x, y - v.y);
	}
	inline void operator+=(const Vector2& v) {
		x += v.x;
		y += v.y;
	}
	inline void operator-=(const Vector2& v) {
		x -= v.x;
		y -= v.y;
	}

	bool operator==(const Vector2& v) const {
		return (v.x == x && v.y == y);
	}
	bool operator!=(const Vector2& v) const {
		return (v.x != x || v.y != y);
	}

	inline float length() {
		return sqrt((x * x) + (y * y));
	}
};
