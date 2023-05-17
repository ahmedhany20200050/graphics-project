#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <windows.h>
#include<cmath>
int Max(int x,int y)
{
    if(x>y)
        return x;
    return y;
}
int Round(double x)
{
    return int(x+0.5);
}
void Draw4points(HDC hdc,int xc,int yc,int a,int b,COLORREF c)
{
    SetPixel(hdc,xc+a,yc+b,c);
    SetPixel(hdc,xc+a,yc-b,c);
    SetPixel(hdc,xc-a,yc+b,c);
    SetPixel(hdc,xc-a,yc-b,c);

//    SetPixel(hdc, xc+b, yc+a, c);
//    SetPixel(hdc, xc-b, yc+a, c);
//    SetPixel(hdc, xc-b, yc-a, c);
//    SetPixel(hdc, xc+b, yc-a, c);

}





// Direct Polar Method
void DrawEllipseDirectPolar(HDC hdc,int xc,int yc,int A,int B,COLORREF c)
{
    double dtheta=1.0/Max(A,B);
    for(double theta=0; theta<6.28; theta+=dtheta)
    {
        double x=xc+A*cos(theta);
        double y=yc+B*sin(theta);
        SetPixel(hdc,Round(x),Round(y),c);
    }
}




// Cartesian Equation
void Draw_Cartesian_Equation(HDC hdc,int xc,int yc,int r1,int r2,COLORREF c)
{
    double x=r1;
    double y=0;
    while(y*r1*r1<x*r2*r2)
    {
        y++;
        x=sqrt(1-(y*y)/(r2*r2))*r1;
        Draw4points(hdc,xc,yc,Round(x),y,c);
    }

     x=0;
     y=r2;
    Draw4points(hdc,xc,yc,x,y,c);
    while(x*r2*r2<y*r1*r1)
    {
        x++;
        y=sqrt(1-(x*x)/(r1*r1))*r2;
        Draw4points(hdc,xc,yc,x,Round(y),c);
    }

}






// draw Bresenham's
void DrawEllipse_Bresenham(HDC hdc,int xc,int yc,int a,int b,COLORREF c)
{
    int x=0;
    int y=b;
    Draw4points(hdc,xc,yc,x,y,c);
    while(y*a*a>x*b*b)
    {
        int d=4*b*b+8*b*b*x+a*a-4*a*a*y+4*a*a*y*y+4*b*b*x*x-4*b*b*a*a;
        if(d<0)
        {
            x++;
        }
        else
        {
            x++;
            y--;
        }
        Draw4points(hdc,xc,yc,x,y,c);

    }
    x=a;
    y=0;
    Draw4points(hdc,xc,yc,x,y,c);
    while(y*a*a<x*b*b)
    {
        int d=4*a*a+8*a*a*y+b*b-4*b*b*x+4*b*b*x*x+4*a*a*y*y-4*a*a*b*b;
        if(d<0)
        {
            y++;
        }
        else
        {
            y++;
            x--;
        }
        Draw4points(hdc,xc,yc,x,y,c);
    }
}





//  Bresenham's optimizes method Method
/*

 the algorithm calculates and stores certain values outside the loop to avoid redundant calculations.
 It also uses incremental updates (c1, c2, c1delta, c2delta) based on the decision variable (d1)
 to determine the next point coordinates.

By minimizing the number of arithmetic operations performed inside the loop,
 the optimized version can achieve better performance compared to the standard Bresenham's algorithm.

 */

void ellipseFasterBresenham(HDC hdc,int xc,int yc,int a,int b,COLORREF c)
{
    int x=0;
    int y=b;
    Draw4points(hdc,xc,yc,x,y,c);
    int d1=4*b*b-4*b*a*a+a*a;
    int c1=12*b*b;
    int c2=12*b*b*+8*a*a-8*a*a*b;
    int c1delta=8*b*b;
    int c2delta=c1delta+8*a*a;
    while(y*a*a>x*b*b)
    {
        if(d1<0)
        {
            d1+=c1;
            c2+=c1delta;
        }
        else
        {
            d1+=c2;
            c2+=c2delta;
            y--;
        }
        c1+=c1delta;
        x++;
        Draw4points(hdc,xc,yc,x,y,c);

    }
    x=a;
    y=0;
    Draw4points(hdc,xc,yc,x,y,c);
    d1=4*a*a-4*a*b*b+b*b;
    c1=12*a*a;
    c2=12*a*a+8*b*b-8*b*b*a;
    c1delta=8*a*a;
    c2delta=c1delta+8*b*b;
    while(x*b*b>y*a*a)
    {
        if(d1<0)
        {
            d1+=c1;
            c2+=c1delta;
        }
        else
        {
            d1+=c2;
            c2+=c2delta;
            x--;
        }
        c1+=c1delta;
        y++;
        Draw4points(hdc,xc,yc,x,y,c);
    }
}












