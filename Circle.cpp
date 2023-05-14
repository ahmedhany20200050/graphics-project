#include <Windows.h>
#include <iostream>
#include "cmath"
// This function swaps two points
void mySwap(int &a, int &b) {
    int tmp = a;
    a = b;
    b = tmp;
}

// This function round double number to integer number
int myRound(double num) {
    return (int) (num + 0.5);
}

void Draw8Points(HDC hdc,int xc,int yc, int a, int b,COLORREF color)
{
    SetPixel(hdc, xc+a, yc+b, color);
    SetPixel(hdc, xc-a, yc+b, color);
    SetPixel(hdc, xc-a, yc-b, color);
    SetPixel(hdc, xc+a, yc-b, color);
    SetPixel(hdc, xc+b, yc+a, color);
    SetPixel(hdc, xc-b, yc+a, color);
    SetPixel(hdc, xc-b, yc-a, color);
    SetPixel(hdc, xc+b, yc-a, color);
}

void CircleDirect(HDC hdc,int xc,int yc, int R,COLORREF color)
{
    int x=0,y=R;
    int R2=R*R;
    Draw8Points(hdc,xc,yc,x,y,color);
    while(x<y)
    {
        x++;
        y=myRound(sqrt((double)(R2-x*x)));
        Draw8Points(hdc,xc,yc,x,y,color);
    }
}
void CirclePolar(HDC hdc,int xc,int yc, int R,COLORREF color)
{
    int x=R,y=0;
    double theta=0,dtheta=1.0/R;
    Draw8Points(hdc,xc,yc,R,0,color);
    while(x>y)
    {
        theta+=dtheta;
        x= myRound(R*cos(theta));
        y= myRound(R*sin(theta));
        Draw8Points(hdc,xc,yc,x,y,color);
    }
}
void CircleIterativePolar(HDC hdc,int xc,int yc, int R,COLORREF color)
{
    double x=R,y=0;
    double dtheta=1.0/R;
    double cdtheta=cos(dtheta),sdtheta=sin(dtheta);
    Draw8Points(hdc,xc,yc,R,0,color);
    while(x>y)
    {
        double x1=x*cdtheta-y*sdtheta;
        y=x*sdtheta+y*cdtheta;
        x=x1;
        Draw8Points(hdc,xc,yc,myRound(x), myRound(y),color);
    }
}

//midpoint
void CircleBresenham(HDC hdc,int xc,int yc, int R,COLORREF color)
{
    int x=0,y=R;
    int d=1-R;
    Draw8Points(hdc,xc,yc,x,y,color);
    while(x<y)
    {
        if(d<0)
            d+=2*x+3 ;
        else
        {
            d+=2*(x-y)+5;
            y--;
        }
        x++;
        Draw8Points(hdc,xc,yc,x,y,color);
    }
}
//modified midpoint
void CircleFasterBresenham(HDC hdc,int xc,int yc, int R,COLORREF color)
{
    int x=0,y=R;
    int d=1-R;
    int c1=3, c2=5-2*R;
    Draw8Points(hdc,xc,yc,x,y,color);
    while(x<y)
    {
        if(d<0)
        {
            d+=c1;
            c2+=2;
        }
        else
        {
            d+=c2;
            c2+=4;
            y--;
        }
        c1+=2;
        x++;
        Draw8Points(hdc,xc,yc,x,y,color);
    }
}
//// void draw (HDC hdc,int xc,int yc, int R,COLORREF color)
//{
//    double dtheta = 1.0/R ;
//    for (double theta = 0; theta < 6.28 ; theta+=dtheta) {
//        double x = xc +R*cos(theta);
//        double y = yc +R*sin(theta);
//        SetPixel(hdc, myRound(x), myRound(y), color);
//    }
//}
//LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//static int x11,y11,x2,y2 ;
//static double r ;
//HDC hdc=GetDC(hwnd);
//switch (message)                  /* handle the messages*/
//{
//case WM_LBUTTONDOWN:
//x11=LOWORD(lParam);
//y11=HIWORD(lParam);
//break;
//case WM_RBUTTONDOWN:
//x2=LOWORD(lParam);
//y2=HIWORD(lParam);
//r=sqrt((x2-x11)*(x2-x11)+(y2-y11)*(y2-y11));
//CircleFasterBresenham(hdc,x11,y11,(int)r,RGB(255,0,0));
//break;
//case WM_DESTROY:
//PostQuitMessage (0);       /* send a WMQUIT to the message queue*/
//break;
//default:                      /* for messages that we don't deal with*/
//return DefWindowProc (hwnd, message, wParam, lParam);
//}
//
//return 0;
//}

//
//int APIENTRY WinMain(HINSTANCE h, HINSTANCE p, LPSTR cmd, int csh) {
//WNDCLASS wc;
//wc.lpszClassName = "MyClass";
//wc.lpszMenuName = nullptr;
//wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
//wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
//wc.hbrBackground = (HBRUSH) GetStockObject(LTGRAY_BRUSH);
//wc.style = CS_HREDRAW | CS_VREDRAW;
//wc.cbClsExtra = 0;
//wc.cbWndExtra = 0;
//wc.lpfnWndProc = WindowProcedure;
//wc.hInstance = h;
//RegisterClass(&wc);
//HWND hWnd = CreateWindow("MyClass", "Hello", WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, nullptr, nullptr, h, nullptr);
//ShowWindow(hWnd, csh);
//UpdateWindow(hWnd);
//MSG msg;
//while (GetMessage(&msg, nullptr, 0, 0) > 0) {
//TranslateMessage(&msg);
//DispatchMessage(&msg);
//}
//return 0;
//}