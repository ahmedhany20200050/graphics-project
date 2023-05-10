#ifndef GRAPHICS_PROJECT_FILLING_H
#define GRAPHICS_PROJECT_FILLING_H
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
#endif //GRAPHICS_PROJECT_FILLING_H
