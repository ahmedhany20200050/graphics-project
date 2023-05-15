#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include<bits/stdc++.h>
using namespace std;
/*  Declare Windows procedure  */

/**
    How this code works?
    we just get 2 points which represents the diameter of the rectangle
    as we get the top left point and the bottom right point
    to make the rectangle we want to do the clipping in
    then we get the point we want to make
    if it is inside the rectangle then it will be drawn

*/
void simpleDDA(HDC hdc, int xs, int ys, int xe, int ye, COLORREF color) {
    double dx = xe - xs;
    double dy = ye - ys;
    SetPixel(hdc, xs, ys, color);
    if (abs(dx) >= abs(dy)) {
        if (xs > xe) {
            swap(xe, xs);
            swap(ye, ys);
            dx = xe - xs;
            dy = ye - ys;
        }
        int x = xs;
        double y = ys, m = (double) dy / dx;
        while (x != xe) {
            x += 1;
            y += m;
            SetPixel(hdc, x, round(y), color);
        }
    } else {
        if (ys > ye) {
            swap(xe, xs);
            swap(ye, ys);
            dx = xe - xs;
            dy = ye - ys;
        }
        int y = ys;
        double x = xs, m = (double) dx / dy;
        while (y != ye) {
            x += m;
            y += 1;
            SetPixel(hdc, round(x), y, color);
        }
    }
}
void pointClippingRectangle(HDC hdc,int x,int y,int xl,int yt,int xr,int yb,COLORREF c){
    //first i draw the rectangle to make it clear that the point
    //is drawn inside it
    simpleDDA(hdc,xl,yt,xr,yt,c);
    simpleDDA(hdc,xl,yt,xl,yb,c);
    simpleDDA(hdc,xl,yb,xr,yb,c);
    simpleDDA(hdc,xr,yt,xr,yb,c);
    if (x>=xl&&x<=xr&&y>=yb&&y<=yt){
        SetPixel(hdc,x,y,c);
    }
}
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
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
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Code::Blocks Template Windows App"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */
static int  xleft,ytop,xright,ybottom;
static int x,y;
static int counter = 0;
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    HDC hdc=GetDC(hwnd);
    switch (message)                  /* handle the messages*/
    {
        case WM_LBUTTONDOWN:
            if (counter == 0){
                    //cout<<"first"<<endl;
                xleft=LOWORD(lParam);
                ytop = HIWORD(lParam);
                counter++;
            }
            else if(counter == 1){
                //cout<<"second"<<endl;
                xright = LOWORD(lParam);
                ybottom = HIWORD(lParam);
                if (xleft > xright){
                    swap(xleft,xright);

                }
                if (ybottom > ytop){
                    swap(ybottom,ytop);
                }
                counter++;
            }
            else{
                counter = 0;
                //cout<<"IM ABOUT TO CUM"<<endl;
                x = LOWORD(lParam);
                y = HIWORD(lParam);
                pointClippingRectangle(hdc,x,y,xleft,ytop,xright,ybottom,RGB(255,0,0));
                ReleaseDC(hwnd,hdc);

            }
            break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WMQUIT to the message queue*/
            break;
        default:                      /* for messages that we don't deal with*/
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
