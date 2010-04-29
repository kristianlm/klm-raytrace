// Project1
//

#include <windows.h>
#include <stdio.h>
#include <math.h>

#include "Project1_res.h"

#include "avi.h"
#include "image.h"
#include "vector.h"

#include "camera.h"
#include "LightSource.h"

#include "ObjLoader.h"


bool WriteAvi = 1;
int aastep = 1;


int mstate = 0, sel = 0;

LRESULT WINAPI MainWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

HINSTANCE hInstance;
HWND hWindow;

HDC hdc;

MYIMAGE myimage;

char txt[1024];

// WinMain
struct _RGB32
{
	unsigned char b, g, r, a;
};



//_RGB32 vid[MX * MY];

/*void Plot(HDC hdc, int x, int y, int col)
{
	SetPixelV(hdc, x/SCALE, y/SCALE, col);
	char* c = (char*)&col;

/*	if(x >= 0 && y >= 0)
		if(x < MX && y < MY)
		{
			_RGB32* pix = &vid[x + (MX*y)];
			pix->r = c[0];
			pix->g = c[1];
			pix->b = c[2];
			pix->a = 0;
		}
}*/


void Draw()
{

	for(int i = 0 ; i <= numPlanes ; i++)
	{
		VECTOR v[4];
		v[0].x = o[i].p.x;
		v[0].y = o[i].p.y;
		v[0].z = o[i].p.z;

		v[1].x = o[i].p.x + o[i].ub.x;
		v[1].y = o[i].p.y + o[i].ub.y;
		v[1].z = o[i].p.z + o[i].ub.z;

		v[2].x = o[i].p.x + o[i].ub.x + o[i].vb.x;
		v[2].y = o[i].p.y + o[i].ub.y + o[i].vb.y;
		v[2].z = o[i].p.z + o[i].ub.z + o[i].vb.z;

		v[3].x = o[i].p.x + o[i].vb.x;
		v[3].y = o[i].p.y + o[i].vb.y;
		v[3].z = o[i].p.z + o[i].vb.z;

		float zs = 0.105;
		float x2 = 10.0 * v[3].x / ((v[3].z+1.0) * zs);
		float y2 = 10.0 * -v[3].y / ((v[3].z+1.0) * zs);
		for(int j = 0 ; j < 4 ; j++)
		{
			float x  = 10.0 * v[j].x / ((v[j].z+1.0) * zs);
			float y  = 10.0 * -v[j].y / ((v[j].z+1.0) * zs);
			//Line(x+MX/2, y+MY/2, x2+MX/2, y2+MY/2, 0xFFF0FF);
			x2 = x; y2 = y;
		}

	}

}


