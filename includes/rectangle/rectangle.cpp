//
// Created by metr on 5/15/2023.
//

#include "rectangle.h"
#include "../../Line.h"
#include "../hermit/hermit.h"
#include "../../beziar.h"
void drawSquareWithHermit(HDC hdc,int x,int y,COLORREF borderColor){
    simpleDDA(hdc,x,y,x+100,y,borderColor);
    simpleDDA(hdc,x+100,y,x+100,y+100,borderColor);
    simpleDDA(hdc,x+100,y+100,x,y+100,borderColor);
    simpleDDA(hdc,x,y+100,x,y,borderColor);
    for(int i=0;i<=100;i+=1){
        performHermiteCurveAlgorithm(hdc,x+i,y,40,20,x+i,y+100,30,10, RGB(0,0,255));
    }
}
void drawRectangleWithBezier(HDC hdc,int x,int y,COLORREF borderColor){
    simpleDDA(hdc,x,y,x+100,y,borderColor);
    simpleDDA(hdc,x+100,y,x+100,y+50,borderColor);
    simpleDDA(hdc,x+100,y+50,x,y+50,borderColor);
    simpleDDA(hdc,x,y+50,x,y,borderColor);

    for(int i=0;i<=50;i+=1){
        bezair(hdc,x,y+i,x+50,y,x+50,y+50,x+100,y+i, RGB(0,0,255));
    }
}