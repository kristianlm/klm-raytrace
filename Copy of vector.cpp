

#include <math.h>

#include "Copy of vector.h"

extern HDC hdc;

COLORINFO::COLORINFO()
{
	r = g = b = 0;
}

void Line(float x1, float y1, float x2, float y2, int col)
{
	x1 /= SCALE;
	y1 /= SCALE;
	x2 /= SCALE;
	y2 /= SCALE;
	
	float x = x1;
	float y = y1;
	VECTOR d;
	d.x = x2 - x1;
	d.y = y2 - y1;
	
	float dist = d.Abs();
	if(dist > 1.0) 
		d.Normalize();
	
	for(int i = 0 ; i < dist ; i++)
	{
		SetPixelV(hdc, x, y, col);
		x += d.x;
		y += d.y;
	}
}

float VECTOR::Dot(VECTOR* v)
{
	return v->x*x + v->y*y;
}
float VECTOR::Abs()
{
	return sqrt(x*x + y*y);
}

void VECTOR::Normalize()
{
	float abs = Abs();
 	x /= abs;
 	y /= abs; 	
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
	Line(p.x, p.y, p.x + length*d.x, p.y + length*d.y, col);
	return;
	float x = p.x;
	float y = p.y;
	
	for(int i = 0 ; i < length ; i++)
	{
		SetPixelV(hdc, x, y, col);
		x += d.x;
		y += d.y;
	}
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
