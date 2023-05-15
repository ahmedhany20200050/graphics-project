#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include<bits/stdc++.h>
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))
/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");
void swap(int &x,int &y){
    int tmp = x;
    x = y;
    y = tmp;
}
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

struct OutCode
{
    int All =0;
    int left =0;
    int right =0;
    int top = 0;
    int bottom = 0;
};

OutCode GetOutCode(double x, double y, int xleft, int ytop, int xright, int ybottom)
{
    OutCode out;
    out.All = 0;
    out.left = 0;
    out.right = 0;
    out.top = 0;
    out.bottom = 0;

    if (x < xleft) {
        out.left = 1;
        out.All += out.left;
    } else if (x > xright) {
        out.right = 2;
        out.All += out.right;
    }
    if (y < ytop) {
        out.top = 4;
        out.All += out.top;
    } else if (y > ybottom) {
        out.bottom = 8;
        out.All += out.bottom;
    }

    return out;
}

void VIntersect(double xs,double ys,double xe,double ye,int x,double *xi,double *yi)
{
    *xi=x;
    *yi=ys+(x-xs)*(ye-ys)/(xe-xs);
}
void HIntersect(double xs,double ys,double xe,double ye,int y,double *xi,double *yi)
{
    *yi=y;
    *xi=xs+(y-ys)*(xe-xs)/(ye-ys);
}
void CohenSuth(HDC hdc,int xs,int ys,int xe,int ye,int xleft,int ytop,int xright,int ybottom)
{
    simpleDDA(hdc,xleft,ytop,xleft,ybottom,RGB(255,0,0));
    simpleDDA(hdc,xleft,ytop,xright,ytop,RGB(255,0,0));
    simpleDDA(hdc,xleft,ybottom,xright,ybottom,RGB(255,0,0));
    simpleDDA(hdc,xright,ytop,xright,ybottom,RGB(255,0,0));
    double x1=xs,y1=ys,x2=xe,y2=ye;
    //simpleDDA(hdc,round(x1),round(y1),round(x2),round(y2),RGB(255,0,0));
    OutCode out1=GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
    OutCode out2=GetOutCode(x2,y2,xleft,ytop,xright,ybottom);
    printf("%d\n",out1.All);
    printf("%d\n",out2.All);
    while( (out1.All || out2.All) && !(out1.All & out2.All))
    {
        double xi,yi;
        if(out1.All)
        {
            if(out1.left)VIntersect(x1,y1,x2,y2,xleft,&xi,&yi);
            else if(out1.top)HIntersect(x1,y1,x2,y2,ytop,&xi,&yi);
            else if(out1.right)VIntersect(x1,y1,x2,y2,xright,&xi,&yi);
            else HIntersect(x1,y1,x2,y2,ybottom,&xi,&yi);
            x1=xi;
            y1=yi;
            out1=GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
        }
        else
            {
                if(out2.left)VIntersect(x1,y1,x2,y2,xleft,&xi,&yi);
                else if(out2.top)HIntersect(x1,y1,x2,y2,ytop,&xi,&yi);
                else if(out2.right)VIntersect(x1,y1,x2,y2,xright,&xi,&yi);
                else HIntersect(x1,y1,x2,y2,ybottom,&xi,&yi);
                    x2=xi;
                    y2=yi;
                    out2=GetOutCode(x2,y2,xleft,ytop,xright,ybottom);
            }
    }
    //cout<<out1.All<<" "<<out2.All<<endl;

    if(!out1.All && !out2.All)
    {
        simpleDDA(hdc,round(x1),round(y1),round(x2),round(y2),RGB(255,0,0));

    }
}

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
static int counter = 0;
static int x11,y11,x22,y22;
static int xR,xL,yT,yB;
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //hello
    //in order to make this code works properly
    //first you have to click on the top left corner of square
    //then you click on the bottom right corner
    //then it will make it's calculations
    //then you click on the 2 points of the line
    HDC hdc=GetDC(hwnd);
    switch (message)                  /* handle the messages */
    {
        case WM_LBUTTONDOWN:
            if (counter == 0){
                xL = LOWORD(lParam);
                yT = HIWORD(lParam);
                counter++;
            }
            else if(counter == 1){
                xR = LOWORD(lParam);
                yB = HIWORD(lParam);
                int width = xR - xL;
                int height = yT - yB;
                if (width < 0)width = -1 * width;
                if (height < 0)height = -1 * height;
                int sideLength = min(width,height);
                xR = xL + sideLength;
                yB = yT + sideLength;
                counter++;
            }
            else if (counter == 2){
                x11 = LOWORD(lParam);
                y11 = HIWORD(lParam);
                counter++;
            }
            else{
                x22 = LOWORD(lParam);
                y22 = HIWORD(lParam);
                counter = 0;

                CohenSuth(hdc,x11,y11,x22,y22,xL,yT,xR,yB);
                ReleaseDC(hwnd,hdc);
            }
            break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
