//
// Created by ahmed on 5/15/2023.
//

#ifndef GRAPHICS2_2D_TRANSFORMATION_H
#define GRAPHICS2_2D_TRANSFORMATION_H
#include "bits/stdc++.h"
using namespace std;

vector<pair<int,int>> translateLine(vector<pair<int,int>>points,int translateX,int translateY){
    vector<pair<int,int>> newPoints;
    newPoints.emplace_back(points[0].first+translateX,points[0].second+translateY);
    newPoints.emplace_back(points[1].first+translateX,points[1].second+translateY);
    return newPoints;
}

vector<pair<int,int>> scaleLine(vector<pair<int,int>>points,double scaleX,double scaleY){
    vector<pair<int,int>> newPoints;
    newPoints.emplace_back(points[0].first*scaleX,points[0].second*scaleY);
    newPoints.emplace_back(points[1].first*scaleX,points[1].second*scaleY);
    return newPoints;
}

#endif //GRAPHICS2_2D_TRANSFORMATION_H
