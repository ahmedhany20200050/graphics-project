
#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include "iostream"
#include "beziar.cpp"
#include "Circle.cpp"
#include "2d_transformation.cpp"
//#include <CommCtrl.h>
#include <cstring>
#include<bits/stdc++.h>
#include <dshow.h>
//#include <windowsx.h>
//#include <QComboBox>
//#include <QStringList>
using namespace std;
union OutCode{
  unsigned All: 4;
  struct{
      unsigned left:1,top:1,right:1,bottom:1;
  };
};
OutCode GetOutCode(double x,double y,int xleft,int ytop,int xright,int ybottom){
    OutCode out;
    out.All=0;
    if(x<xleft)out.left=1;else if(x>xright)out.right=1;
    if(y<ytop)out.top=1;else if(y>ybottom)out.bottom=1;
    return  out;
}
void VIntersect(double xs,double ys,double xe,double ye,int x,double*xi,double *yi){
    *xi=x;
    *yi=ys+(x-xs)*(ye-ys)/(xe-xs);
}
void HIntersect(double xs,double ys,double xe,double ye,int y,double*xi,double *yi){
    *yi=y;
    *xi=xs+(y-ys)*(xe-xs)/(ye-ys);
}

void CohenSuth(HDC hdc,int xs,int ys,int xe,int ye,int xleft,int ytop,int xright,int ybottom){
    double x1=xs,y1=ys,x2=xe,y2=ye;
    OutCode out1=GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
    OutCode out2=GetOutCode(x2,y2,xleft,ytop,xright,ybottom);
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
        } else
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
    if(!out1.All && !out2.All)
    {
        MoveToEx(hdc,round(x1),round(y1),NULL);
        LineTo(hdc,round(x2),round(y2));
    }
}

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
static int x11,x2,x3,x4;
static int y11,y2,y3,y4,counter=0;
static int xfirst,xend,yfirst,yend;
static string choice="none";
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    //x2 +y2 =r2
    HDC hdc=GetDC(hwnd);
    //we go depending on choice
    if(choice=="fucking bezier"){
        switch(message){
            case WM_LBUTTONDOWN:
                if(counter==0){
                    x11= LOWORD(lParam);
                    y11= HIWORD(lParam);
                    counter++;
                } else if(counter==1){
                    x2= LOWORD(lParam);
                    y2= HIWORD(lParam);
                    x3=x2;
                    y3=y2+50;
                    x4=x11;
                    y4=y11+50;
                    simpleDDA(hdc,x11,y11,x2,y2,RGB(255,0,0));
                    simpleDDA(hdc,x2,y2,x3,y4,RGB(255,0,0));
                    simpleDDA(hdc,x3,y3,x4,y4,RGB(255,0,0));
                    simpleDDA(hdc,x4,y4,x11,y11,RGB(255,0,0));
                    counter++;
                }else if(counter==2){
                    xfirst= LOWORD(lParam);
                    yfirst= HIWORD(lParam);
                    counter++;
                }else if(counter==3){
                    xend= LOWORD(lParam);
                    yend= HIWORD(lParam);
                    CohenSuth( hdc, xfirst, yfirst, xend, yend,x11, y11, x2, y11+50);
                    counter=0;
                }

                break;
        }

    } else if(choice=="dda line"){
        // do line drawing things
    }

    //combobox things
    switch (message)                  /* handle the messages*/
    {

        case WM_CREATE:
            CreateWindowEx(WS_EX_STATICEDGE, "COMBOBOX", "MyCombo1",
                           CBS_DROPDOWN | WS_CHILD | WS_VISIBLE,
                           0, 0, 200, 200, hwnd, reinterpret_cast<HMENU>(100), reinterpret_cast<HINSTANCE>(hwnd), NULL); // 100 = ID of this control
            ComboBox_AddItemData(GetDlgItem(hwnd, 100),"fucking bezier");
            ComboBox_AddItemData(GetDlgItem(hwnd, 100),"fucking nigga");
            ComboBox_AddItemData(GetDlgItem(hwnd, 100),"fucking hermit");


            break;
        case WM_COMMAND:
            if(HIWORD(wParam)== CBN_CLOSEUP){
                counter=0;
                char  *s= (char*) malloc(sizeof(char));
                GetWindowTextA(GetDlgItem(hwnd, 100),s,55);
                string ss;
                cout<<"we are good\n";
                for (int i = 0; i < 20; ++i) {
                    cout<<s[i];
                    if(s[i]=='\0')break;
                    ss+=s[i];
                }
                cout<<"\n";
                choice=ss;
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