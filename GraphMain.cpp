
#include <windows.h>
#include <stdio.h>

#include "image.h"

#include "pathnode.h"
#include "unit.h"

HWND hWnd;               /* This is the handle for our window */

LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
char szClassName[ ] = "GraphAppWindow";





int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nFunsterStil)
{
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_WINDOW+1;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hWnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           "Graph",       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           640,                 /* The programs width */
           600,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );
    ShowWindow (hWnd, nFunsterStil);


    HDC hdc = GetDC(hWnd);
    MYIMAGE im;
    im.Open(640, 480);

	PATHNODE* pp = &pn[1];
	pp->valid = true;
	pp->type = PATH_TYPE_FACTORY;
	pp->x = pp->y = 100.0;
	pp->neighbour[0] = 2;

	pp = &pn[2];
	pp->valid = true;
	pp->type = PATH_TYPE_MINE;
	pp->x = pp->y = 170.0;
	pp->neighbour[0] = 3;

	pp = &pn[3];
	pp->valid = true;
	pp->type = PATH_TYPE_MINE;
	pp->x = 100.0;
	pp->y = 180.0;
	pp->neighbour[0] = 1;


	UNIT u;
	u.valid = true;
	u.fromnode = 1;
	u.tonode = 2;

	__int64 c1, c2;
	double freq, elap;
	QueryPerformanceFrequency((LARGE_INTEGER*)&c1);
	freq = c1;

	int count = 0;
    while (1)
    {
    	if(count++ > 10)
    	{
			QueryPerformanceCounter((LARGE_INTEGER*)&c2);
			elap = freq / (c2 - c1);
			char txt[100];
			sprintf(txt, "fps: %.1f", elap);
			TextOut(hdc, 0, 480, txt, strlen(txt));

			QueryPerformanceCounter((LARGE_INTEGER*)&c1);
			count = 0;
    	}
    	//im.Clear(0);
    	//DrawGraph(&im);
    	//u.Move();
    	//u.Draw(&im);




    	BitBlt(hdc, 0, 0, 640, 480, im.hdcB, 0, 0, SRCCOPY);



    	if(PeekMessage (&messages, NULL, 0, 0, PM_REMOVE))
		{
			if(messages.message == WM_QUIT) break;
			TranslateMessage(&messages);
			DispatchMessage(&messages);
		}
    }

    im.Close();
    ReleaseDC(hWnd, hdc);

    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
