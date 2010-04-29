
#include <windows.h>
#include <stdio.h>

#include "vector.h"
#include "image.h"

extern HDC hdc;

MYIMAGE::MYIMAGE()
{
    bits = 0;
    mx = my = 0;
    hdcB = 0;
    bmp = 0;

}

void MYIMAGE::Open(int sx, int sy)
{

	mx = sx;
	my = sy;

	HDC hdcS = GetDC(0);
	hdcB = CreateCompatibleDC(hdcS);
	ReleaseDC(0, hdcS);

	ZeroMemory(&bi, sizeof(bi));

	BITMAPINFOHEADER &bih = bi.bmiHeader;

	bih.biBitCount = 32;
	bih.biCompression = BI_RGB;
	bih.biClrUsed = 0;
	bih.biSize = sizeof(bih);
	bih.biPlanes = 1;

	bih.biWidth = sx;
	bih.biHeight = -sy;
	bih.biSizeImage = sx * sy * 4 ;

	bih.biYPelsPerMeter = bih.biXPelsPerMeter = 10000;

	bmp = CreateDIBSection(hdcB, &bi, DIB_RGB_COLORS, (void**)&bits, NULL, NULL);

	GetObject(bmp, sizeof(dibs), &dibs);

	SelectObject(hdcB, bmp);
}
void MYIMAGE::Close()
{
    /*HDC hdcB;
	BITMAPINFO bi;
	HBITMAP bmp;
	DIBSECTION dibs;*/

    if(bmp != 0)
        DeleteObject(bmp);
    bmp = 0;
    if(hdcB != 0)
       DeleteObject(hdcB);
    hdcB = 0;
}
void MYIMAGE::Clear(int col)
{
	ZeroMemory(bits, mx * my * sizeof(unsigned long*));
	/*
	for(int x = 0 ; x < mx ; x++)
        for(int y = 0 ; y < my ; y++)
            SetPixel(x, y, col);*/
}

void MYIMAGE::SetPixel(int x, int y, int col)
{
	if(x >= 0 && x < mx && y >= 0 && y < my)
	{
		unsigned char* b = (unsigned char*)&bits[x + (mx*y)];
		unsigned char* cc = (unsigned char*)&col;

		*b = cc[2]; b++;
		*b = cc[1]; b++;
		*b = cc[0];
	}
}
void MYIMAGE::GetPixel(int x, int y, COLORINFO* col)
{
	if(x >= 0 && x < mx && y >= 0 && y < my)
	{
		int off = x + (mx * y);
		unsigned char* cc = (unsigned char*)&bits[off];

		col->r = cc[2];
		col->g = cc[1];
		col->b = cc[0];
	}
}
int  MYIMAGE::GetPixel(int x, int y)
{
	if(x >= 0 && x < mx && y >= 0 && y < my)
	{
		int off = x + (mx * y);
		return bits[off];
	}
}

void MYIMAGE::LoadBmp(char* filename)
{
    Close();

	BITMAPFILEHEADER bfh;
	ZeroMemory(&bfh, sizeof(bfh));
	ZeroMemory(&bi, sizeof(bi));

	//prep info header
	/*bih.biSize = sizeof(bih);
	bih.biWidth = MX;
	bih.biHeight = MY;
	bih.biPlanes = 1;
	bih.biBitCount = 32;
	bih.biCompression = BI_RGB;
	bih.biSizeImage = MX * MY * 4;
	bih.biXPelsPerMeter = 1000;
	bih.biYPelsPerMeter = 1000;*/

	HANDLE file = CreateFile(filename, GENERIC_READ, NULL, NULL, OPEN_ALWAYS, NULL, NULL);
    DWORD written;
	ReadFile(file, &bfh, sizeof(bfh), &written, NULL);
	ReadFile(file, &bi, sizeof(bi), &written, NULL);

	BITMAPINFOHEADER &bih = bi.bmiHeader;
	//bih.biBitCount = 32;
	//bih.biCompression = BI_RGB;
	//bih.biClrUsed = 0;
	//bih.biSize = sizeof(bih);
	//bih.biPlanes = 1;

	mx = bih.biWidth;
	my = bih.biHeight;
	//bih.biSizeImage = mx * my * 4 ;
	Open(mx, my);


    unsigned char c[4];

	SetFilePointer(file, bfh.bfOffBits, NULL, FILE_BEGIN);


	for(int y = my-1 ; y >= 0 ; y--)
	for(int x = 0 ; x < mx ; x++)
	{
		ReadFile(file, &c, 3, &written, NULL);
		SetPixel(x, y, RGB(c[2], c[1], c[0]));
	}
	CloseHandle(file);

}



void MYIMAGE::SaveInSequence(char* path)
{
	char filename[80];

	WIN32_FIND_DATA fd;
	for(int i = 0 ; i < 1000 ; i++)
	{
		sprintf(filename, "%s\\z%04d.bmp", path, i);
		if(FindFirstFile(filename, &fd) == INVALID_HANDLE_VALUE)
			break;
	}

	CreateDirectory(path, NULL);

	DWORD written = 0;


	BITMAPFILEHEADER bfh;	ZeroMemory(&bfh, sizeof(bfh));
	BITMAPINFOHEADER bih;	ZeroMemory(&bih, sizeof(bih));

	//prep file header
	char* cc = (char*)&bfh.bfType;cc[0] = 'B'; cc[1] = 'M';
	bfh.bfSize = mx * my * 4 + sizeof(bfh) + sizeof(bih);
	bfh.bfOffBits = sizeof(bfh) + sizeof(bih);

	//prep info header
	bih.biSize = sizeof(bih);
	bih.biWidth = mx;
	bih.biHeight = my;
	bih.biPlanes = 1;
	bih.biBitCount = 24;
	bih.biCompression = BI_RGB;
	bih.biSizeImage = mx * my * 4;
	bih.biXPelsPerMeter = 1000;
	bih.biYPelsPerMeter = 1000;

	HANDLE file = CreateFile(filename, GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, NULL, NULL);
	//write it out
	WriteFile(file, &bfh, sizeof(bfh), &written, NULL);
	WriteFile(file, &bih, sizeof(bih), &written, NULL);

	for(int y = my-1 ; y >= 0 ; y--)
	{
		//convert to 24-bit
		for(int x = 0 ; x < mx ; x++)
			WriteFile(file, &bits[x + (mx * y)], 3, &written, NULL);
	}
	CloseHandle(file);

	char txt[100];
	sprintf(txt, "%s written", filename);
	TextOut(hdc, 0, 480+18,txt, strlen(txt));
}
