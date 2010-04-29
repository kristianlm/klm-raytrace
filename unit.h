

#pragma once

#include <windows.h>

#define UNIT_TYPE_WORKER	0

class UNIT
{
public:
	bool valid;
	unsigned char type;

	int fromnode, tonode;
	float process;

	void Draw(MYIMAGE* im);
	void Move();
};
