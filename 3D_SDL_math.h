#include "dependencies.h"

#ifndef MATH_H
#define MATH_H


class Vec3f {
public:
        float x, y, z;
		Vec3f() : x(0.0f), y(0.0f), z(0.0f) {}
		Vec3f(float, float, float);
		Vec3f operator*(const Vec3f &) const;
		Vec3f operator+(const Vec3f &) const;
		Vec3f operator*(const float &) const;
		Vec3f operator-() const;
		static Vec3f LERP(const Vec3f &, const Vec3f &, float);
		float Mag() const;
		void Normalize();
}; 


class Quat {
public:
	Vec3f v; float s;
	Quat(const Vec3f &, const float);
	Quat();
	static Quat CalculateQuat (const Vec3f &, float);
	Quat operator*(const Quat &) const;
	float operator[](const int);
	Vec3f Rotate(const Vec3f &) const; 
	Quat operator-() const;
	float Mag() const;
	void Normalize();
};

class Matrix {
public:  
	Matrix();
	float* operator[](const int);
	Vec3f TransformP(const Vec3f &) const;
	void RotateX(const float);
	void RotateY(const float);
	void RotateZ(const float);
	void Translate(const Vec3f &);
	void Scale(const float);
	Matrix operator*(Matrix &);
private:
	float m[4][3];
};

float dot(const Vec3f &a, const Vec3f &b);

#endif