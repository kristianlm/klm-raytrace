
#include <windows.h>
#include <math.h>


#include "draw.h"

void DrawCircle(HDC hdc, float x, float y, float rad, int col)
{
	for(int xx = -rad ; xx < rad ; xx++)
	{
		int yp = sqrt(rad*rad - xx*xx);
		for(int yy = -yp ; yy < yp ; yy++)
		{
			SetPixelV(hdc, x + xx, y + yy, col);
		}
	}
}

void DrawLine(HDC hdc, float x1, float y1, float x2, float y2, int col)
{
	float xd = x2 - x1;
	float yd = y2 - y1;
	float dist = sqrt(xd*xd + yd*yd);
	xd /= dist;
	yd /= dist;

	float xx = x1, yy = y1;
	for(int i = 0 ; i < dist ; i++)
	{
		xx += xd;
		yy += yd;
		SetPixelV(hdc, xx, yy, col);
	}
}

void DrawCircle(MYIMAGE* im, float x, float y, float rad, int col)
{
	for(int xx = -rad ; xx < rad ; xx++)
	{
		int yp = sqrt(rad*rad - xx*xx);
		for(int yy = -yp ; yy < yp ; yy++)
		{
			im->SetPixel(x + xx, y + yy, col);
		}
	}
}

void DrawLine(MYIMAGE* im, float x1, float y1, float x2, float y2, int col)
{
	float xd = x2 - x1;
	float yd = y2 - y1;
	float dist = sqrt(xd*xd + yd*yd);
	xd /= dist;
	yd /= dist;

	float xx = x1, yy = y1;
	for(int i = 0 ; i < dist ; i++)
	{
		xx += xd;
		yy += yd;
		im->SetPixel(xx, yy, col);
	}
}
