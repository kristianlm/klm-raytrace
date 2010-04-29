#pragma once

#include <windows.h>
#include <vfw.h>

class MYAVI
{
public:
	MYAVI();
	
	int mx, my;
	
	int f;
	HBITMAP bmp;
	DIBSECTION dibs;
	
	IAVIFile* avifile;
	IAVIStream* avistream, *avicompressed;
	
	char* bits;
	
	void Open(char* filename, int sx, int sy);
	void AddFrame();
	void Close();
	void Clear(int col);
	
	void SetPixel(int x, int y, int col);
	
};

