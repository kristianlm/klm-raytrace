
#pragma once

#include <windows.h>

#include "image.h"

void DrawCircle(HDC hdc, float x, float y, float rad, int col);
void DrawLine(HDC hdc, float x1, float y1, float x2, float y2, int col);

void DrawCircle(MYIMAGE* im, float x, float y, float rad, int col);
void DrawLine(MYIMAGE* im, float x1, float y1, float x2, float y2, int col);
