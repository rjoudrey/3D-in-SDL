#include "3d_SDL_math.h"

// Constructs a matrix from a concatenation of two other matrices.
Matrix Matrix::operator*(Matrix &b) 
{
	Matrix q;
	float f = q[0][2];
	for (int row = 0; row < 4; row++)
		for (int col = 0; col < 3; col++)
		{
			q[row][col] = 0.0f;
			for (int comp = 0; comp < 3; comp++)
				q[row][col] += this->m[row][comp] * b[col][comp];
		}
	return q;
}


Matrix::Matrix()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 3; j++)
			m[i][j] = 0.0f;
	m[0][2] = 69.f;
}
float* Matrix::operator[](const int i)
{
	return m[i];
}

Vec3f::Vec3f(float x, float y, float z) { 
	this->x = x; this->y = y; this->z = z; 
}
Vec3f Vec3f::operator*(const Vec3f &v) const {
	return Vec3f(this->y * v.z - this->z * v.y, 
				 this->z * v.x - this->x * v.z,
				 this->x * v.y - this->y * v.x);
}
Vec3f Vec3f::operator+(const Vec3f &v) const {
	return Vec3f(this->x + v.x, this->y + v.y, this->z + v.z);
}
Vec3f Vec3f::operator*(const float &s) const {
	return Vec3f(this->x * s, this->y * s, this->z * s);
}

Quat Quat::operator-() const {
	Quat q; q.v.x = -this->v.x; q.v.y = -this->v.y; q.v.z = -this->v.z; 
	q.s = this->s;
	return q;
}

//Grassman Product - Gives a composite of two rotations.
Quat Quat::operator*(const Quat &q) const
{
	Quat r; r.v = (q.v * s) + (v * q.s) + (v * q.v);
	r.s = s * q.s - dot(v, q.v);
	return r;
}

float Quat::Mag() const
{
	return sqrt(pow(v.x, 2.0f) + pow(v.y, 2.0f) + pow(v.z, 2.0f) + pow(s, 2.0f));
}

void Quat::Normalize()
{
	float mag = this->Mag();
	this->v.x /= mag, this->v.y /= mag, this->v.z /= mag; this->s /= mag;
}

Vec3f Quat::Rotate(const Vec3f &v) const  {
	Quat qV; qV.v = v; qV.s = 0.0f;
	return (*this * qV * -*this).v; 
}

// Used to find a point that is t percent along the way from A to B
Vec3f Vec3f::LERP(const Vec3f &a, const Vec3f &b, float t)
{
	Vec3f rV;
	rV.x = (1 - t) * a.x + t * b.x;
	rV.y = (1 - t) * a.y + t * b.y;
	rV.z = (1 - t) * a.z + t * b.z;
	return rV;
}

float Vec3f::Mag() const
{
	return sqrt(pow(x, 2.0f) + pow(y, 2.0f) + pow(z, 2.0f));
}

void Vec3f::Normalize()
{
	float mag = this->Mag();
	this->x /= mag, this->y /= mag, this->z /= mag;
}

// Transforms a vector by a matrix
Vec3f Matrix::TransformP(const Vec3f &p) const
{
	Vec3f rV;  
	rV.x = p.x * m[0][0] + p.y * m[1][0] + p.z * m[2][0] + m[3][0];
	rV.y = p.x * m[0][1] + p.y * m[1][1] + p.z * m[2][1] + m[3][1];
	rV.z = p.x * m[0][2] + p.y * m[1][2] + p.z * m[2][2] + m[3][2];
	return rV;
}

Quat::Quat()
{
	v.x = v.y = v.z = s = 0.0f;
}

Quat::Quat(const Vec3f &v, float t)
{ 
	this->v = v; this->s = t;
}

Quat Quat::CalculateQuat(const Vec3f &v, float t)
{
	return Quat(Vec3f(v.x * sin(t/2), v.y * sin(t/2), v.z * sin(t/2)),  cos(t/2));
}

float Quat::operator[](const int i)
{
	if (i > 3 || i < 0) exit(-2); // Assertion Check
	switch(i) {
		case 0: return v.x;
		case 1: return v.y;
		case 2: return v.z;
		case 3: return s;
		default: exit(-3);
	}
}

// Given an angle t in radians, a pure rotation matrix is created (around x-axis)
void Matrix::RotateX(const float t)
{
	m[0][0] = 1.0f;	 m[0][1] = 0.0f;     m[0][2] = 0.0f;
	m[1][0] = 0.0f;	 m[1][1] = cos(t);	 m[1][2] = sin(t);
	m[2][0] = 0.0f;	 m[2][1] = -sin(t);  m[2][2] = cos(t);
	m[3][0] = 0.0f;  m[3][1] = 0.0f;     m[3][2] = 0.0f;
}

void Matrix::RotateY(const float t)
{
	m[0][0] = cos(t);  m[0][1] = 0.0f;    m[0][2] = -sin(t);
	m[1][0] = 0.0f;    m[1][1] = 1.0f;    m[1][2] = 0.0f;
	m[2][0] = sin(t);  m[2][1] = 0.0f;    m[2][2] = cos(t);
	m[3][0] = 0.0f;    m[3][1] = 0.0f;     m[3][2] = 0.0f;
}

void Matrix::RotateZ(const float t)
{
	m[0][0] = cos(t);  m[0][1] = sin(t);   m[0][2] = 0.0f;
	m[1][0] = -sin(t); m[1][1] = cos(t);   m[1][2] = 0.0f;
	m[2][0] = 0.0f;	   m[2][1] = 0.0f;     m[2][2] = 1.0f;
	m[3][0] = 0.0f;    m[3][1] = 0.0f;     m[3][2] = 0.0f;
} 

void Matrix::Translate(const Vec3f &v)
{
	m[3][0] = v.x, m[3][1] = v.y, m[3][2] = v.z;
}

void Matrix::Scale(const float f)
{
	m[0][0] *= f; m[1][1] *= f; m[2][2] *= f;
}

float dot(const Vec3f &a, const Vec3f &b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}
