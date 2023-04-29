#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include<bits/stdc++.h>
using namespace std;



void draw8Points(HDC hdc,int xc,int yc,int x,int y,COLORREF c){
    SetPixel(hdc,xc+x,yc+y,c);
    simpleDDA(hdc,xc+x,yc+y,xc,yc,RGB(0,0,255));
    SetPixel(hdc,xc-x,yc+y,c);
    SetPixel(hdc,xc-x,yc-y,c);
    simpleDDA(hdc,xc-x,yc-y,xc,yc,RGB(0,0,255));
    SetPixel(hdc,xc+x,yc-y,c);
    SetPixel(hdc,xc+y,yc+x,c);
    simpleDDA(hdc,xc+y,yc+x,xc,yc,RGB(0,0,255));
    SetPixel(hdc,xc-y,yc+x,c);
    SetPixel(hdc,xc-y,yc-x,c);
    simpleDDA(hdc,xc-y,yc-x,xc,yc,RGB(0,0,255));
    SetPixel(hdc,xc+y,yc-x,c);
}

void bezair(HDC hdc,double x1,double y1,double x2,double y2,double x3,double y3,double x4,double y4, COLORREF color)
{
    double a,b,c,d,aa,bb,cc,dd;
    a=-1*x1+3*x2+(-3*x3)+1*x4;
    b=3*x1+(-6*x2)+3*x3+0*x4;
    c=-3*x1+3*x2;
    d=x1;
    //coefficients for y
    aa=-1*y1+3*y2+(-3*y3)+1*y4;
    bb=3*y1+(-6*y2)+3*y3+0*y4;
    cc=-3*y1+3*y2;
    dd=y1;


    for (double t = 0.000; t <= 1.0; t+=0.001) {
        double x,y;
        x=a*(pow(t,3))+b*(t*t)+c*t+d;
        y=aa*(pow(t,3))+bb*(t*t)+cc*t+dd;
        SetPixel(hdc,x,y,color);
    }

}