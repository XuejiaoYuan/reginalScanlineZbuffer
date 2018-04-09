#pragma once
#include "CommonFunc.h"

class Vector3D {
public:
	GLfloat x, y, z;
	Vector3D(GLfloat _x, GLfloat _y, GLfloat _z) :x(_x), y(_y), z(_z) {}
	Vector3D(GLfloat v[3]) :x(v[0]), y(v[1]), z(v[2]){}
	Vector3D() {};

	void operator= (const Vector3D& a) {
		x = a.x;
		y = a.y;
		z = a.z;
	}

	Vector3D operator+(const Vector3D& a) {
		return Vector3D(x + a.x, y + a.y, z + a.z);
	}

	Vector3D operator-(const Vector3D& a) {
		return Vector3D(x - a.x, y - a.y, z - a.z);
	}

	Vector3D operator*(const Vector3D& a) {
		return Vector3D(x * a.x, y * a.y, z * a.z);
	}

	Vector3D operator*(const float& a) {
		return Vector3D(x*a, y*a, z*a);
	}

	Vector3D operator/(const Vector3D& a) {
		if (!a.x || !a.y || !a.z) {
			cout << "Using zero to divide a number!!" << endl;
			exit(1);
		}
		return Vector3D(x / a.x, y / a.y, z / a.z);
	}

	bool operator==(const Vector3D& a) {
		return (a.x == x && a.y == y && a.z == z);
	}

	bool operator!=(const Vector3D& a) {
		return !((*this) == a);
	}

};