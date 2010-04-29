

#pragma once

#include "vector.h"


class CAMERA
{
public:
    VECTOR p, d;
    VECTOR cu, cv;

    void CalcUV();

};

extern CAMERA Camera;


void RayTrace(float x, float y, COLORINFO* ci, bool text = false);

