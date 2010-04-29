

#include <windows.h>
#include <math.h>
#include <stdio.h>

#include "Camera.h"
#include "Vector.h"

extern HDC hdc;
CAMERA Camera;

void CAMERA::CalcUV()
{

	VECTOR down;
	down.z = down.x = 0.0;
	down.y = -1.0;

	cu = down.Cross(&d);
	cv =   cu.Cross(&d);

	cu.Normalize();
	cv.Normalize();

}



void RayTrace(float x, float y, COLORINFO* ci, bool text )
{
	LINE l;
	l.p.x = Camera.p.x;// (x - float(MX/2)) / 40.0f;
	l.p.y = Camera.p.y;//-(y - float(MY/2)) / 40.0f;
	l.p.z = Camera.p.z;

	float uu = ( (x - float(SX/2))) / (SX / 2.0);
	float vv = (-(y - float(SY/2))) / (SY / 2.0);

	float zoom = 1.0;
	float us = uu * sin(PI*0.225); // u scaler
	float vs = vv * sin(PI*0.225); // v scaler
	l.d.x = Camera.d.x + Camera.cu.x * us + Camera.cv.x * vs;
	l.d.y = Camera.d.y + Camera.cu.y * us + Camera.cv.y * vs;
	l.d.z = Camera.d.z + Camera.cu.z * us + Camera.cv.z * vs;
	l.d.Normalize();

	INTERSECTINFO ii;
	INTERSECTINFO ii2;
	float mint = 100000.0;
	int usedi = 0;
	for(int i = 0 ; i <= numPlanes ; i++)
	{
	   	ii.valid = 0;
		o[i].IntersectInfo(&l, &ii, 2);
		if(ii.valid && ii.t < mint)
		{
		    ii2 = ii;
			*ci = ii.ci;
			mint = ii.t;
			usedi = i;
		}
	}
	if(ii2.valid)
	{
		o[usedi].IntersectInfo(&l, &ii2, LVL);
		*ci = ii2.ci;
	}


	if(text){
		char tt[200];
		sprintf(tt, "#%d. n(%.1f,%.1f,%.1f) D(%.1f) - uv(%.1f, %.1f) - t(%.1f)                      ", usedi, o[usedi].n.x,o[usedi].n.y,o[usedi].n.z, o[usedi].D, ii2.u, ii2.v, ii2.t);
		TextOut(hdc, 0, 480+18*2, tt, strlen(tt));
	}
}

