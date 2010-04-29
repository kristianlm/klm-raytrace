
#pragma once

#include <windows.h>
#include "image.h"


#define PATH_TYPE_MINE 			0
#define PATH_TYPE_FACTORY 		1
#define PATH_TYPE_DISPATCHER	2

extern float path_type_radius[];
extern int path_type_color[];

class PATHNODE
{
public:
	bool valid;
	unsigned char type;

	float x, y;

	int neighbour[10];

	void Draw(HDC hdc);
	void Draw(MYIMAGE* im);

};

void DrawGraph(HDC hdc);
void DrawGraph(MYIMAGE* im);


extern PATHNODE pn[1000];

