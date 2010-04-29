#pragma once

#include "vector.h"

class LightSource
{
public:
	LightSource(void);
	~LightSource(void);

	VECTOR p;
	COLORINFO color;
	float multiplier, fade;
};

extern LightSource Light[10];
extern int numLights;
