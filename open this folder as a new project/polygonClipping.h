//
// Created by ahmed on 5/15/2023.
//

#ifndef GRAPHICS3_POLYGONCLIPPING_H
#define GRAPHICS3_POLYGONCLIPPING_H
#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif
#include<bits/stdc++.h>
#include <tchar.h>
#include <windows.h>
using namespace std;
/*  Declare Windows procedure  */
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

struct vertex{
    double x,y;
    vertex(int x1 = 0, int y1 = 0){
        x = x1;
        y = y1;
    }
};
typedef bool (*IsInFunc)(vertex& v,int edge);
typedef vertex (*IntersectFunc)(vertex& v1,vertex& v2,int edge);
bool InLeft(vertex &v, int edge){
    return v.x>=edge;
}
bool InRight(vertex &v,int edge){
    return v.x<=edge;
}
bool InTop(vertex &v,int edge){
    return v.y>=edge;
}
bool InBottom(vertex &v, int edge){
    return v.y<=edge;
}
vertex vIntersect(vertex &v1,vertex &v2,int xedge){
    vertex ans;
    ans.x=xedge;
    ans.y=v1.y+(xedge-v1.x)*(v2.y-v1.y)/(v2.x-v1.y);
    return ans;
}
vertex hIntersect(vertex &v1,vertex &v2,int yedge){
    vertex ans;
    ans.y=yedge;
    ans.x=v1.x+(yedge-v1.y)*(v2.x-v1.x)/(v2.y-v1.y);
    return ans;
}
vector<vertex> clipWithEdge(vector<vertex> points,int edge,IsInFunc In,IntersectFunc Intersect){
    vector<vertex> outList;
    vertex v1 = points[points.size()-1];
    bool v1_in = In(v1,edge);
    for (int i=0;i<points.size();i++){
        vertex v2 = points[i];
        bool v2_in = In(v2,edge);
        if (!v1_in&&v2_in){
            outList.push_back(Intersect(v1,v2,edge));
            outList.push_back(v2);
        }
        else if(v1_in&&v2_in){
            outList.push_back(v2);
        }
        else if(v1_in){
            outList.push_back(Intersect(v1,v2,edge));
        }
        v1 = v2;
        v1_in = v2_in;
    }
    return outList;
}
void polyGonClipping(HDC hdc,vector<vertex>list,int xleft,int ytop,int xright,int ybottom,COLORREF c){

    vector<vertex>vlist=list;
//    for (int i=0;i<n;i++)
//        vlist.push_back(vertex(p[i].x,p[i].y));
    vlist=clipWithEdge(vlist,xleft,InLeft,vIntersect);
    vlist=clipWithEdge(vlist,xright,InRight,vIntersect);
    vlist=clipWithEdge(vlist,ytop,InTop,hIntersect);
    vlist=clipWithEdge(vlist,ybottom,InBottom,hIntersect);

    vertex v1 = vlist[vlist.size()-1];
    for (int i=0;i<(int)vlist.size();i++){
        vertex v2 = vlist[i];
        simpleDDA(hdc,v1.x,v1.y,v2.x,v2.y,c);
        v1 = v2;
    }
}
//LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
//
///*  Make the class name into a global variable  */
//TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");
//
//int WINAPI WinMain (HINSTANCE hThisInstance,
//                    HINSTANCE hPrevInstance,
//                    LPSTR lpszArgument,
//                    int nCmdShow)
//{
//    HWND hwnd;               /* This is the handle for our window */
//    MSG messages;            /* Here messages to the application are saved */
//    WNDCLASSEX wincl;        /* Data structure for the windowclass */
//
//    /* The Window structure */
//    wincl.hInstance = hThisInstance;
//    wincl.lpszClassName = szClassName;
//    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
//    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
//    wincl.cbSize = sizeof (WNDCLASSEX);
//
//    /* Use default icon and mouse-pointer */
//    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
//    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
//    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
//    wincl.lpszMenuName = NULL;                 /* No menu */
//    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
//    wincl.cbWndExtra = 0;                      /* structure or the window instance */
//    /* Use Windows's default colour as the background of the window */
//    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;
//
//    /* Register the window class, and if it fails quit the program */
//    if (!RegisterClassEx (&wincl))
//        return 0;
//
//    /* The class is registered, let's create the program*/
//    hwnd = CreateWindowEx (
//            0,                   /* Extended possibilites for variation */
//            szClassName,         /* Classname */
//            _T("Code::Blocks Template Windows App"),       /* Title Text */
//            WS_OVERLAPPEDWINDOW, /* default window */
//            CW_USEDEFAULT,       /* Windows decides the position */
//            CW_USEDEFAULT,       /* where the window ends up on the screen */
//            544,                 /* The programs width */
//            375,                 /* and height in pixels */
//            HWND_DESKTOP,        /* The window is a child-window to desktop */
//            NULL,                /* No menu */
//            hThisInstance,       /* Program Instance handler */
//            NULL                 /* No Window Creation data */
//    );
//
//    /* Make the window visible on the screen */
//    ShowWindow (hwnd, nCmdShow);
//
//    /* Run the message loop. It will run until GetMessage() returns 0 */
//    while (GetMessage (&messages, NULL, 0, 0))
//    {
//        /* Translate virtual-key messages into character messages */
//        TranslateMessage(&messages);
//        /* Send message to WindowProcedure */
//        DispatchMessage(&messages);
//    }
//
//    /* The program return-value is 0 - The value that PostQuitMessage() gave */
//    return messages.wParam;
//}
//
//
///*  This function is called by the Windows function DispatchMessage()  */
//static int  xL,yT,xR,yB;
//static int x,y;
//static int counter = 0;
//static Point ps[5];
//LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    //how this code works
//    //first we get the rectangle top left then bottom right points
//    //after that we take 5 points of the polygon
//    HDC hdc=GetDC(hwnd);
//    switch (message)                  /* handle the messages*/
//    {
//        case WM_LBUTTONDOWN:
//            if (counter == 0){
//                xL = LOWORD(lParam);
//                yT = HIWORD(lParam);
//                counter++;
//            }
//            else if (counter == 1){
//                xR = LOWORD(lParam);
//                yB = HIWORD(lParam);
//                simpleDDA(hdc,xL,yT,xL,yB,RGB(255,0,0));
//                simpleDDA(hdc,xL,yT,xR,yT,RGB(255,0,0));
//                simpleDDA(hdc,xL,yB,xR,yB,RGB(255,0,0));
//                simpleDDA(hdc,xR,yT,xR,yB,RGB(255,0,0));
//                ReleaseDC(hwnd,hdc);
//                counter++;
//            }
//            else if(counter>=2&&counter<7){
//                ps[counter-2].x=LOWORD(lParam);
//                ps[counter-2].y=HIWORD(lParam);
//                counter++;
//                if (counter==7){
//                    counter = 0;
//                    polyGonClipping(hdc,ps,5,xL,yT,xR,yB,RGB(255,0,0));
//                    ReleaseDC(hwnd,hdc);
//                }
//            }
//
//            break;
//        case WM_DESTROY:
//            PostQuitMessage (0);       /* send a WMQUIT to the message queue*/
//            break;
//        default:                      /* for messages that we don't deal with*/
//            return DefWindowProc (hwnd, message, wParam, lParam);
//    }
//
//    return 0;
//}

#endif //GRAPHICS3_POLYGONCLIPPING_H
