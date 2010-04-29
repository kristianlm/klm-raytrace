
#pragma once

#include <windows.h>

#include "image.h"

#define PI (3.1415926535897932384626433832795)


class VECTOR
{
public:

	float x, y, z;

	float Dot(VECTOR* v);
	float Abs();

	void Normalize();
	VECTOR Cross(VECTOR* v);

};


class LINE
{
public:

	VECTOR p, d;
	float length;

	void SetDirection(float x, float y);
	void SetPoint(float x, float y);
	void SetLength(float l);

	void Draw(HDC hdc, int col);
	float Distance(float x, float y, float* t);

	void ColorInfo(float x, float y, COLORINFO* ci);

};
