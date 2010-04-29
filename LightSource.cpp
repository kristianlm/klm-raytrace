#include ".\lightsource.h"

LightSource Light[10];

int numLights = 3;

LightSource::LightSource(void)
{
	multiplier = 1.0;
	fade = 1.0;
	p.x = p.y = p.z = 0;
}

LightSource::~LightSource(void)
{
}

