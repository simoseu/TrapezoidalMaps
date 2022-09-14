#include "trapezoid.h"

Trapezoid::Trapezoid(cg3::Segment2d top, cg3::Segment2d bottom, cg3::Point2d leftPoint, cg3::Point2d rightPoint){
    this->top = top;
    this->bottom = bottom;

    this->leftPoint = leftPoint;
    this->rightPoint = rightPoint;
}

cg3::Segment2d Trapezoid::getTop(){
    return top;
}

cg3::Segment2d Trapezoid::getBottom(){
    return bottom;
}

cg3::Point2d Trapezoid::getLeftPoint(){
    return leftPoint;
}

cg3::Point2d Trapezoid::getRightPoint(){
    return rightPoint;
}
