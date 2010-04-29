
#pragma once


class COLORINFO
{
public:
	COLORINFO();
	float r, g, b;
};

class INTERSECTINFO
{
public:
	COLORINFO ci;
	float t;
	float u, v;
	bool valid;
};
