

#include <windows.h>



#define MX 			640
#define MY 			480
#define SCALE 		1

#define NUMLINES	9




class COLORINFO
{
public:
	COLORINFO();
	float r, g, b;
};

class VECTOR
{
public:

	float x, y;
	
	float Dot(VECTOR* v);
	float Abs();
	
	void Normalize();
	
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
