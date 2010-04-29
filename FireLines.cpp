// Project1
//

#include <windows.h>
#include <stdio.h>
#include <math.h>

#include "Project1_res.h"

#include "linesvector.h"



LINE l[1000];
int mstate = 0, sel = 0;

LRESULT WINAPI MainWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

HINSTANCE hInstance;
HWND hWindow;

HDC hdc;

char txt[1024];

// WinMain
struct _RGB32
{
	unsigned char b, g, r, a;
};


_RGB32 vid[MX * MY];

void Plot(HDC hdc, int x, int y, int col)
{
	SetPixelV(hdc, x/SCALE, y/SCALE, col);
	char* c = (char*)&col;
	
	if(x >= 0 && y >= 0)
		if(x < MX && y < MY)
		{
			_RGB32* pix = &vid[x + (MX*y)];
			pix->r = c[0];	
			pix->g = c[1];	
			pix->b = c[2];	
			pix->a = 0;	
		}	
}
void StoreBitmap()
{
	char filename[80];
	
	WIN32_FIND_DATA fd;
	for(int i = 0 ; i < 1000 ; i++)
	{
		sprintf(filename, "lines\\z%04d.bmp", i);
		if(FindFirstFile(filename, &fd) == INVALID_HANDLE_VALUE) 
			break;
	}

	DWORD written = 0;

	
	BITMAPFILEHEADER bfh;	ZeroMemory(&bfh, sizeof(bfh));
	BITMAPINFOHEADER bih;	ZeroMemory(&bih, sizeof(bih));

	//prep file header	
	char* cc = (char*)&bfh.bfType;cc[0] = 'B'; cc[1] = 'M';
	bfh.bfSize = MX * MY * 4 + sizeof(bfh) + sizeof(bih);
	bfh.bfOffBits = sizeof(bfh) + sizeof(bih);

	//prep info header
	bih.biSize = sizeof(bih);
	bih.biWidth = MX;
	bih.biHeight = MY;
	bih.biPlanes = 1;
	bih.biBitCount = 32;
	bih.biCompression = BI_RGB;
	bih.biSizeImage = MX * MY * 4;
	bih.biXPelsPerMeter = 1000;
	bih.biYPelsPerMeter = 1000;
	
	HANDLE file = CreateFile(filename, GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, NULL, NULL);
	//write it out
	WriteFile(file, &bfh, sizeof(bfh), &written, NULL);
	WriteFile(file, &bih, sizeof(bih), &written, NULL);
	
	for(int y = MY-1 ; y >= 0 ; y--)
	{
		WriteFile(file, &vid[0 + (MX * y)], MX*4, &written, NULL);
	}
	CloseHandle(file);
	
	sprintf(txt, "%s written", filename);
	TextOut(hdc, 0, MY+18,txt, strlen(txt));
}


