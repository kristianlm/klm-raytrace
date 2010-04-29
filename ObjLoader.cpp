

#include <stdio.h>

#include ".\objloader.h"

#include "vector.h"
#include "Common.h"

ObjLoader::ObjLoader(void)
{
}

ObjLoader::~ObjLoader(void)
{
}

bool ReadLine(HANDLE file, char* buffer, int max)
{
	char c = 0;
	DWORD read = 0;
	int pos = 0;
	while(true)
	{
		if(!ReadFile(file, &c, 1, &read, NULL))
			return 0;

		if(c == 10 || c == 13)
		{
			buffer[pos] = 0;
			return true;		
		}

        buffer[pos] = c;


		pos++;
		if(pos >= max) return 0;
	}
}
int FirstOf(char find, char* buff, int from)
{
	int i = from;
	while(true)
	{
		if(buff[i] == find) return i+1;
		if(buff[i] == 0) return -1;
		i++;
	}
}

struct _vtemp 
{
	float x, y, z;
};
_vtemp vtemp[1000];

void ObjLoader::LoadFromFile()
{
	/*char b[] = "v 3.14 -8.13";
	int os = 3;
	float a = atof(&b[os]);

	return ;*/
	
	HANDLE file = CreateFile("k.obj", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if(file == INVALID_HANDLE_VALUE)
	{
		MessageBox(0, "file err", 0, 0);
		return;
	}

/* exmaple text
v 1.0395 -1.6632 -1.24741
vn 0 0 -1
vt 0 0
f 3/3/13 7/7/25 5/5/19 1/1/7
*/
	char TextureFile[128];
	int vindex = 1;
	int w = 0;
	char buffer[256];

	int Red = 0xFF, Green = 0xff, Blue = 0xFF;
	bool Reflecting = 0, Texturing = false;
	while(true)
	{
		if(!ReadLine(file, buffer, 256))
			break;


		if(buffer[0] == 'g' && buffer[1] == ' ')
		{
			int pos = FirstOf('@', buffer, 2);
			if(pos > 0)
			{
				if(FirstOf('R', buffer, pos) > 0)
					Reflecting = true;
				else
					Reflecting = false;

				if(FirstOf('T', buffer, pos) > 0)
				{
					Texturing = true;
					int first = FirstOf('"', buffer, pos);
					int last = FirstOf('"', buffer, first);
					memcpy(TextureFile, &buffer[first], last-first);
					TextureFile[last-first-1]=0;

					//MessageBox(0, TextureFile, 0, 0);
					
				}
				else
					Texturing = false;
				
				int ppos = FirstOf('r', buffer, pos);
				if(ppos > 0) // red
					Red = atoi(&buffer[ppos]);
				
				ppos = FirstOf('g', buffer, pos);
				if(ppos > 0) // red
					Green = atoi(&buffer[ppos]);

				ppos = FirstOf('b', buffer, pos);
				if(ppos > 0) // red
					Blue = atoi(&buffer[ppos]);

				char* tn = &buffer[pos];
			}
			else
			{
				Red = Green = Blue = 0xa0;
			}
		}
		if(buffer[0] == 'v' && buffer[1] == ' ')
		{
			int os = 2;
			float x = atof(&buffer[os]); 

			os = FirstOf(' ', buffer, os); if(os < 0) MessageBox(0,"parse err", 0, 0);
			float y = atof(&buffer[os]);

			os = FirstOf(' ', buffer, os); if(os < 0) MessageBox(0,"parse err", 0, 0);
			float z = atof(&buffer[os]);

			vtemp[vindex].x = x * 0.3;
			vtemp[vindex].y = y * 0.3;
			vtemp[vindex].z = z * 0.3;
			vindex++;
		}
		if(buffer[0] == 'f' && buffer[1] == ' ')
		{
			// "f 3/3/13 7/7/25 5/5/19 1/1/7"
			int os = 2;
			int vi1 = atoi(&buffer[os]); 

			os = FirstOf(' ', buffer, os); if(os < 0) MessageBox(0,"parse err", 0, 0);
			int vi2 = atof(&buffer[os]);

			os = FirstOf(' ', buffer, os); if(os < 0) MessageBox(0,"parse err", 0, 0);
			int vi3 = atof(&buffer[os]);

			os = FirstOf(' ', buffer, os); if(os < 0) MessageBox(0,"parse err", 0, 0);
			int vi4 = atof(&buffer[os]);

			o[w].reflecting = Reflecting;
			o[w].p.x = vtemp[vi1].x;
			o[w].p.y = vtemp[vi1].y;
			o[w].p.z = vtemp[vi1].z;

			o[w].ub.x = vtemp[vi4].x - vtemp[vi1].x;
			o[w].ub.y = vtemp[vi4].y - vtemp[vi1].y;
			o[w].ub.z = vtemp[vi4].z - vtemp[vi1].z;
			o[w].u2 = o[w].ub.Abs();
			o[w].ub.Normalize();

			o[w].vb.x = vtemp[vi2].x - vtemp[vi1].x;
			o[w].vb.y = vtemp[vi2].y - vtemp[vi1].y;
			o[w].vb.z = vtemp[vi2].z - vtemp[vi1].z;
			o[w].v2 = o[w].vb.Abs();
			o[w].vb.Normalize();

			o[w].CalcN();

			if(Texturing)
			{
				o[w].tex.LoadBmp(TextureFile);
			}
			else
			{
				o[w].tex.Open(1, 1);
				o[w].tex.SetPixel(0, 0, RGB(Red, Green, Blue));
			}


			/*o[w].u2 = 8.0;
			o[w].v2 = 8.0;*/

			w++;

			char txt[200];
			sprintf(txt, "\"%s\"\r\n%f, %f, %f", buffer, vtemp[vi1].x, vtemp[vi1].y, vtemp[vi1].z);
 			//MessageBox(0, txt, 0, 0);
		}
	}

	numPlanes = w - 1;

	CloseHandle(file);
}
void ObjLoader::LoadDemo()
{
	int w = 0;
	// ********************** floor
	o[w].reflecting = 1;
	o[w].p.x = -4.0;
	o[w].p.y = -1.8;
	o[w].p.z =  4.0;

	o[w].ub.x =  1.0;
	o[w].ub.y =  0.0;
	o[w].ub.z =  0.0;
	o[w].ub.Normalize();

	o[w].vb.x =  0.0;
	o[w].vb.y =  0.0;
	o[w].vb.z = -1.0;
	o[w].vb.Normalize();

	o[w].CalcN();

	o[w].u2 = 8.0;
	o[w].v2 = 8.0;

	o[w].col = 0xFF;

	// ********************* front
	w++;

	o[w].p.x = -0.5;
	o[w].p.y = -1.5;
	o[w].p.z =  0.5;

	o[w].ub.x = 1.0;
	o[w].ub.y = 0.0;
	o[w].ub.z = 0.0;
	o[w].ub.Normalize();

	o[w].vb.x = 0.0;
	o[w].vb.y = 1.0;
	o[w].vb.z = 0.0;
	o[w].vb.Normalize();

	o[w].CalcN();

	o[w].u2 = 1.0;
	o[w].v2 = 1.0;

	o[w].col = 0xFF;
	// ********************* back
	w++;

	o[w].p.x =  0.5;
	o[w].p.y = -1.5;
	o[w].p.z = -0.5;

	o[w].ub.x = -1.0;
	o[w].ub.y = 0.0;
	o[w].ub.z = 0.0;
	o[w].ub.Normalize();

	o[w].vb.x = 0.0;
	o[w].vb.y = 1.0;
	o[w].vb.z = 0.0;
	o[w].vb.Normalize();

	o[w].CalcN();

	o[w].u2 = 1.0;
	o[w].v2 = 1.0;

	o[w].col = 0xFF;

	// ********************* top
	w++;
	o[w].p.x = -0.5;
	o[w].p.y = -0.5;
	o[w].p.z =  0.5;

	o[w].ub.x = 1.0;
	o[w].ub.y = 0.0;
	o[w].ub.z = 0.0;
	o[w].ub.Normalize();

	o[w].vb.x = 0.0;
	o[w].vb.y = 0.0;
	o[w].vb.z =-1.0;
	o[w].vb.Normalize();

	o[w].u2 = 1.0;
	o[w].v2 = 1.0;

	o[w].col = 0xFF0000;
	o[w].CalcN();

	// ************************ left
	w++;
	o[w].p.x = -0.5;
	o[w].p.y = -1.5;
	o[w].p.z =  0.5;

	o[w].ub.x =  0.0;
	o[w].ub.y =  0.0;
	o[w].ub.z = -1.0;
	o[w].ub.Normalize();

	o[w].vb.x = -0.0;
	o[w].vb.y =  1.0;
	o[w].vb.z =  0.0;
	o[w].vb.Normalize();

	o[w].u2 = 1.0;
	o[w].v2 = 1.0;

	o[w].col = 0xFF00;
	o[w].CalcN();

	// ************************ right
	w++;
	o[w].p.x =  0.5;
	o[w].p.y = -1.5;
	o[w].p.z = -0.5;

	o[w].ub.x =  0.0;
	o[w].ub.y =  0.0;
	o[w].ub.z =  1.0;
	o[w].ub.Normalize();

	o[w].vb.x =  0.0;
	o[w].vb.y =  1.0;
	o[w].vb.z =  0.0;
	o[w].vb.Normalize();

	o[w].u2 = 1.0;
	o[w].v2 = 1.0;

	o[w].col = 0xFF00;
	o[w].CalcN();

	numPlanes = w;
	for(w = 0 ; 1 && w <= numPlanes ; w++)
	{
	    o[w].CalcN();

        int r = rand();
	    o[w].tex.Open(1, 1);
	    for(int x = 0 ; x < o[w].tex.mx ; x++)
	    for(int y = 0 ; y < o[w].tex.my ; y++)
	    {
	    	if(w == 2)
				o[w].tex.SetPixel(x, y, RGB(255,0,0));
			/*else if(w == 2)
				o[w].tex.SetPixel(x, y, RGB(0,255,0));
			else*/
			/*{
				float x2 = x - o[w].tex.mx / 2.0;
				float y2 = y - o[w].tex.my / 2.0;
				float d = sqrt(x2*x2+y2*y2) / 2.0;
				o[w].tex.SetPixel(x, y, RGB(r, 124+sin(d)*120, 124+sin(d)*120));
			}*/
			else
			o[w].tex.SetPixel(x, y, RGB(rand(), rand(), rand()));
	    }

	    for(int y = 0 ; y < o[w].tex.my ; y++)
	    {
           // o[w].tex.SetPixel(y, y, RGB(255,255,0));
	    }
    	/*if(rand() % 2 == 0)
			o[w].tex.LoadBmp("brick.bmp");
		else*/
		if(w == 0)
			o[w].tex.LoadBmp("tst.bmp");
		else
			o[w].tex.LoadBmp("brick.bmp");


	}

	for(w = 0 ; 0 && w <= numPlanes ; w++)
	{
		o[w].p.x = (rnd() ) * 2.0;
		o[w].p.y = (rnd() ) * 2.0;
		o[w].p.z = (rnd() ) * 2.0;

		o[w].ub.x = rnd() * 5.0;
		o[w].ub.y = rnd() * 5.0;
		o[w].ub.z = rnd() * 5.0;
		o[w].ub.Normalize();

		o[w].vb.x = rnd() * 5.0;
		o[w].vb.y = rnd() * 5.0;
		o[w].vb.z = rnd() * 5.0;
		o[w].vb.Normalize();

		o[w].u2 = 1.0;
		o[w].v2 = 1.0;

		o[w].col = RGB(rand(),rand(),255);

		o[w].v1 = o[w].u1 = 0;

		float proj = o[w].vb.Dot(&o[w].ub);

		o[w].vb.x = o[w].vb.x - proj * o[w].ub.x;
		o[w].vb.y = o[w].vb.y - proj * o[w].ub.y;
		o[w].vb.z = o[w].vb.z - proj * o[w].ub.z;
		o[w].vb.Normalize();

		o[w].CalcN();
	}
}