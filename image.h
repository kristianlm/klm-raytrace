
#pragma once

#include "intersectinfo.h"

class MYIMAGE
{
public:
	MYIMAGE();

	int mx, my;

	HDC hdcB;
	BITMAPINFO bi;
	HBITMAP bmp;
	DIBSECTION dibs;

	unsigned long* bits;

	void Open(int sx, int sy);
	void Close();
	void Clear(int col);

	void SetPixel(int x, int y, int col);
	void GetPixel(int x, int y, COLORINFO* col);
	int  GetPixel(int x, int y);
	void LoadBmp(char* filename);
	void SaveInSequence(char* path);

};
