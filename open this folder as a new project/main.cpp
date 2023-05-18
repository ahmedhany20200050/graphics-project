
#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include "iostream"
#include "beziar.h"
#include "Circle.h"
#include "2d_transformation.h"
#include <cstring>
#include<bits/stdc++.h>
#include <dshow.h>
#include <unistd.h>
#include "Line.h"
#include "lineClippingRectangle.h"
#include "lineClippingSquare.h"
#include "pointClippingRectangle.h"
#include "polygonClipping.h"
#include "thread"
#include "filling.h"
#include "rectangle.h"
#include "ellipse.h"

using namespace std;





/*  Declare Windows procedure  */
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
    wincl.hCursor = LoadCursor (NULL, IDC_HELP);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) CTLCOLOR_SCROLLBAR;

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
static int x,x2,x3,x4;
static int y,y2,y3,y4,counter=0;
static int xfirst,xend,yfirst,yend;
static string choice="none";
static int lineBeginX,lineBeginY,lineEndX,lineEndY;
static COLORREF mainColor= RGB(0,0,255);
static bool isChanged= false;
static int xleft,ytop,ybottom,xright;
double radius;
int fillingQuarter;
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{


    HDC hdc=GetDC(hwnd);
    //we go depending on choice
    switch (message) {
        case WM_LBUTTONDOWN:
            if(isChanged){
                char  *s= (char*) malloc(sizeof(char));
                GetWindowTextA(GetDlgItem(hwnd, 100),s,55);
                string ss;
                cout<<"we are good\n";
                for (int i = 0; i < 55; ++i) {
                    cout<<s[i];
                    if(s[i]=='\0')break;
                    ss+=s[i];
                }
                cout<<"\n";
                choice=ss;
                isChanged= false;
            }

            thread workerThread([hdc,lParam,hwnd](){
                if(choice=="bezier"){
                    if(counter==0){
                        x= LOWORD(lParam);
                        y= HIWORD(lParam);
                        counter++;
                    }else if(counter==1){
                        x2= LOWORD(lParam);
                        y2= HIWORD(lParam);
                        counter++;
                    }else if(counter==2){
                        x3= LOWORD(lParam);
                        y3= HIWORD(lParam);
                        counter++;
                    }else if(counter==3){
                        x4= LOWORD(lParam);
                        y4= HIWORD(lParam);
                        bezair(hdc,x,y,x2,y2,x3,y3,x4,y4,mainColor);
                        counter=0;
                    }

                } else if(choice=="line dda"){
                    cout<<"inside line dda\n";
                    if(counter==0){
                        lineBeginX= LOWORD(lParam);
                        lineBeginY= HIWORD(lParam);
                        counter++;
                    }else if(counter==1){
                        lineEndX= LOWORD(lParam);
                        lineEndY= HIWORD(lParam);
                        simpleDDA(hdc,lineBeginX,lineBeginY,lineEndX,lineEndY,mainColor);
                        counter=0;
                    }
                }else if(choice=="line midpoint"){
                    cout<<"inside line midpoint\n";
                    if(counter==0){
                        lineBeginX= LOWORD(lParam);
                        lineBeginY= HIWORD(lParam);
                        counter++;
                    }else if(counter==1){
                        lineEndX= LOWORD(lParam);
                        lineEndY= HIWORD(lParam);
                        Bresenham(hdc,lineBeginX,lineBeginY,lineEndX,lineEndY,mainColor);
                        counter=0;
                    }
                }else if(choice=="line parametric"){
                    cout<<"inside line parametric\n";
                    if(counter==0){
                        lineBeginX= LOWORD(lParam);
                        lineBeginY= HIWORD(lParam);
                        counter++;
                    }else if(counter==1){
                        lineEndX= LOWORD(lParam);
                        lineEndY= HIWORD(lParam);
                        directMethod1(hdc,lineBeginX,lineBeginY,lineEndX,lineEndY,mainColor);
                        counter=0;
                    }
                }else if(choice=="circle direct"){
                    cout<<"inside circle direct\n";
                    if(counter==0){
                        x= LOWORD(lParam);
                        y= HIWORD(lParam);
                        counter++;
                    }else if(counter==1){
                        x2= LOWORD(lParam);
                        y2= HIWORD(lParam);
                        int r= ::sqrt(((x-x2)*(x-x2))+((y-y2)*(y-y2)));
                        CircleDirect(hdc,x,y,r,mainColor);
                        counter=0;
                    }
                }else if(choice=="circle polar"){
                    cout<<"inside circle polar\n";
                    if(counter==0){
                        x= LOWORD(lParam);
                        y= HIWORD(lParam);
                        counter++;
                    }else if(counter==1){
                        x2= LOWORD(lParam);
                        y2= HIWORD(lParam);
                        int r= ::sqrt(((x-x2)*(x-x2))+((y-y2)*(y-y2)));
                        CirclePolar(hdc,x,y,r,mainColor);
                        counter=0;
                    }
                }else if(choice=="circle iterative polar"){
                    cout<<"inside circle iterative polar\n";
                    if(counter==0){
                        x= LOWORD(lParam);
                        y= HIWORD(lParam);
                        counter++;
                    }else if(counter==1){
                        x2= LOWORD(lParam);
                        y2= HIWORD(lParam);
                        int r= ::sqrt(((x-x2)*(x-x2))+((y-y2)*(y-y2)));
                        CircleIterativePolar(hdc,x,y,r,mainColor);
                        counter=0;
                    }
                }else if(choice=="circle midpoint"){
                    cout<<"inside circle midpoint\n";
                    if(counter==0){
                        x= LOWORD(lParam);
                        y= HIWORD(lParam);
                        counter++;
                    }else if(counter==1){
                        x2= LOWORD(lParam);
                        y2= HIWORD(lParam);
                        int r= ::sqrt(((x-x2)*(x-x2))+((y-y2)*(y-y2)));
                        CircleBresenham(hdc,x,y,r,mainColor);
                        counter=0;
                    }
                }else if(choice=="circle modified midpoint"){
                    cout<<"inside circle modified midpoint\n";
                    if(counter==0){
                        x= LOWORD(lParam);
                        y= HIWORD(lParam);
                        counter++;
                    }else if(counter==1){
                        x2= LOWORD(lParam);
                        y2= HIWORD(lParam);
                        int r= ::sqrt(((x-x2)*(x-x2))+((y-y2)*(y-y2)));
                        CircleBresenham(hdc,x,y,r,mainColor);
                        counter=0;
                    }
                }else if(choice=="circle modified midpoint"){
                    cout<<"inside circle modified midpoint\n";
                    if(counter==0){
                        x= LOWORD(lParam);
                        y= HIWORD(lParam);
                        counter++;
                    }else if(counter==1){
                        x2= LOWORD(lParam);
                        y2= HIWORD(lParam);
                        int r= ::sqrt(((x-x2)*(x-x2))+((y-y2)*(y-y2)));
                        CircleBresenham(hdc,x,y,r,mainColor);
                        counter=0;
                    }
                }else if(choice== "fill circle with lines"){
                    cout<<"inside fill circle with lines"<<endl;
                    if(counter==0){
                        x = LOWORD(lParam);
                        y = HIWORD(lParam);
                        counter++;
                    }else if(counter==1){
                        x2 = LOWORD(lParam);
                        y2= HIWORD(lParam);
                        radius = sqrt(pow(abs(x - x2), 2) + pow(abs(y - y2), 2));
                        CircleFasterBresenham(hdc,x,y,(int)radius,mainColor);
                        simpleDDA(hdc,x+(int)radius,y,x-(int)radius,y,mainColor);
                        simpleDDA(hdc,x,y-(int)radius,x,y+(int)radius,mainColor);
                        counter++;
                    }else if(counter>=2){
                        x2 = LOWORD(lParam);
                        y2 = HIWORD(lParam);
                        if(x2>x && x2<(x+(int)radius) && y2<y && y2>y-(int)radius){
                            fillingQuarter=1;
                            counter++;
                        }else if(x2>(x-(int)radius) && x2<x && y2 <y && y2>(y-(int)radius)){
                            fillingQuarter=2;
                            counter++;
                        }else if(x2>(x-(int)radius) && x2<x && y2>y && y2<(y+(int)radius)){
                            fillingQuarter=3;
                            counter++;
                        }else if(x2>x && x2<(x+(int)radius) && y2>y && y2<(y+(int)radius)){
                            fillingQuarter=4;
                            counter++;
                        }else{
                            counter=0;
                        }
                        if(counter!=0){
                            fillTheCircleWithLines(hdc,x,y, (int)radius,fillingQuarter,mainColor);
                            // Or
//                            fillTheCircleWithCircles(hdc,x,y, (int)radius,fillingQuarter,mainColor);
                        }
                    }
                }else if(choice== "fill circle with circles"){
                    cout<<"inside fill circle with circles"<<endl;
                    if(counter==0){
                        x = LOWORD(lParam);
                        y = HIWORD(lParam);
                        counter++;
                    }else if(counter==1){
                        x2 = LOWORD(lParam);
                        y2= HIWORD(lParam);
                        radius = sqrt(pow(abs(x - x2), 2) + pow(abs(y - y2), 2));
                        CircleFasterBresenham(hdc,x,y,(int)radius,mainColor);
                        simpleDDA(hdc,x+(int)radius,y,x-(int)radius,y,mainColor);
                        simpleDDA(hdc,x,y-(int)radius,x,y+(int)radius,mainColor);
                        counter++;
                    }else if(counter>=2){
                        x2 = LOWORD(lParam);
                        y2 = HIWORD(lParam);
                        if(x2>x && x2<(x+(int)radius) && y2<y && y2>y-(int)radius){
                            fillingQuarter=1;
                            counter++;
                        }else if(x2>(x-(int)radius) && x2<x && y2 <y && y2>(y-(int)radius)){
                            fillingQuarter=2;
                            counter++;
                        }else if(x2>(x-(int)radius) && x2<x && y2>y && y2<(y+(int)radius)){
                            fillingQuarter=3;
                            counter++;
                        }else if(x2>x && x2<(x+(int)radius) && y2>y && y2<(y+(int)radius)){
                            fillingQuarter=4;
                            counter++;
                        }else{
                            counter=0;
                        }
                        if(counter!=0){
//                            fillTheCircleWithLines(hdc,x,y, (int)radius,fillingQuarter,mainColor);
                            // Or
                            fillTheCircleWithCircles(hdc,x,y, (int)radius,fillingQuarter,mainColor);
                        }
                    }
                }else if(choice== "fill square with hermit"){
                    cout<<"inside fill square with hermit"<<endl;

                    if(counter==0){
                        x= LOWORD(lParam);
                        y= HIWORD(lParam);
                        drawSquareWithHermit(hdc,x,y,mainColor);
                        counter=0;
                    }
                }else if(choice== "fill rectangle with bezier"){
                    cout<<"inside fill rectangle with bezier"<<endl;
                    if(counter==0){
                        x= LOWORD(lParam);
                        y= HIWORD(lParam);
                        drawRectangleWithBezier(hdc,x,y,mainColor);
                        counter=0;
                    }
                }else if(choice== "fill square with bezier"){
                    cout<<"inside fill square with  bezier"<<endl;
                    if(counter==0){
                        counter++;
                    }else if(counter==1){
                        counter++;
                    }else if(counter==2){
                        counter=0;
                    }
                }else if(choice== "convex filling"){
                    cout<<"inside convex filling"<<endl;
//                performConvexFilling()
                    if(counter==0){
                        x= LOWORD(lParam);
                        y= HIWORD(lParam);
                        counter++;
                    }else if(counter==1){
                        x2= LOWORD(lParam);
                        y2= HIWORD(lParam);
                        counter++;
                    }else if(counter==2){
                        x3= LOWORD(lParam);
                        y3= HIWORD(lParam);
                        Point points[3]={Point(x,y),Point(x2,y2),Point(x3,y3)};
                        performConvexFilling(hdc,points,3,mainColor);
                        counter=0;
                    }
                }else if(choice== "non convex filling"){
                    cout<<"inside non convex filling"<<endl;
                    if(counter==0){
                        x= LOWORD(lParam);
                        y= HIWORD(lParam);
                        counter++;
                    }else if(counter==1){
                        x2= LOWORD(lParam);
                        y2= HIWORD(lParam);
                        counter++;
                    }else if(counter==2){
                        x3= LOWORD(lParam);
                        y3= HIWORD(lParam);
                        counter++;
                    }else if(counter==3){
                        x4= LOWORD(lParam);
                        y4= HIWORD(lParam);
                        counter++;
                    } else if(counter==4){
                        int x5= LOWORD(lParam);
                        int y5= HIWORD(lParam);
                        Point points[5]={Point(x,y),Point(x2,y2),Point(x3,y3),Point(x4,y4),Point(x5,y5)};
                        performGeneralPolygonFilling(hdc,points,5,mainColor);
                        counter=0;
                    }
                }else if(choice== "recursive flood fill"){
                    cout<<"inside recursive fill"<<endl;
                    if(counter==0){
                        x= LOWORD(lParam);
                        y= HIWORD(lParam);
                        performRecursiveFloodFillAlgorithm(hdc,x,y,mainColor,mainColor);
                        counter=0;
                    }
                }else if(choice== "non recursive flood fill"){
                    cout<<"inside non recursive fill"<<endl;
                    if(counter==0){
                        x= LOWORD(lParam);
                        y= HIWORD(lParam);
                        performNonRecursiveFloodFillAlgorithm(hdc,x,y,mainColor,mainColor);
                        counter=0;
                    }
                }else if(choice== "ellipse polar"){
                    cout<<"inside ellipse polar"<<endl;
                    if(counter==0){
                        x2= LOWORD(lParam);
                        y2= HIWORD(lParam);
                        counter++;
                    }else if(counter==1){
                        x= LOWORD(lParam);
                        y= HIWORD(lParam);
                        counter++;
                    }else if(counter==2){
                        x3= LOWORD(lParam);
                        y3= HIWORD(lParam);
                        int B=sqrt((x-x2)*(x-x2)+(y-y2)*(y-y2));
                        int A=sqrt((x3-x2)*(x3-x2)+(y3-y2)*(y3-y2));
                        Draw_Polar_Iterative(hdc,x2,y2,A,B,mainColor);
                        counter=0;
                    }
                }else if(choice== "ellipse direct"){
                    cout<<"inside ellipse direct"<<endl;
                    if(counter==0){
                        x2= LOWORD(lParam);
                        y2= HIWORD(lParam);
                        counter++;
                    }else if(counter==1){
                        x= LOWORD(lParam);
                        y= HIWORD(lParam);
                        counter++;
                    }else if(counter==2){
                        x3= LOWORD(lParam);
                        y3= HIWORD(lParam);
                        int B=sqrt((x-x2)*(x-x2)+(y-y2)*(y-y2));
                        int A=sqrt((x3-x2)*(x3-x2)+(y3-y2)*(y3-y2));
                        Draw_Cartesian_Equation(hdc,x2,y2,A,B,mainColor);
                        counter=0;
                    }
                }else if(choice== "ellipse midpoint"){
                    cout<<"inside ellipse midpoint"<<endl;
                    if(counter==0){
                        x2= LOWORD(lParam);
                        y2= HIWORD(lParam);
                        counter++;
                    }else if(counter==1){
                        x= LOWORD(lParam);
                        y= HIWORD(lParam);
                        counter++;
                    }else if(counter==2){
                        x3= LOWORD(lParam);
                        y3= HIWORD(lParam);
                        //distance between two points
                        int B=sqrt((x-x2)*(x-x2)+(y-y2)*(y-y2));
                        int A=sqrt((x3-x2)*(x3-x2)+(y3-y2)*(y3-y2));
                        DrawEllipse_Bresenham(hdc,x2,y2,A,B,mainColor);
                        counter=0;
                    }
                }else if(choice== "lineClippingRectangle"){
                    if(counter==0){
                        x= LOWORD(lParam);
                        y= HIWORD(lParam);
                        xleft=x,ytop=y,ybottom=y+40,xright=x+100;
                        simpleDDA(hdc,xleft,ytop,xleft,ybottom,mainColor);
                        simpleDDA(hdc,xleft,ytop,xright,ytop,mainColor);
                        simpleDDA(hdc,xleft,ybottom,xright,ybottom,mainColor);
                        simpleDDA(hdc,xright,ytop,xright,ybottom,mainColor);
                        counter++;
                    }else if(counter==1){
                        x= LOWORD(lParam);
                        y= HIWORD(lParam);
                        counter++;
                    }else if(counter==2){
                        int xx= LOWORD(lParam);
                        int yy= HIWORD(lParam);
                        CohenSuth(hdc,x,y,xx,yy,xleft,ytop,xright,ybottom);
                        counter=0;
                    }
                }else if(choice== "lineClippingSquare"){
                    if(counter==0){
                        x= LOWORD(lParam);
                        y= HIWORD(lParam);
                        xleft=x,ytop=y,ybottom=y+100,xright=x+100;
                        simpleDDA(hdc,xleft,ytop,xleft,ybottom,mainColor);
                        simpleDDA(hdc,xleft,ytop,xright,ytop,mainColor);
                        simpleDDA(hdc,xleft,ybottom,xright,ybottom,mainColor);
                        simpleDDA(hdc,xright,ytop,xright,ybottom,mainColor);
                        counter++;
                    }else if(counter==1){
                        x= LOWORD(lParam);
                        y= HIWORD(lParam);
                        counter++;
                    }else if(counter==2){
                        int xx= LOWORD(lParam);
                        int yy= HIWORD(lParam);
                        CohenSuth(hdc,x,y,xx,yy,xleft,ytop,xright,ybottom);
                        counter=0;
                    }
                }else if(choice== "pointClippingRectangle"){
                    if(counter==0){
                        x= LOWORD(lParam);
                        y= HIWORD(lParam);
                        xleft=x,ytop=y,ybottom=y+30,xright=x+50;
                        simpleDDA(hdc,xleft,ytop,xleft,ybottom,mainColor);
                        simpleDDA(hdc,xleft,ytop,xright,ytop,mainColor);
                        simpleDDA(hdc,xleft,ybottom,xright,ybottom,mainColor);
                        simpleDDA(hdc,xright,ytop,xright,ybottom,mainColor);
                        counter++;
                    }else if(counter==1){
                        x= LOWORD(lParam);
                        y= HIWORD(lParam);
                        pointClippingRectangle(hdc,x,y,xleft,ytop,xright,ybottom,mainColor);
                        counter=0;
                    }
                }else if(choice== "pointClippingSquare"){
                    if(counter==0){
                        x= LOWORD(lParam);
                        y= HIWORD(lParam);
                        xleft=x,ytop=y,ybottom=y+60,xright=x+60;
                        simpleDDA(hdc,xleft,ytop,xleft,ybottom,mainColor);
                        simpleDDA(hdc,xleft,ytop,xright,ytop,mainColor);
                        simpleDDA(hdc,xleft,ybottom,xright,ybottom,mainColor);
                        simpleDDA(hdc,xright,ytop,xright,ybottom,mainColor);
                        counter++;
                    }else if(counter==1){
                        x= LOWORD(lParam);
                        y= HIWORD(lParam);
                        pointClippingRectangle(hdc,x,y,xleft,ytop,xright,ybottom,mainColor);
                        counter=0;
                    }
                }else if(choice== "polygonClipping"){
                    if(counter==0){
                        x= LOWORD(lParam);
                        y= HIWORD(lParam);
                        xleft=x,ytop=y,ybottom=y+40,xright=x+70;
                        simpleDDA(hdc,xleft,ytop,xleft,ybottom,mainColor);
                        simpleDDA(hdc,xleft,ytop,xright,ytop,mainColor);
                        simpleDDA(hdc,xleft,ybottom,xright,ybottom,mainColor);
                        simpleDDA(hdc,xright,ytop,xright,ybottom,mainColor);
                        counter++;
                    }else if(counter==1){
                        x2= LOWORD(lParam);
                        y2= HIWORD(lParam);
                        counter++;
                    }else if(counter==2){
                        x3= LOWORD(lParam);
                        y3= HIWORD(lParam);
                        counter++;
                    }else if(counter==3){
                        x4= LOWORD(lParam);
                        y4= HIWORD(lParam);
                        counter++;
                    }else if(counter==4){
                        int x5= LOWORD(lParam);
                        int y5= HIWORD(lParam);
                        vector<vertex>v;
                        v.emplace_back(x,y);
                        v.emplace_back(x2,y2);
                        v.emplace_back(x3,y3);
                        v.emplace_back(x4,y4);
                        v.emplace_back(x5,y5);
                        polyGonClipping(hdc,v,xleft,ytop,xright,ybottom,mainColor);
                        counter=0;
                    }
                }else if(choice=="translate line"){
                    thread translatingThread([hdc](){
                        cout<<"Inside translate line\n";
                        cout<<"enter x translate"<<endl;
                        cin>>x;
                        cout<<"enter y translate"<<endl;
                        cin>>y;
                        pair<int,int>beginOfLine={lineBeginX,lineBeginY};
                        pair<int,int>endOfLine={lineEndX,lineEndY};
                        vector<pair<int,int>>points={beginOfLine,endOfLine};
                        vector<pair<int,int>> newPoints=translateLine(points,x,y);
                        simpleDDA(hdc,newPoints[0].first,newPoints[0].second,newPoints[1].first,newPoints[1].second,mainColor);
                    });
                    translatingThread.detach();
//                sleep(5);
                }else if(choice=="scale line"){
                    thread scalingThread([hdc](){
                        cout<<"enter x scale"<<endl;
                        cin>>x;
                        cout<<"enter y scale"<<endl;
                        cin>>y;
                        pair<int,int>beginOfLine={lineBeginX,lineBeginY};
                        pair<int,int>endOfLine={lineEndX,lineEndY};
                        vector<pair<int,int>>points={beginOfLine,endOfLine};
                        vector<pair<int,int>> newPoints=scaleLine({{lineBeginX,lineBeginY},{lineEndX,lineEndY}},x,y);
                        cout<<newPoints[0].first<<"  "<<"  "<< newPoints[0].second<<"  "<<newPoints[1].first<<"  "<<newPoints[1].second<<endl;
                        simpleDDA(hdc,newPoints[0].first,newPoints[0].second,newPoints[1].first,newPoints[1].second,mainColor);
                    });
                    scalingThread.detach();
//                sleep(5);
                }else if(choice=="use red"){
                    cout<<"color is now red"<<endl;
                    mainColor= RGB(255,0,0);
                }else if(choice=="use green"){
                    cout<<"color is now green"<<endl;
                    mainColor= RGB(0,255,0);
                }else if(choice=="use blue"){
                    cout<<"color is now blue"<<endl;
                    mainColor= RGB(0,0,255);
                }else if(choice=="white background"){
                    RECT window;
                    GetWindowRect(hwnd, &window);

                    int width = window.right - window.left;
                    int height = window.bottom - window.top;
                    for (int i = 0; i <= window.right ; ++i) {
                        for (int j = 0; j <= window.bottom ; ++j) {
                            SetPixel(hdc,i,j,RGB(255,255,255));
                        }
                    }
                }else if(choice=="clear"){
                    RECT window;
                    GetWindowRect(hwnd, &window);

                    int width = window.right - window.left;
                    int height = window.bottom - window.top;
                    for (int i = 0; i <= window.right ; ++i) {
                        for (int j = 0; j <= window.bottom ; ++j) {
                            if(i<300&&j<20)continue;
                            SetPixel(hdc,i,j,RGB(240,240,240));
                        }
                    }
                }else if(choice=="save"){
                    RECT window;
                    GetWindowRect(hwnd, &window);

                    int width = window.right - window.left;
                    int height = window.bottom - window.top;


//                simpleDDA(hdc,width-18,25,width,25,RGB(0,0,0));
                    thread savingThread([hdc,width,height](){
                        ofstream file("array.txt");
                        for (int i = 0; i < width ; ++i) {
                            for (int j = 0; j < height ; ++j) {
                                if(i<300&&j<25) {
                                    file << GetPixel(hdc,i,j) << " ";
                                    continue;
                                };
                                COLORREF c=GetPixel(hdc,i,j);
                                file << c << " ";
                                SetPixel(hdc,i,j,RGB(0,0,0));

                            }
                            file<<endl;
                        }
                        file.close();
                    });
                    savingThread.detach();

                }else if(choice=="load"){
                    RECT window;
                    GetWindowRect(hwnd, &window);

                    int width = window.right - window.left;
                    int height = window.bottom - window.top;
                    ifstream file("array.txt");
                    for (int i = 0; i < width ; ++i) {
                        for (int j = 0; j < height ; ++j) {
                            COLORREF currentColor;
                            file >> currentColor;
                            if(i<300&&j<25)continue;
                            SetPixel(hdc,i,j,currentColor);
                        }
                    }
                    file.close();
                }
            });
            workerThread.detach();

            break;
    }



    //combobox things
    switch (message)                  /* handle the messages*/
    {

        case WM_CREATE:
            CreateWindowEx(WS_EX_STATICEDGE, "COMBOBOX", "MyCombo1",
                           CBS_DROPDOWN | WS_CHILD | WS_VISIBLE,
                           0, 0, 300, 800, hwnd, reinterpret_cast<HMENU>(100), reinterpret_cast<HINSTANCE>(hwnd), NULL); // 100 = ID of this control;
            ComboBox_AddItemData(GetDlgItem(hwnd, 100),"bezier");
            ComboBox_AddItemData(GetDlgItem(hwnd, 100),"line dda");
            ComboBox_AddItemData(GetDlgItem(hwnd, 100),"line midpoint");
            ComboBox_AddItemData(GetDlgItem(hwnd, 100),"line parametric");
            ComboBox_AddItemData(GetDlgItem(hwnd, 100),"circle direct");
            ComboBox_AddItemData(GetDlgItem(hwnd, 100),"circle polar");
            ComboBox_AddItemData(GetDlgItem(hwnd, 100),"circle iterative polar");
            ComboBox_AddItemData(GetDlgItem(hwnd, 100),"circle midpoint");
            ComboBox_AddItemData(GetDlgItem(hwnd, 100),"circle modified midpoint");
            ComboBox_AddItemData(GetDlgItem(hwnd, 100),"fill circle with lines");
            ComboBox_AddItemData(GetDlgItem(hwnd, 100),"fill circle with circles");
            ComboBox_AddItemData(GetDlgItem(hwnd, 100),"fill square with hermit");
            ComboBox_AddItemData(GetDlgItem(hwnd, 100),"fill rectangle with bezier");
            ComboBox_AddItemData(GetDlgItem(hwnd, 100),"convex filling");
            ComboBox_AddItemData(GetDlgItem(hwnd, 100),"non convex filling");
            ComboBox_AddItemData(GetDlgItem(hwnd, 100),"recursive flood fill");
            ComboBox_AddItemData(GetDlgItem(hwnd, 100),"non recursive flood fill");
            ComboBox_AddItemData(GetDlgItem(hwnd, 100),"ellipse direct");
            ComboBox_AddItemData(GetDlgItem(hwnd, 100),"ellipse polar");
            ComboBox_AddItemData(GetDlgItem(hwnd, 100),"ellipse midpoint");

            ComboBox_AddItemData(GetDlgItem(hwnd, 100),"lineClippingRectangle");
            ComboBox_AddItemData(GetDlgItem(hwnd, 100),"lineClippingSquare");
            ComboBox_AddItemData(GetDlgItem(hwnd, 100),"pointClippingRectangle");
            ComboBox_AddItemData(GetDlgItem(hwnd, 100),"pointClippingSquare");
            ComboBox_AddItemData(GetDlgItem(hwnd, 100),"polygonClipping");
            ComboBox_AddItemData(GetDlgItem(hwnd, 100),"translate line");
            ComboBox_AddItemData(GetDlgItem(hwnd, 100),"scale line");
            ComboBox_AddItemData(GetDlgItem(hwnd, 100),"use red");
            ComboBox_AddItemData(GetDlgItem(hwnd, 100),"use green");
            ComboBox_AddItemData(GetDlgItem(hwnd, 100),"use blue");
            ComboBox_AddItemData(GetDlgItem(hwnd, 100),"white background");
            ComboBox_AddItemData(GetDlgItem(hwnd, 100),"clear");
            ComboBox_AddItemData(GetDlgItem(hwnd, 100),"save");
            ComboBox_AddItemData(GetDlgItem(hwnd, 100),"load");



            break;
        case WM_COMMAND:
            if(HIWORD(wParam)== CBN_CLOSEUP){
                isChanged= true;
                counter=0;

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

/*
 * these functions were here before including other cpp files
 * there existence causes overlapping with function in other files so i commented them
 *
 *       by Ahmed Hany
 *
 */

//void simpleDDA(HDC hdc, int xs, int ys, int xe, int ye, COLORREF color) {
//    double dx = xe - xs;
//    double dy = ye - ys;
//    SetPixel(hdc, xs, ys, color);
//    if (abs(dx) >= abs(dy)) {
//        if (xs > xe) {
//            swap(xe, xs);
//            swap(ye, ys);
//            dx = xe - xs;
//            dy = ye - ys;
//        }
//        int x = xs;
//        double y = ys, m = (double) dy / dx;
//        while (x != xe) {
//            x += 1;
//            y += m;
//            SetPixel(hdc, x, round(y), color);
//        }
//    } else {
//        if (ys > ye) {
//            swap(xe, xs);
//            swap(ye, ys);
//            dx = xe - xs;
//            dy = ye - ys;
//        }
//        int y = ys;
//        double x = xs, m = (double) dx / dy;
//        while (y != ye) {
//            x += m;
//            y += 1;
//            SetPixel(hdc, round(x), y, color);
//        }
//    }
//}
//void draw8Points(HDC hdc,int xc,int yc,int x,int y,COLORREF c){
//    SetPixel(hdc,xc+x,yc+y,c);
//    simpleDDA(hdc,xc+x,yc+y,xc,yc,RGB(0,0,255));
//    SetPixel(hdc,xc-x,yc+y,c);
//    SetPixel(hdc,xc-x,yc-y,c);
//    simpleDDA(hdc,xc-x,yc-y,xc,yc,RGB(0,0,255));
//    SetPixel(hdc,xc+x,yc-y,c);
//    SetPixel(hdc,xc+y,yc+x,c);
//    simpleDDA(hdc,xc+y,yc+x,xc,yc,RGB(0,0,255));
//    SetPixel(hdc,xc-y,yc+x,c);
//    SetPixel(hdc,xc-y,yc-x,c);
//    simpleDDA(hdc,xc-y,yc-x,xc,yc,RGB(0,0,255));
//    SetPixel(hdc,xc+y,yc-x,c);
//}

//void bezair(HDC hdc,double x1,double y1,double x2,double y2,double x3,double y3,double x4,double y4, COLORREF color)
//{
//    double a,b,c,d,aa,bb,cc,dd;
//    a=-1*x1+3*x2+(-3*x3)+1*x4;
//    b=3*x1+(-6*x2)+3*x3+0*x4;
//    c=-3*x1+3*x2;
//    d=x1;
//    //coefficients for y
//    aa=-1*y1+3*y2+(-3*y3)+1*y4;
//    bb=3*y1+(-6*y2)+3*y3+0*y4;
//    cc=-3*y1+3*y2;
//    dd=y1;
//
//
//    for (double t = 0.000; t <= 1.0; t+=0.001) {
//        double x,y;
//        x=a*(pow(t,3))+b*(t*t)+c*t+d;
//        y=aa*(pow(t,3))+bb*(t*t)+cc*t+dd;
//        SetPixel(hdc,x,y,color);
//    }
//
//}