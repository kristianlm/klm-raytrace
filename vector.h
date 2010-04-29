
#pragma once

#include <windows.h>

#include "image.h"
#include "intersectinfo.h"

#define MX 			1024
#define MY 			768
#define SCALE 		1

#define NUMLINES	9

extern int SX, SY, LVL;

#define PI (3.1415926535897932384626433832795)

void Line(float x1, float y1, float x2, float y2, int col);

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

class PLANE
{
public:
	PLANE();
	VECTOR n;
	float D;

	//	* Storage info:
	// u,v Center point.
	VECTOR p;
	// Coordinate basis.
	VECTOR ub, vb;
	// Rectangle definition
	float u1, u2;
	float v1, v2;

	MYIMAGE tex;
	int col;

	bool reflecting;
	bool bump;
	MYIMAGE bumpmap;

	void IntersectInfo(LINE* v, INTERSECTINFO* ii, int level);
	void CalcN();
};


extern PLANE o[100];
extern int numPlanes;
