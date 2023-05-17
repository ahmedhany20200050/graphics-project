//
// Created by ahmed on 5/17/2023.
//

#ifndef GRAPHICS3_FILLING_H
#define GRAPHICS3_FILLING_H

#include<bits/stdc++.h>
#include "Circle.h"
#include "Line.h"
#include <cmath>
using namespace std;


typedef struct {
    int xLeft,xRight;
} table[1000];

class Point{
public:
    int x,y;
    explicit Point(int x=0,int y=0){
        this->x=x;
        this->y=y;
    }
};

struct EdgeRec
{
    double x;
    double minv;
    int ymax;
    // To order the the records based on the value of x
    bool operator<(EdgeRec r) const
    {
        return x<r.x;
    }
};
typedef list<EdgeRec> EdgeRecList;
int myFillingRound(double num) {
    return (int) round(num);
}
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
EdgeRec initializeEdgeRec(Point& point1,Point& point2)
{
    if(point1.y>point2.y)swap(point1,point2);
    EdgeRec rec{};
    rec.x=point1.x;
    rec.ymax=point2.y;
    rec.minv=(double)(point2.x-point1.x)/(point2.y-point1.y);
    return rec;
}
void initializeEdgeTable(Point points[],int n,EdgeRecList table[])
{
    Point point1=points[n-1];
    for(int i=0;i<n;i++)
    {
        Point point2=points[i];
        if(point1.y==point2.y){point1=point2;continue;}
        EdgeRec rec=initializeEdgeRec(point1, point2);
        table[point1.y].push_back(rec);
        point1=points[i];
    }
}

