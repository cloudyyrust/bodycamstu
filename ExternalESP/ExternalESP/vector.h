// Vector.h
#pragma once
#include <cmath>

struct Vector2 {
    float x, y;
    Vector2() : x(0.f), y(0.f) {}
    Vector2(float x, float y) : x(x), y(y) {}
};

struct Vector3 {
    float x, y, z;
    Vector3() : x(0.f), y(0.f), z(0.f) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

    float Distance(const Vector3& v) {
        return float(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
    }
};

struct Vector4 {
    float x, y, z, w;
    Vector4() : x(0.f), y(0.f), z(0.f), w(0.f) {}
    Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
};