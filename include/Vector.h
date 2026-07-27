#pragma once

#include <sstream>
#include <Types.h>
#include <Reader.h>

template<typename T>
struct Vector3Template {
	T x = 0;
	T y = 0;
	T z = 0;

	Vector3Template() = default;
	Vector3Template(T aX, T aY, T aZ)
		: x(aX)
		, y(aY)
		, z(aZ) {
	}
};

struct Vector3f : public Vector3Template<float> {
	Vector3f() = default;
	Vector3f(float aX, float aY, float aZ) 
		: Vector3Template(aX, aY, aZ) {
	}

	inline void Read(Reader& reader) {
		x = reader.ReadF32();
		y = reader.ReadF32();
		z = reader.ReadF32();
	}

	friend std::ostream& operator<<(std::ostream& os, Vector3f& v) {
		return os << v.x << ", " << v.y << ", " << v.z;
	}
};

struct Vector3b : public Vector3Template<u8> {
	Vector3b() = default;
	Vector3b(u8 aX, u8 aY, u8 aZ)
		: Vector3Template(aX, aY, aZ) {
	}

	inline void Read(Reader& reader) {
		x = reader.ReadU8();
		y = reader.ReadU8();
		z = reader.ReadU8();
	}

	friend std::ostream& operator<<(std::ostream& os, Vector3b& v) {
		return os << v.x << ", " << v.y << ", " << v.z;
	}
};

template<typename T>
struct Vector2Template {
	T x = 0;
	T y = 0;

	Vector2Template() = default;
	Vector2Template(T aX, T aY)
		: x(aX)
		, y(aY) {
	}
};

struct Vector2f : public Vector2Template<float> {
	Vector2f() = default;
	Vector2f(float aX, float aY)
		: Vector2Template(aX, aY) {
	}

	inline void Read(Reader& reader) {
		x = reader.ReadF32();
		y = reader.ReadF32();
	}

	friend std::ostream& operator<<(std::ostream& os, Vector2f& v) {
		return os << v.x << ", " << v.y;
	}
};