void performGeneralPolygonFilling(HDC hdc,Point points[],int n,COLORREF c){
    auto *table=new EdgeRecList [1000];
    initializeEdgeTable(points,n,table);
    int y=0;
    while(y<1000 && table[y].empty()){
        y++;
    }
    if(y==1000)return;
    EdgeRecList ActiveList=table[y];
    while (!ActiveList.empty())
    {
        ActiveList.sort();
        for(auto it=ActiveList.begin();it!=ActiveList.end();it++)
        {
            int x1=(int)ceil(it->x);
            it++;
            int x2=(int)floor(it->x);
            for(int x=x1;x<=x2;x++)SetPixel(hdc,x,y,c);
        }
        y++;
        auto itr=ActiveList.begin();
        while(itr!=ActiveList.end())
            if(y==itr->ymax) itr=ActiveList.erase(itr); else itr++;
        for(auto & it : ActiveList)
            it.x+=it.minv;
        ActiveList.insert(ActiveList.end(),table[y].begin(),table[y].end());
    }
    delete[] table;
}
void fillTheCircleWithLinesDraw8Points(HDC hdc,int xCenter,int yCenter,int fillingQuarter,int x,int y,COLORREF color){
    SetPixel(hdc, xCenter+x, yCenter+y, color);
    SetPixel(hdc, xCenter-x, yCenter+y, color);
    SetPixel(hdc, xCenter-x, yCenter-y, color);
    SetPixel(hdc, xCenter+x, yCenter-y, color);
    SetPixel(hdc, xCenter+y, yCenter+x, color);
    SetPixel(hdc, xCenter-y, yCenter+x, color);
    SetPixel(hdc, xCenter-y, yCenter-x, color);
    SetPixel(hdc, xCenter+y, yCenter-x, color);
    if(fillingQuarter==1){
        simpleDDA(hdc,xCenter+x, yCenter-y,xCenter,yCenter,color);
        simpleDDA(hdc,xCenter+y, yCenter-x,xCenter,yCenter,color);
    }else if(fillingQuarter==2){
        simpleDDA(hdc,xCenter-x, yCenter-y,xCenter,yCenter,color);
        simpleDDA(hdc,xCenter-y, yCenter-x,xCenter,yCenter,color);
    }else if(fillingQuarter==3){
        simpleDDA(hdc,xCenter-x, yCenter+y,xCenter,yCenter,color);
        simpleDDA(hdc,xCenter-y, yCenter+x,xCenter,yCenter,color);
    }else{
        simpleDDA(hdc,xCenter+x, yCenter+y,xCenter,yCenter,color);
        simpleDDA(hdc,xCenter+y, yCenter+x,xCenter,yCenter,color);
    }
}
void fillTheCircleWithLines(HDC hdc,int xCenter,int yCenter,int radius,int fillingQuarter,COLORREF color){
    int x=0,y=radius;
    int d=1-radius;
    int c1=3, c2=5-2*radius;
    fillTheCircleWithLinesDraw8Points(hdc,xCenter,yCenter,fillingQuarter,x,y,color);
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
        fillTheCircleWithLinesDraw8Points(hdc,xCenter,yCenter,fillingQuarter,x,y,color);
    }
}
bool checkIfThePointIsInOnlyOneQuarter(int x22,int y22,int x11,int y11,double radius,int fillingQuarter){
    int count=0;
    if(x22>x11 && x22<(x11+(int)radius) && y22<y11 && y22>y11-(int)radius && fillingQuarter==1){
        count++;
    }
    if(x22>(x11-(int)radius) && x22<x11 && y22 <y11 && y22>(y11-(int)radius)&& fillingQuarter==2){
        count++;
    }
    if(x22>(x11-(int)radius) && x22<x11 && y22>y11 && y22<(y11+(int)radius) && fillingQuarter==3){
        count++;
    }
    if(x22>x11 && x22<(x11+(int)radius) && y22>y11 && y22<(y11+(int)radius) && fillingQuarter==4){
        count++;
    }
    return (count==1);
}
void drawCircleInsideCircle(HDC hdc,int newX,int newY,int xCenter,int yCenter,COLORREF color,double radius,int fillingQuarter){
//    simpleDDA(hdc,xc+x,yc+y,xc,yc,RGB(0,0,255));
    int realXCenter = xCenter;
    int realYCenter = yCenter;
    int dx = xCenter - newX;
    int dy = yCenter - newY;

    // if slope < 1
    if (abs(dy) <= abs(dx)) {
        // x is independent
        // y is dependent

        if (newX > xCenter) {
            // if xs > xe then swap the two points
            mySwap(newX, xCenter);
            mySwap(newY, yCenter);
        }

        int x = newX;
        double y = newY;

        // calculate slope
        double m = (double) dy / dx;

        //increment x by 1 and increment y by change(m)
        while (x < xCenter) {
            x++;
            y += m;
            double r = min(sqrt(pow(abs(xCenter-x),2) + pow(abs(yCenter-myRound(y)),2)),
                           sqrt(pow(abs(newX-x),2) + pow(abs(newY-myRound(y)),2)));
            if(checkIfThePointIsInOnlyOneQuarter(x-(int)r,myRound(y),realXCenter,realYCenter,radius,fillingQuarter)
               && checkIfThePointIsInOnlyOneQuarter(x+(int)r,myRound(y),realXCenter,realYCenter,radius,fillingQuarter)
               &&checkIfThePointIsInOnlyOneQuarter(x,myRound(y)+(int)r,realXCenter,realYCenter,radius,fillingQuarter)
               &&checkIfThePointIsInOnlyOneQuarter(x,myRound(y)-(int)r,realXCenter,realYCenter,radius,fillingQuarter)){
                CircleFasterBresenham(hdc,x,myRound(y),(int)r,color);
            }
//            SetPixel(hdc,x,myRound(y),color);
        }
    } else if (abs(dy) > abs(dx)) {
        // if slope > 1
        // y is independent
        // x is dependent
        if (newY > yCenter) {
            // if ys > ye then swap the two points
            mySwap(newX, xCenter);
            mySwap(newY, yCenter);
        }

        int y = newY;
        double x = newX;

        // calculate perpendicular slope (m inverse)
        // mi = 1/m = dx/dy
        double mi = (double) dx / dy;

        // increment y by 1 and increment x by change(mi)

        while (y < yCenter) {
            y++;
            x += mi;
            double r = min(sqrt(pow(abs(xCenter- myRound(x)),2) + pow(abs(yCenter-y),2)),
                           sqrt(pow(abs(newX-myRound(x)),2) + pow(abs(newY-y),2)));
            if(checkIfThePointIsInOnlyOneQuarter(myRound(x)-(int)r,y,realXCenter,realYCenter,radius,fillingQuarter)
               && checkIfThePointIsInOnlyOneQuarter(myRound(x)+(int)r,y,realXCenter,realYCenter,radius,fillingQuarter)
               &&checkIfThePointIsInOnlyOneQuarter(myRound(x),y+(int)r,realXCenter,realYCenter,radius,fillingQuarter)
               &&checkIfThePointIsInOnlyOneQuarter(myRound(x),y-(int)r,realXCenter,realYCenter,radius,fillingQuarter)){
                CircleFasterBresenham(hdc,myRound(x),y,(int)r,color);
            }
//            SetPixel(hdc,myRound(x),y,color);
        }
    }
}
void fillTheQuarterOfTheCircle(HDC hdc,int xCenter,int yCenter,int fillingQuarter,int x,int y,COLORREF color,double radius){
//    SetPixel(hdc, xCenter+x, yCenter+y, color);
//    SetPixel(hdc, xCenter-x, yCenter+y, color);
//    SetPixel(hdc, xCenter-x, yCenter-y, color);
//    SetPixel(hdc, xCenter+x, yCenter-y, color);
//    SetPixel(hdc, xCenter+y, yCenter+x, color);
//    SetPixel(hdc, xCenter-y, yCenter+x, color);
//    SetPixel(hdc, xCenter-y, yCenter-x, color);
//    SetPixel(hdc, xCenter+y, yCenter-x, color);
    if(fillingQuarter==1){
        drawCircleInsideCircle(hdc,xCenter+x,yCenter-y,xCenter,yCenter,color,radius,1);
        drawCircleInsideCircle(hdc,xCenter+y,yCenter-x,xCenter,yCenter,color,radius,1);
    }else if(fillingQuarter==2){
        drawCircleInsideCircle(hdc,xCenter-x,yCenter-y,xCenter,yCenter,color,radius,2);
        drawCircleInsideCircle(hdc,xCenter-y,yCenter-x,xCenter,yCenter,color,radius,2);
    }else if(fillingQuarter==3){
        drawCircleInsideCircle(hdc,xCenter-x,yCenter+y,xCenter,yCenter,color,radius,3);
        drawCircleInsideCircle(hdc,xCenter-y,yCenter+x,xCenter,yCenter,color,radius,3);
    }else{
        drawCircleInsideCircle(hdc,xCenter+x,yCenter+y,xCenter,yCenter,color,radius,4);
        drawCircleInsideCircle(hdc,xCenter+y,yCenter+x,xCenter,yCenter,color,radius,4);
    }
}

void fillTheCircleWithCircles(HDC hdc,int xCenter,int yCenter,int radius,int fillingQuarter,COLORREF color){
    int x=0,y=radius;
    int d=1-radius;
    int c1=3, c2=5-2*radius;
    fillTheQuarterOfTheCircle(hdc,xCenter,yCenter,fillingQuarter,x,y,color,radius);
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
        fillTheQuarterOfTheCircle(hdc,xCenter,yCenter,fillingQuarter,x,y,color,radius);
    }
}

#endif //GRAPHICS3_FILLING_H
