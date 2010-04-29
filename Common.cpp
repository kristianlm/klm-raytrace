
#include ".\Common.h"
#include ".\Common.h"

#include <stdlib.h>

Common::Common(void)
{
}

Common::~Common(void)
{
}
float rnd()
{
	float r = rand();
	r /= (float)RAND_MAX;
	return (r - 0.5);
}