#include "trapezoid.h"

Trapezoid::Trapezoid(cg3::Segment2d top, cg3::Segment2d bottom, cg3::Point2d leftPoint, cg3::Point2d rightPoint){
    this->topSegment = top;
    this->bottomSegment = bottom;

    this->leftPoint = leftPoint;
    this->rightPoint = rightPoint;
}

cg3::Segment2d Trapezoid::getTop(){
    return topSegment;
}

cg3::Segment2d Trapezoid::getBottom(){
    return bottomSegment;
}

cg3::Point2d Trapezoid::getLeftPoint(){
    return leftPoint;
}

cg3::Point2d Trapezoid::getRightPoint(){
    return rightPoint;
}

size_t Trapezoid::getUpperLeftTrapezoid(){
    return upperLeftTrapezoid;
}

size_t Trapezoid::getLowerLeftTrapezoid(){
    return lowerLeftTrapezoid;
}

size_t Trapezoid::getUpperRightTrapezoid(){
    return upperRightTrapezoid;
}

size_t Trapezoid::getLowerRightTrapezoid(){
    return lowerRightTrapezoid;
}

