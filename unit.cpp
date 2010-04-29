
#include <windows.h>

#include "draw.h"
#include "unit.h"
#include "pathnode.h"
#include "vectorS.h"


void UNIT::Draw(MYIMAGE* im)
{
	VECTOR d;
	d.x = pn[tonode].x - pn[fromnode].x;
	d.y = pn[tonode].y - pn[fromnode].y;
	d.Normalize();
	float xx = pn[fromnode].x + d.x * process;
	float yy = pn[fromnode].x + d.y * process;

	DrawCircle(im, xx, yy, 5, 0xFFFFFF);
}

void UNIT::Move()
{
	process += 1.0;
}
