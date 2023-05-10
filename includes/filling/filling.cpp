#include "filling.h"
#include<bits/stdc++.h>
using namespace std;
void performRecursiveFloodFillAlgorithm(HDC hdc,int x,int y,COLORREF borderColor,COLORREF fillColor){
    COLORREF color = GetPixel(hdc,x,y);
    if(color==borderColor || color==fillColor){
        return;
    }
    SetPixel(hdc,x,y,fillColor);
    performRecursiveFloodFillAlgorithm(hdc,x+1,y,borderColor,fillColor);
    performRecursiveFloodFillAlgorithm(hdc,x-1,y,borderColor,fillColor);
    performRecursiveFloodFillAlgorithm(hdc,x,y-1,borderColor,fillColor);
    performRecursiveFloodFillAlgorithm(hdc,x,y+1,borderColor,fillColor);
}
void performNonRecursiveFloodFillAlgorithm(HDC hdc,int x,int y,COLORREF borderColor,COLORREF fillColor){
    stack<Point> stk;
    stk.emplace(x,y);
    while(!stk.empty()){
        Point point = stk.top();
        stk.pop();
        COLORREF color = GetPixel(hdc,point.x,point.y);
        if(color==borderColor || color==fillColor){
            continue;
        }
        SetPixel(hdc,point.x,point.y,fillColor);
        stk.emplace(point.x-1,point.y);
        stk.emplace(point.x+1,point.y);
        stk.emplace(point.x,point.y-1);
        stk.emplace(point.x,point.y+1);
    }
}
void initTable(table t){
    for(int i=0;i<1000;i++){
        t[i].xLeft=INT_MAX;
        t[i].xRight= -INT_MAX;
    }
}
void swap(Point &p1,Point &p2){
    Point tmp=p1;
    p1=p2;
    p2=tmp;
}
void edgeToTable(Point p1,Point p2,table t){
    if(p1.y==p2.y)return;
    if(p1.y>p2.y)swap(p1,p2);
    double x = p1.x;
    int y=p1.y;
    double minv = (double)(p2.x-p1.x)/(p2.y-p1.y);
    while(y<p2.y){
        if(x<t[y].xLeft)t[y].xLeft=(int)ceil(x);
        if(x>t[y].xRight)t[y].xRight=(int)floor(x);
        y++;
        x+=minv;
    }
}
void polygonToTable(Point points[],int n,table t){
    Point v1 = points[n-1];
    for(int i=0;i<n;i++){
        Point v2 = points[i];
        edgeToTable(v1,v2,t);
        v1=points[i];
    }
}
void tableToScreen(HDC hdc,table t,COLORREF c){
    for(int i=0;i<1000;i++){
        if(t[i].xLeft<t[i].xRight){
            for(int x=t[i].xLeft;x<=t[i].xRight;x++){
                SetPixel(hdc,x,i,c);
            }
        }
    }
}
void performConvexFilling(HDC hdc,Point points[],int n, COLORREF c){
    table t;
    initTable(t);
    polygonToTable(points,n,t);
    tableToScreen(hdc,t,c);
}
