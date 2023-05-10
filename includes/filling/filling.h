#ifndef GRAPHICS_PROJECT_FILLING_H
#define GRAPHICS_PROJECT_FILLING_H
#include <windows.h>
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
void performRecursiveFloodFillAlgorithm(HDC hdc,int x,int y,COLORREF borderColor,COLORREF fillColor);
void performNonRecursiveFloodFillAlgorithm(HDC hdc,int x,int y,COLORREF borderColor,COLORREF fillColor);
void performConvexFilling(HDC hdc,Point points[],int n, COLORREF c);

#endif //GRAPHICS_PROJECT_FILLING_H