bool running = 1;
void DoEvents()
{
	MSG msg;
	while(PeekMessage(&msg,NULL,0,0, PM_REMOVE))
	{
		if(msg.message == WM_QUIT) running = 0;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
   WNDCLASS wc;
   MSG msg;
   msg.wParam = 0;
   //HACCEL hAccelTable;

   //ObjLoader::LoadFromFile();
   //return 0;

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

   hWindow=CreateWindow("Project1Class","Application",WS_OVERLAPPEDWINDOW,100,50,
									800,650,NULL,NULL,hInst,NULL);
   hInstance=hInst;

   ShowWindow(hWindow,nCmdShow);
   UpdateWindow(hWindow);

	hdc = GetDC(hWindow);

	MYAVI myavi;
	if(WriteAvi)
	{
		myavi.Open("hei.avi", MX, MY);
	}


   //hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_ACCELATOR1);

	SetBkColor(hdc, 0);
	SetTextColor(hdc, 0x00FF00);




//	ObjLoader::LoadDemo();
	ObjLoader::LoadFromFile();


	LINE l1, l2;
	VECTOR diff;
		l1.p.x = 150.0;
		l1.p.y = 100.0;

		l2.p.x = 150.0;
		l2.p.y = 300.0;


	Camera.p.x =  0.0;
	Camera.p.y =  0.0;
	Camera.p.z =  0.0;

	Camera.d.x =  0.0;
	Camera.d.y =  0.0;
	Camera.d.z = -1.0;
	Camera.d.Normalize();

	int l = 0;
	Light[l].p.x =  0.0;// + sin(a) * 1.0;//Camera.p.x / 4 + sin(a + (PI*0.5)) * 1.0;
	Light[l].p.y =  0.0;//-Camera.p.y + sin(theta) * 1.0;
	Light[l].p.z =  0.0;// + cos(a) * 1.0;

	bool PreviewRender = true;

    myimage.Open(MX, MY);
	float CSC = 0.0;

	long CurrFrame = 0;
	long RecLength = 0;
	CAMERA cams[1000];

	bool recording = false;
	bool playing = false;
	bool saving = false;
	while(running)
	{


		DoEvents();
		POINT p; GetCursorPos(&p); ScreenToClient(hWindow, &p);
	   	//SetPixelV(hdc, p.x, p.y, 0xFF);

/*
		for(float t = 0 ; t <= 1.0 ; t += 0.0005)
		{
			float x = l1.p.x + sin(PI*t) * (t * l1.d.x + (1-t) * l2.d.x) + t * diff.x;
			float y = l1.p.y + sin(PI*t) * (t * l1.d.y + (1-t) * l2.d.y) + t * diff.y;
			SetPixelV(hdc, x, y, 0);
		}


		if(GetAsyncKeyState(VK_LBUTTON))
		{
			l1.p.x = p.x;
			l1.p.y = p.y;
		}
		if(GetAsyncKeyState(VK_RBUTTON))
		{
			l2.d.x = (p.x - l1.p.x);
			l2.d.y = (p.y - l1.p.y);
			//l2.d.Normalize();
			/*float abs = l2.d.Abs();
			if(abs == 0) abs = 1.0;
			//l2.d.x *= diff.Abs();
			//l2.d.y *= diff.Abs();
		}

		//l1.d.x =  1.0 * diff.Abs();
		//l1.d.y =   0.0;

		//l2.d.x =  -0.0 * diff.Abs();
		//l2.d.y =   0.0;

		diff.x = l2.p.x - l1.p.x;
		diff.y = l2.p.y - l1.p.y;

		for(float t = 0 ; t <= 1.0 ; t += 0.0005)
		{
			float x = l1.p.x + sin(PI*t) * (t * l1.d.x + (1-t) * l2.d.x) + t * diff.x;
			float y = l1.p.y + sin(PI*t) * (t * l1.d.y + (1-t) * l2.d.y) + t * diff.y;
			SetPixelV(hdc, x, y, 0xFFFF);
		}
	   */


	   //LINE& ll = l[2];

	   //float dist = ll.Distance(p.x, p.y);
	   //ll.Draw(hdc, 0x0);


	   	static bool firsttime = 1;
		if(firsttime || GetAsyncKeyState(VK_SPACE))
		{
			__int64 count1, count2;
			QueryPerformanceCounter((LARGE_INTEGER*)&count1);
			//firsttime = false;


			static float a = PI;
			static float theta = 0.0;

			static int lmx = 0, lmy = 0;
			POINT ps; GetCursorPos(&ps);
		
			static bool lastLbutton = false;
			if(!playing && GetAsyncKeyState(VK_LBUTTON))
			{
				if(lastLbutton)
				{
					a += (500 - ps.x) * 0.005;
					theta += (500 - ps.y) * 0.005;
				}
				SetCursorPos(500, 500);
				lastLbutton = true;
			}
			else lastLbutton = false;


			static int lXkey = 0, lYkey=0;
			if(!saving && GetAsyncKeyState('X'))
			{
				if(lXkey)
					sel++;
				lXkey = false;
			}
			else
				lXkey = true;
			if(!saving && GetAsyncKeyState('Z'))
			{
				if(lYkey)
					sel--;
				lYkey = false;
			}
			else
				lYkey = true;



            /*lmx = p.x;
			lmy = p.y;*/


			/* ROTATING CAMERA AROUND ORIGIN (z axis)
            a += 0.01;

            Camera.p.x = sin(a) * 7.0;
            Camera.p.z = cos(a) * 7.0;
            Camera.p.y = 1.0;



            Camera.d.x = -Camera.p.x;
            Camera.d.y = -Camera.p.y - 2.0;
            Camera.d.z = -Camera.p.z;
            Camera.d.Normalize();
            Camera.CalcUV();
			*/



			if(!saving && GetAsyncKeyState('A'))
			{
				Camera.p.x += sin(a + PI/2) * CSC;
				Camera.p.z += cos(a + PI/2) * CSC;
			}
			if(!saving && GetAsyncKeyState('D'))
			{
				Camera.p.x -= sin(a + PI/2) * CSC;
				Camera.p.z -= cos(a + PI/2) * CSC;
			}
			if(!saving && GetAsyncKeyState('W'))
			{
				Camera.p.x += sin(a) * CSC;
				Camera.p.z += cos(a) * CSC;

				Camera.p.y += sin(theta) * CSC;
			}
			if(!saving && GetAsyncKeyState('S'))
			{
				Camera.p.x -= sin(a) * CSC;
				Camera.p.z -= cos(a) * CSC;

				Camera.p.y -= sin(theta)  * CSC;
			}
			if(!saving && GetAsyncKeyState(VK_SPACE))
			{
				Camera.p.y += 0.1f * CSC;
			}


			numLights = sel+1;
			if(!saving && GetAsyncKeyState('Q'))
			{
				Light[sel].multiplier = p.x * 0.02;
				Light[sel].fade = p.y * 0.001;
			}
			if(!saving && GetAsyncKeyState('E'))
			{
				Light[sel].p.x = Camera.p.x;
				Light[sel].p.y = Camera.p.y;
				Light[sel].p.z = Camera.p.z;
			}


			static int lastsel = 1;
			//if(sel != lastsel)
			{
				lastsel = sel;
				sprintf(txt, "Light %d mf(%.1f, %.1f)", sel, Light[sel].multiplier, Light[sel].fade);
				TextOut(hdc, 0, 480, txt, strlen(txt));
			}


			Camera.d.x = sin(a);
			Camera.d.z = cos(a);
			Camera.d.y = sin(theta);
			Camera.d.Normalize();
			Camera.CalcUV();

			sprintf(txt, "a(%.1f) th(%.1f)", a, theta);
			TextOut(hdc, 0, 480+5*18, txt, strlen(txt));

			if(!saving && GetAsyncKeyState('R'))
			{
				recording = true;
				CurrFrame = 0;
				PreviewRender = true;
			}
			if(!saving && GetAsyncKeyState('P'))
			{
				if(recording)
					RecLength = CurrFrame;

				recording = 0;
				playing = 1;
				CurrFrame = 0;
			}
			if(!saving && GetAsyncKeyState('L'))
			{
				playing = 1;
				saving = 1;
				CurrFrame = 0;
			}

			if(recording)
			{
				cams[CurrFrame] = Camera;
				CurrFrame++;
				if(CurrFrame > 999) recording = 0;
				sprintf(txt, "RECORDING %d", CurrFrame);
			}
			else if(playing)
			{
				Camera = cams[CurrFrame];
				//Camera.CalcUV();

				if(CurrFrame <= RecLength + 1)
					CurrFrame++;
				else
					saving = 0;
				if(CurrFrame > 999)  CurrFrame=0;

				sprintf(txt, "PLAYING %d/%d", CurrFrame, RecLength);
			}

			TextOut(hdc, 0, 480 + 18*6, txt, strlen(txt));

			if(!saving)
				PreviewRender = true;
			else
				PreviewRender = false;

			// Render settings
			if(GetAsyncKeyState('F'))
				PreviewRender = false;
			//else

			if(!PreviewRender)
			{
				SX = MX;
				SY = MY;
				LVL = 100;
				aastep = 3;
			}
			else
			{
				SX = 100;
				SY = 75;
				LVL = 50;
				aastep = 1;
			}

			int step = 0;
			float st = 1.0 / aastep;
			float hst = st * 0.5;
		    float rr = 0, gg = 0, bb = 0, count = 0;

			for(int k = 0 ; k <= step; k++)
			{
				for(float y = k ; y < SY ; y += step+1)
				{
					for(float x = 0 ; x < SX ; x++)
					{
                        count = 0; rr = gg = bb = 0;
						if(aastep != 1)
						{
                            for(int iy = 0 ; iy < aastep ; iy ++)
                            for(int ix = 0 ; ix < aastep ; ix ++)
                            {
                                COLORINFO ci;
                                float xx = hst + (st * ix);
                                float yy = hst + (st * iy);

                                RayTrace(x + xx, y + yy, &ci);


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
						}
						else
						{   //no aa
                            COLORINFO ci;
                            RayTrace(x, y , &ci);
                            rr = ci.r;
                            gg = ci.g;
                            bb = ci.b;
						}

					   int c = RGB(rr, gg, bb);

					   //Plot(hdc, x, y, c);

						myimage.SetPixel(x, y, c);
						if(WriteAvi && !recording)
							myavi.SetPixel(x, y, c);
						//if(myavi.f % 10 == 0)
						   //Plot(hdc, x, y, c);


					   //if(k == 0)
					   //	for(int kk = 1 ; kk <= step ;kk++)
						//   	Plot(hdc, x, y+kk, c);
					}

					//if(GetAsyncKeyState(VK_ESCAPE)) {y = 480; k = step+1;}

					SetPixelV(hdc, 0, y+20, 0xFF);
				

                    if(!PreviewRender && ((int)y % 20 == 0 || y >= SY-1))
                       BitBlt(hdc, 0, y-20, SX, 20, myimage.hdcB, 0, y-20, SRCCOPY);

                    if((int)y % 4 == 0)
                        DoEvents();
				}
			}
			if(PreviewRender)
				StretchBlt(hdc, 0, 0, SX*3, SY*3, myimage.hdcB, 0, 0, SX, SY, SRCCOPY);

			QueryPerformanceCounter((LARGE_INTEGER*)&count2);
			__int64 freq;
			QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
			double elap = (count2 - count1) / (double)freq;
			CSC = 5.0 * elap;
			sprintf(txt, "render time: %.3f (FPS %.2f)", elap, 1.0 / elap);
			TextOut(hdc, 0, 480+ 18*3, txt, strlen(txt));

			if(WriteAvi && saving && CurrFrame < RecLength)
				myavi.AddFrame();
		

			if(!PreviewRender && !saving)
	  						  myimage.SaveInSequence("C:\\temp\\Render");
		}//if vk_escape

	}

    myimage.Close();
	if(WriteAvi)
	{
		myavi.Close();
	}



	for(int w = 0 ; 1 && w <= numPlanes ; w++)
	{
	    o[w].tex.Close();
	}



	ReleaseDC(hWindow, hdc);

   return msg.wParam;
}

// MainWndProc

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam,LPARAM lParam)
{
   COLORINFO ci;
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
		 /*
      case WM_MOUSEMOVE:
      	RayTrace(p.x, p.y, &ci, true);
*/
      	break;
      	/*if(mstate == 1)
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
      	}*/
/*
      case WM_LBUTTONDOWN:
      	mstate = 1;
      	break;

      case WM_RBUTTONDOWN:
      	mstate = 2;
      	break;
*/
      /*case WM_LBUTTONUP:
      case WM_RBUTTONUP:
      	Draw();
      	mstate = 0;
      	break;
*/
     /* case WM_KEYDOWN:
      	if(wParam == 'X')
      		sel++;
      	if(wParam == 'Z')
      		sel--;
      	break;
      /*case WM_PAINT:
        HDC hdc;PAINTSTRUCT ps;
        hdc = BeginPaint(hWnd, &ps);
        BitBlt(hdc, 0, 0, MX, MY, myimage.hdcB, 0, 0, SRCCOPY);
        EndPaint(hWnd, &ps);
      	break;*/

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

