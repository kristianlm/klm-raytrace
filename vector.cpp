

#include <math.h>
#include <stdio.h>

#include "vector.h"
#include "Camera.h"
#include "LightSource.h"

extern HDC hdc;
extern CAMERA Camera;

PLANE o[100];
int numPlanes = 2;

int SX = 150;
int SY = 100;
int LVL = 60;

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
PLANE::PLANE()
{
	reflecting = bump = 0;
	col = 0xFF;
}
void PLANE::CalcN()
{
	VECTOR pp;
	pp.x = -p.x;
	pp.y = -p.y;
	pp.z = -p.z;

	float a = ub.Dot(&pp);
	float b = vb.Dot(&pp);

	n.x = a * ub.x + b * vb.x - pp.x;
	n.y = a * ub.y + b * vb.y - pp.y;
	n.z = a * ub.z + b * vb.z - pp.z;

	// n is now point on plane closest to origin.
	float dist = n.Abs();

	n = vb.Cross(&ub);
	n.Normalize();

	float sign = -1.0;
	if(pp.Dot(&n) > 0.0) sign = 1.0;

	D = sign * dist;
}

void PLANE::IntersectInfo(LINE* line, INTERSECTINFO* ii, int level)
{

	float nevner = n.Dot(&line->d);

	if(nevner != 0.0)
	{
		ii->t = -(D + n.Dot(&line->p)) / nevner;
		if(ii->t < 100000.0 && ii->t > 0.0)
		{

			VECTOR hit;
			hit.x = line->p.x + line->d.x * ii->t - p.x;
			hit.y = line->p.y + line->d.y * ii->t - p.y;
			hit.z = line->p.z + line->d.z * ii->t - p.z;

			ii->u = hit.Dot(&ub);
			ii->v = hit.Dot(&vb);

			if(ii->u >= 0 && ii->v >= 0)
			{
				if(ii->v <= v2 && ii->u <= u2)
				{
					ii->valid = true;

					if(level < 10)
					{
						return;
					}

					COLORINFO ci;ii->ci.r = ii->ci.g = ii->ci.b = 0;

                    float count = 0.0;
                    float ts = 0.4;
                    int tx = fmod(((ii->u) / u2) * tex.mx*3, tex.mx);
                    int ty = fmod(((ii->v) / v2) * tex.my*3, tex.my);
                    //for(int vm = -ii->t * ts ; vm < ii->t * ts ; vm ++)
                    //for(int um = -ii->t * ts ; um < ii->t * ts ; um ++)
                    {
                        unsigned char* cc = (unsigned char* )&col;
                        tex.GetPixel(tx +0, ty +0, &ci);

                        ii->ci.r = ci.r;
                        ii->ci.g = ci.g;
                        ii->ci.b = ci.b;
                        //count++;
                    }

					if(level < 50)
						return;
                    //ii->ci.r /= count;
                    //ii->ci.g /= count;
                    //ii->ci.b /= count;

					//hit vecxtor is now where ray intersects with plane in world coords.
					hit.x += p.x;
					hit.y += p.y;
					hit.z += p.z;

                    // We got a color, now lets check it up against thelight source
                    //hit.Normalize();
					float strength = 0.0;
					/*if(bump)
					{
						VECTOR normal;
						COLORINFO bmcX1, bmcX2, bmcY1, bmcY2;

						int tx = ((ii->u) / u2) * bumpmap.mx;
						int ty = ((ii->v) / v2) * bumpmap.my;

						bumpmap.GetPixel(tx-1, ty, &bmcX1);
						bumpmap.GetPixel(tx+1, ty, &bmcX2);
						bumpmap.GetPixel(tx, ty-1, &bmcY1);
						bumpmap.GetPixel(tx, ty+1, &bmcY2);

						float BNX = bmcX1.r - bmcX2.r;
						float BNY = bmcY1.r - bmcY2.r;
						BNX *= .10;
						BNY *= .10;

						float ncx = ub.x * BNX + vb.x * BNY;
						float ncy = ub.y * BNX + vb.y * BNY;
						float ncz = ub.z * BNX + vb.z * BNY;

						normal.x = n.x + ncx;
						normal.y = n.y + ncy;
						normal.z = n.z + ncz;
						normal.Normalize();

						/*ii->ci.r = bmcX1.r;
						ii->ci.g = bmcX1.g;
						ii->ci.b = bmcX1.b;

						strength = 1.0 * ld.Dot(&normal);// / (lengthfromlight*lengthfromlight);
					}
					else*/
					
					for(int l = 0 ; l < numLights ; l++)
					{
						float tstrength;
						VECTOR ld;	// from light to hit
						ld.x = (hit.x) - Light[l].p.x;
						ld.y = (hit.y) - Light[l].p.y;
						ld.z = (hit.z) - Light[l].p.z;
						float lengthfromlight = ld.Abs();
						ld.x /= lengthfromlight;
						ld.y /= lengthfromlight;
						ld.z /= lengthfromlight; //ld normalized

						tstrength = Light[l].multiplier * ld.Dot(&n) / ((lengthfromlight)*(.5*Light[l].fade));


						//light is behind plane, cant be seen
						if(tstrength < 0.0) tstrength *= -1.0;
						//if(tstrength > 1.0) tstrength = 1.0;

						//calculate shadows only if level is high
						if(level > 70)
						{
							//Check for shadow
							LINE ray; //from light source to point of intersection.
							ray.p.x = Light[l].p.x;
							ray.p.y = Light[l].p.y;
							ray.p.z = Light[l].p.z;

							ray.d.x = hit.x - Light[l].p.x;
							ray.d.y = hit.y - Light[l].p.y;
							ray.d.z = hit.z - Light[l].p.z;
							ray.d.Normalize();
							INTERSECTINFO si, mi; //mi is this objects ii with light. need to know its distance from Light.
							IntersectInfo(&ray, &mi, 2);
							bool shadowed = false;
							for(int ww = 0 ; ww <= numPlanes ; ww++)
							{
								si.valid = 0;
								if(&o[ww] != this)
									o[ww].IntersectInfo(&ray, &si, 2);
								if(si.valid && si.t < mi.t)
								{
									shadowed = true;
									break;
								}
							}
							if(shadowed) // a shadowing object exsists
								tstrength = 0.0;
						}
						strength += tstrength;
					}

					if(strength < 0.15) strength = 0.15;

					ii->ci.r *= strength;
					ii->ci.g *= strength;
					ii->ci.b *= strength;

					/* fog?
					ii->ci.r += 1.5 * ii->t*ii->t;
					ii->ci.g += 1.5 * ii->t*ii->t;
					ii->ci.b += 1.5 * ii->t*ii->t;
					*/

					if(level > 90 && reflecting)
					{	// Reflection calculations
						LINE rray;
						VECTOR i;
						i.x = -hit.x + line->p.x;
						i.y = -hit.y + line->p.y;
						i.z = -hit.z + line->p.z;
						i.Normalize();
						float dp = n.Dot(&i);

						rray.d.x = 2.0 * dp * n.x - i.x;
						rray.d.y = 2.0 * dp * n.y - i.y;
						rray.d.z = 2.0 * dp * n.z - i.z;
						rray.d.Normalize();

						rray.p = hit;

						float mint = 100000.0;
						int usedww = -1;
						INTERSECTINFO ri; COLORINFO co;
						for(int ww = 0 ; ww <= numPlanes ; ww++)
						{
							ri.valid = 0;
							if(&o[ww] != this)
							{
								o[ww].IntersectInfo(&rray, &ri, 2);
								if(ri.valid && ri.t < mint)
								{
									usedww = ww;
									mint = ri.t;
									//co = ri.ci;
								}
							}
						}
						if(usedww >= 0)
						{
							o[usedww].IntersectInfo(&rray, &ri, 80);
							co = ri.ci;
						}


						float rf = 0.9, arf = 1.0 - rf;
/*						ii->ci.r *= 0.5;
						ii->ci.g *= 0.5;
						ii->ci.b *= 0.5;*/
						
						ii->ci.r += (rf * co.r) / pow(0.1*mint+1.0, 2.0);
						ii->ci.g += (rf * co.g) / pow(0.1*mint+1.0, 2.0);
						ii->ci.b += (rf * co.b) / pow(0.1*mint+1.0, 2.0);
					}

					if(ii->ci.r > 250) ii->ci.r = 250;
					if(ii->ci.g > 250) ii->ci.g = 250;
					if(ii->ci.b > 250) ii->ci.b = 250;
				}
			}
		}

	}
	//else
	{
		//ii->valid = 0;
		/*ii->t = 0.0;
		ii->ci.g = rand() % 128;
		ii->ci.b = rand() % 128;*/
	}
}