// Draw_Polar_Equation
void Draw_Polar_Equation(HDC hdc,int xc,int yc,int r1,int r2,COLORREF c)
{
    //calculates the incremental angle dtheta between each point on the polar curve.
    // It is determined by taking the reciprocal of the maximum radius (r1 or r2).
    // ensuring evenly distribution of points
    double dtheta=1.0/Max(r1,r2);
    //starting point of the polar curve, which is (r1, 0).
    double x=r1;
    double y=0;
    double theta=0;
    // drawing the first part of the curve
    for( ; x*r2*r2>y*r1*r1; theta+=dtheta)
    {
        Draw4points(hdc,xc,yc,Round(x),Round(y),c);
        x=(r1*cos(theta));
        y=(r2*sin(theta));
    }
    x=0;
    y=r2;
    //draw the second part of the curve
    for( ; x*r2*r2<y*r1*r1; theta+=dtheta)
    {
        Draw4points(hdc,xc,yc,Round(x),Round(y),c);
        x=(r1*cos(theta));
        y=(r2*sin(theta));
    }

}




// Draw Polar Iterative Method
void Draw_Polar_Iterative(HDC hdc,int xc,int yc,double r1,double r2,COLORREF c)
{
    // calculates the incremental angle between each point on the polar curve
    double dtheta=1.0/Max(r1,r2);
    // cos theta
    double ctheta=cos(dtheta);
    // sin theta
    double stheta=sin(dtheta);
    //starting point of the polar curve, which is (r1, 0).
    double x=r1;
    double y=0;
    Draw4points(hdc,xc,yc,x,y,c);
    while(x>0)
    {
        // so as the circle with the diffrence (r1,r2),it ensures that the calculation of the next point being drawn.
        double x1=x*ctheta-(r1/r2)*y*stheta;
        y=y*ctheta+(r2/r1)*x*stheta;
        x=x1;
        Draw4points(hdc,xc,yc,Round(x),Round(y),c);
    }
}



/*  This function is called by the Windows function DispatchMessage()  */
int xs,ys,xe,ye,x3,y3;
double A, B;
static int cnt=0;
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{


    switch (message)                  /* handle the messages */
    {
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        case WM_LBUTTONDOWN:
        {
            if (cnt==0){
                xe=LOWORD(lParam);
                ye=HIWORD(lParam);

                cnt++;

            }
            else{
                xs=LOWORD(lParam);
                ys=HIWORD(lParam);
            }
            A=sqrt((xe-xs)*(xe-xs)+(ye-ys)*(ye-ys));
            break;

        }
        case WM_RBUTTONDOWN:
        {
            HDC hdc=GetDC(hwnd);
            x3=LOWORD(lParam);
            y3=HIWORD(lParam);
            B=sqrt((x3-xs)*(x3-xs)+(y3-ys)*(y3-ys));
            DrawEllipseDirectPolar(hdc,xs,ys,A,B,RGB(150,50,255));
            ReleaseDC(hwnd,hdc);
            break;
        }
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
int APIENTRY WinMain(HINSTANCE h, HINSTANCE p, LPSTR cmd, int csh) {
    WNDCLASS wc;
    wc.lpszClassName = "MyClass";
    wc.lpszMenuName = nullptr;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hbrBackground = (HBRUSH) GetStockObject(LTGRAY_BRUSH);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.lpfnWndProc = WindowProcedure;
    wc.hInstance = h;
    RegisterClass(&wc);
    HWND hWnd = CreateWindow("MyClass", "Hello", WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, nullptr, nullptr, h, nullptr);
    ShowWindow(hWnd, csh);
    UpdateWindow(hWnd);
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
