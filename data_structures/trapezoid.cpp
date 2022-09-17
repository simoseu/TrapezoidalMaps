#include "trapezoid.h"

Trapezoid::Trapezoid(cg3::Segment2d topSegment, cg3::Segment2d bottomSegment, cg3::Point2d leftPoint, cg3::Point2d rightPoint){
    this->topSegment = topSegment;
    this->bottomSegment = bottomSegment;
    this->leftPoint = leftPoint;
    this->rightPoint = rightPoint;
}

cg3::Segment2d Trapezoid::getTopSegment(){
    return topSegment;
}

cg3::Segment2d Trapezoid::getBottomSegment(){
    return bottomSegment;
}

cg3::Point2d Trapezoid::getLeftPoint(){
    return leftPoint;
}

cg3::Point2d Trapezoid::getRightPoint(){
    return rightPoint;
}

size_t Trapezoid::getUpperLeftNeighbor(){
    return upperLeftNeighbor;
}

size_t Trapezoid::getLowerLeftNeighbor(){
    return lowerLeftNeighbor;
}

size_t Trapezoid::getUpperRightNeighbor(){
    return upperRightNeighbor;
}

size_t Trapezoid::getLowerRightNeighbor(){
    return lowerRightNeighbor;
}