void DrawLines()
{

	for(int i = 0 ; i <= sel ; i++)	
	{
   	l[i].Draw(hdc, 0xFF);		   
	}
	
}

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
   WNDCLASS wc;
   MSG msg;
   //HACCEL hAccelTable;

   if (!hPrevInst)
   {
      wc.lpszClassName="Project1Class";
      wc.lpfnWndProc=MainWndProc;
      wc.style=CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
      wc.hInstance=hInst;
      wc.hIcon=LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON1));
      wc.hCursor=LoadCursor(NULL, IDC_ARROW);
      wc.hbrBackground=(HBRUSH)(COLOR_WINDOW+2);
      wc.lpszMenuName=MAKEINTRESOURCE(IDR_MAINMENU);
      wc.cbClsExtra=0;
      wc.cbWndExtra=0;
      RegisterClass(&wc);
   }

   hWindow=CreateWindow("Project1Class","Application",WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,
									800,600,NULL,NULL,hInst,NULL);
   hInstance=hInst;

   ShowWindow(hWindow,nCmdShow);
   UpdateWindow(hWindow);

   //hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_ACCELATOR1);
	hdc = GetDC(hWindow);
	
	SetBkColor(hdc, 0);
	SetTextColor(hdc, 0x00FF00);


	for(int i = 0 ; i < 100 ; i++)	
	{
		int x1 = 10 + rand() % (MX-20);
		int x2 = 10 + rand() % (MX-20);
		int y1 = 10 + rand() % (MY-20);
		int y2 = 10 + rand() % (MY-20);
	
		l[i].SetPoint(x1, y1);
		
		VECTOR d;
		d.x = x2 - x1;
		d.y = y2 - y1;
		float dist = d.Abs();
		d.Normalize();
		
		l[i].SetDirection(d.x, d.y);	
		l[i].SetLength(dist);
   	l[i].Draw(hdc, 0xFF0000);		   
	}
	
	
		
   while(true)
	{
		if(PeekMessage(&msg,NULL,0,0, PM_REMOVE))
   	{
   		if(msg.message == WM_QUIT) break;
	      TranslateMessage(&msg);
	      DispatchMessage(&msg);
	   }
	   
	   POINT p; GetCursorPos(&p); ScreenToClient(hWindow, &p);
	   //SetPixelV(hdc, p.x, p.y, 0);
	   
	   //LINE& ll = l[2];
	   
	   //float dist = ll.Distance(p.x, p.y);
	   //ll.Draw(hdc, 0x0);
	  	
		static int lastsel = 1;   
	  	if(sel != lastsel)
	  	{
	  		lastsel = sel;
	   	sprintf(txt, "Selection %d", sel);
	   	TextOut(hdc, 0, 480, txt, strlen(txt));
	   }
	   
	   
		if(GetAsyncKeyState(VK_CONTROL))
		{
			int step = 1;
			for(int k = 0 ; k <= step; k++) {
			for(int y = k ; y < MY ; y += step+1)	
			{
				for(int x = 0 ; x < MX ; x++)	
				{
					float rr = 0, gg = 0, bb = 0, count = 0;
					for(float yy = -0.75 ; yy <= 0.75 ; yy += 2.5)
					for(float xx = -0.75 ; xx <= 0.75 ; xx += 2.5)
					{
						COLORINFO ci;
						
					   for(int i = 0 ; i <= sel ; i++)
					   {
							l[i].ColorInfo(x+xx, y+yy, &ci);
					   }
						rr += ci.r;
						gg += ci.g;
						bb += ci.b;
						count++;
					}
					rr /= count;
					gg /= count;
					bb /= count;
					if(rr > 255) rr = 255;
					if(gg > 255) gg = 255;
					if(bb > 255) bb = 255;
					
				   int c = RGB(rr, gg, bb);
				   Plot(hdc, x, y, c);
				   if(k == 0)
				   	for(int kk = 1 ; kk <= step ;kk++)
					   	Plot(hdc, x, y+kk, c);
				}
				if(GetAsyncKeyState(VK_ESCAPE)) {y = 480; k = step+1;}
			   //sprintf(txt, "%d", y);
			   //TextOut(hdc, 0, 0, txt, strlen(txt));
			}}
			
			StoreBitmap();
		}//if vk_escape
	   
	}
	
	
	
	
	

	ReleaseDC(hWindow, hdc);
	
   return msg.wParam;
}

// MainWndProc

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam,LPARAM lParam)
{
   static int wmId, wmEvent;
   POINT p;
   GetCursorPos(&p);
   ScreenToClient(hWnd, &p);
   p.x *= SCALE;
   p.y *= SCALE;

   switch(msg)
   {
      // WM_COMMAND

      case WM_COMMAND:
         wmId    = LOWORD(wParam);
         wmEvent = HIWORD(wParam);
         switch(wmId)
         {
            // About

            case ID_FILE_EXIT:
               PostQuitMessage(0);
               break;

            case ID_HELP_ABOUT:
               DialogBox(hInstance, (LPSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)AboutDlgProc);
               break;

            default:
               return DefWindowProc(hWnd, msg, wParam, lParam);
         }
         break;

      // WM_DESTROY
      case WM_MOUSEMOVE:
      	if(mstate == 1)
      	{
				l[sel].Draw(hdc, 0);	
	      	l[sel].p.x = p.x;
	      	l[sel].p.y = p.y;
	      	l[sel].Draw(hdc, 0xFF);
      	}
      	else if(mstate == 2)
      	{
	      	VECTOR d;
				l[sel].Draw(hdc, 0);	
	      	d.x = p.x - l[sel].p.x;
	      	d.y = p.y - l[sel].p.y;
	      	float len; len = d.Abs();
	      	d.Normalize();
	      	l[sel].d.x = d.x;
	      	l[sel].d.y = d.y;      
				l[sel].length = len;
				l[sel].Draw(hdc, 0xFF);	
      	}
      	break;
      case WM_LBUTTONDOWN:
      	mstate = 1;
      	break;
      	
      case WM_RBUTTONDOWN:
      	mstate = 2;
      	break;
      	
      case WM_LBUTTONUP:
      case WM_RBUTTONUP:
      	DrawLines();
      	mstate = 0;
      	break;
      	
      case WM_KEYDOWN:
      	if(wParam == VK_RIGHT)
      		sel++;
      	if(wParam == VK_LEFT)
      		sel--;
      	break;
//      case WM_PAINT:
  //    	break;

      case WM_DESTROY:
         PostQuitMessage(0);
         break;

		// DEFAULT

		default:
         return(DefWindowProc(hWnd, msg, wParam, lParam));
   }
   return 0;
}

// AboutDlgProc

LRESULT CALLBACK AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
   switch (message)
   {
      case WM_INITDIALOG:
         break;

      case WM_COMMAND:
         if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
         {
            EndDialog(hDlg, LOWORD(wParam));
            return TRUE;
         }
         break;
   }
   return FALSE;
}

