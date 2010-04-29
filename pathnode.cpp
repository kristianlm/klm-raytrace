
#include <windows.h>
#include <math.h>

#include "draw.h"
#include "pathnode.h"

PATHNODE pn[1000];


float path_type_radius[] = {
	10.0,
	25.0,
	10.0
	};
int path_type_color[] = {
	0x808080,
	0xF00000,
	0x000080
	};

void PATHNODE::Draw(HDC hdc)
{
	DrawCircle(hdc, x, y, path_type_radius[type], path_type_color[type]);
}

void PATHNODE::Draw(MYIMAGE* im)
{
	DrawCircle(im, x, y, path_type_radius[type], path_type_color[type]);
}

void DrawGraph(HDC hdc)
{
	for(int i = 0 ; i < 100 ; i++)
	{
		PATHNODE &pp = pn[i];

		if(pp.valid)
		{
			pp.Draw(hdc);
			for(int l = 0 ; l < 10 ; l++)
			{
				if(pp.neighbour[l] != 0)
				{
					DrawLine(hdc, pp.x, pp.y, pn[pp.neighbour[l]].x, pn[pp.neighbour[l]].y, 0x00FFFF);
				}
			}
		}
	}
}
void DrawGraph(MYIMAGE* im)
{
	for(int i = 0 ; i < 3 ; i++)
	{
		PATHNODE &pp = pn[i];

		if(pp.valid)
		{
			pp.Draw(im);
			for(int l = 0 ; l < 10 ; l++)
			{
				if(pp.neighbour[l] != 0)
				{
					DrawLine(im, pp.x, pp.y, pn[pp.neighbour[l]].x, pn[pp.neighbour[l]].y, 0x00FFFF);
				}
			}
		}
	}
}