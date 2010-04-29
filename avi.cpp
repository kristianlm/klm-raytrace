// avi.cpp : Defines the entry point for the application.
//


#include <windows.h>
#include <Vfw.h>
#include <stdio.h>

#include "avi.h"


extern HDC hdc;
extern HWND hWindow;

MYAVI::MYAVI()
{
	f = 0;
}

void MYAVI::Open(char* filename, int sx, int sy)
{
	mx = sx;
	my = sy;

	static bool _not_init = 1;
	if(_not_init)
	{
		AVIFileInit();
		_not_init = 0;
	}

	HRESULT hr = AVIFileOpen(&avifile, "C:\\temp\\3d.avi", OF_CREATE | OF_WRITE, NULL);
	if(hr != AVIERR_OK)
	{
		MessageBox(0, "err opening", 0, 0);
		AVIFileExit();
	}

	HDC hdcS = GetDC(0);
	HDC hdcB = CreateCompatibleDC(hdcS);
	ReleaseDC(0, hdcS);

	BITMAPINFO bi;
	ZeroMemory(&bi, sizeof(bi));
	BITMAPINFOHEADER &bih = bi.bmiHeader;


	bih.biBitCount = 24;
	bih.biCompression = BI_RGB;
	bih.biClrUsed = 0;
	bih.biSize = sizeof(bih);
	bih.biPlanes = 1;

	bih.biWidth = sx;
	bih.biHeight = sy;
	bih.biSizeImage = sx * sy * 3;

	bih.biYPelsPerMeter = bih.biXPelsPerMeter = 10000;

	bmp = CreateDIBSection(hdcB, &bi, DIB_RGB_COLORS, (void**)&bits, NULL, NULL);


	GetObject(bmp, sizeof(dibs), &dibs);

	SelectObject(hdcB, bmp);

	AVISTREAMINFO si;
	ZeroMemory(&si, sizeof(si));

	si.fccType = streamtypeVIDEO;
	si.fccHandler = 0;
	si.dwRate = 1000;
	si.dwScale = 40;
	si.dwSuggestedBufferSize = sx* sy * 3;
	SetRect(&si.rcFrame, 0, 0,sx, sy);

	hr = AVIFileCreateStream(avifile, &avistream, &si);
    if (hr!=AVIERR_OK)
	{
		MessageBox(0, "stream", 0, 0);
		AVIFileExit();
	}


	AVICOMPRESSOPTIONS opts;
	ZeroMemory(&opts,sizeof(opts));

    opts.fccHandler=mmioFOURCC('d','i','v','x');
	opts.dwFlags = AVICOMPRESSF_KEYFRAMES | AVICOMPRESSF_DATARATE;
	opts.dwKeyFrameEvery = 50;
	opts.dwBytesPerSecond = 1 * 1024;


	AVICOMPRESSOPTIONS* aopts = &opts;
	BOOL res = (BOOL)AVISaveOptions(hWindow, 0, 1, &avistream, &aopts);

	char* c = (char*)&opts.fccHandler;



    hr = AVIMakeCompressedStream(&avicompressed, avistream, aopts, NULL);
    if (hr != AVIERR_OK) {
		MessageBox(0, "make compression fail", 0, 0);
		AVIFileExit();
	}
    hr = AVIStreamSetFormat(avicompressed, 0, &dibs.dsBmih, dibs.dsBmih.biSize+dibs.dsBmih.biClrUsed*sizeof(RGBQUAD));
    if (hr!=AVIERR_OK) {
		MessageBox(0, "set format fail", 0, 0);
		AVIFileExit();
	}
	/*for(int f = 0 ; f < 250 ; f++)
	{
		//for(int x = 0 ; x < 320 ; x++)
		//	for(int y = 0 ; y < 220 ; y++)
		//		SetPixelV(hdc, x, y, bits[x*3 + (y * 320*3)]);
		sprintf(txt, "%d/%d", f, 250);
		TextOut(hdc, 0, 0, txt, strlen(txt));


		//::SetDIBits(
		hr = AVIStreamWrite(avicompressed, f, 1, dibs.dsBm.bmBits, dibs.dsBmih.biSizeImage, AVIIF_KEYFRAME, NULL, NULL);
		if(hr != AVIERR_OK)
		{
			MessageBox(0, "fail stream write", 0, 0);
		}
//			else
//				MessageBox(0, "written", 0, 0);
	}*/

}

void MYAVI::AddFrame()
{
	HRESULT hr = AVIStreamWrite(avicompressed, f++, 1, dibs.dsBm.bmBits, dibs.dsBmih.biSizeImage, NULL/*AVIIF_KEYFRAME*/, NULL, NULL);

	if(hr != AVIERR_OK)
	{
		MessageBox(0, "fail stream write", 0, 0);
	}

	char txt[100];
	sprintf(txt, "AVI %d", f);
	TextOut(hdc, 0, 500, txt, strlen(txt));

}
void MYAVI::Close()
{
	DeleteObject(bmp);


	AVIStreamRelease(avicompressed);
	AVIStreamRelease(avistream);
	AVIFileClose(avifile);

}
void MYAVI::Clear(int col)
{
	ZeroMemory(bits, mx * my * 3);
	return;
	for(int r = 0 ; r < 1000 ; r++)
	{
		POINT p;
		p.x = rand() % mx;
		p.y = rand() % my;

		if(p.x < mx && p.x > 0)
			if(p.y < my && p.y > 0)
			{
				bits[3*(p.x-1) + (320 * 3 * p.y)] = 0xFF;
				bits[3*(p.x) + (320 * 3 * p.y)] = 0xFF;
				bits[3*(p.x) + (320 * 3 * (p.y-1))] = 0xFF;
				bits[3*(p.x-1) + (320 * 3 * (p.y-1))] = 0xFF;
			}
	}
}
void MYAVI::SetPixel(int x, int y, int col)
{
	if(x >= 0 && x < mx && y >= 0 && y < my)
	{
		unsigned char* cc = (unsigned char*)&col;
		int off = 3*x + (mx * 3 * (my-y-1));

		bits[off++] = cc[2];
		bits[off++] = cc[1];
		bits[off  ] = cc[0];
	}
}
