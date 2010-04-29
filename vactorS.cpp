

#include <math.h>
#include <stdio.h>

#include "draw.h"
#include "vectorS.h"

float VECTOR::Dot(VECTOR* v)
{
	return v->x*x + v->y*y + v->z*z;
}
float VECTOR::Abs()
{
	return sqrt(x*x + y*y + z*z);
}

void VECTOR::Normalize()
{
	float abs = Abs();
 	x /= abs;
 	y /= abs;
 	z /= abs;
}
VECTOR VECTOR::Cross(VECTOR* v)
{
	VECTOR r;
	r.x =  y*v->z - z*v->y;
	r.y = -x*v->z + z*v->x;
	r.z =  x*v->y - y*v->x;
	return r;
}


void LINE::SetDirection(float x, float y)
{
	d.x = x; d.y = y;
	d.Normalize();
}
void LINE::SetPoint(float x, float y)
{
	p.x = x; p.y = y;
}
void LINE::SetLength(float l)
{
	length = l;
}



float LINE::Distance(float xx, float yy, float* t)
{
	VECTOR v;
	v.x = xx - p.x;
	v.y = yy - p.y;

	*t = d.Dot(&v);

	if(*t < 0) // Point is outside line
	{
		//Line(p.x, p.y, v.x+p.x, v.y+p.y, 0xFF);
 	}
	else if(*t > length)
	{
		//Line(p.x+length*d.x, p.y+length*d.y, v.x+p.x, v.y+p.y, 0xFF00);
		v.x -= d.x * length;
		v.y -= d.y * length;
	}
	else
	{
		v.x -= *t * d.x;
		v.y -= *t * d.y;
		//Line(p.x, p.y, v.x+p.x, v.y+p.y, 0xFF00FF);
	}


	return v.Abs();
}

void LINE::Draw(HDC hdc, int col)
{
	DrawLine(hdc, p.x, p.y, p.x + length*d.x, p.y + length*d.y, col);
	return;
}
void LINE::ColorInfo(float x, float y, COLORINFO* ci)
{

	float t;
	float dist = Distance(x, y, &t);
	if(t < 0 || t > length) t = 0;

	if(dist < 1200.01)
	{
		float r = 1.0 * sin((t / length) * atan(1.0) * 4.0);
		if(r == 0) r = 0.0001;
		dist /= r * 20;

		if(dist == 0.0) dist = 0.0001;
		ci->r += 200.0 / pow(dist, (float)0.8);
		ci->g += 50.0 / pow(dist, (float)0.9);
		ci->b += 4.0 / pow(dist, (float)1.3);

	}
	else {
		//r += 255;
		//g += 50;
		//b += 10;
	}
}