/*

OLD INTERSECT STRUCTTURE


/*	float bottom = 	-line->d.x - line->d.y - line->d.z
					+ ub.Dot(&line->d) * (ub.x + ub.y + ub.z)
					+ vb.Dot(&line->d) * (vb.x + vb.y + vb.z);

	if(bottom != 0.0)
	{
		VECTOR lp;	// Change line position coord so plane "becomes" in origin
		lp.x = line->p.x - p.x;
		lp.y = line->p.y - p.y;
		lp.z = line->p.z - p.z;

		ii->t =   lp.x + lp.y + lp.z
				- ub.Dot(&lp) * (ub.x + ub.y + ub.z)
				- vb.Dot(&lp) * (vb.x + vb.y + vb.z);

		ii->t /= bottom;

		VECTOR p;
		p.x = lp.x + ii->t * line->d.x;
		p.y = lp.y + ii->t * line->d.y;
		p.z = lp.z + ii->t * line->d.z;

		ii->u = ub.Dot(&p);
		ii->v = vb.Dot(&p);

			if(ii->u >= 0 && ii->v >= 0)
			{
				if(ii->v <= v2 && ii->u <= u2)
				{
					ii->valid = true;
					unsigned char* cc = (unsigned char*)&col;
					ii->ci.r = cc[0];
					ii->ci.g = cc[1];
					ii->ci.b = cc[2];
				}
			}

	}
	else
		ii->t = -11;

	return ;

*/

