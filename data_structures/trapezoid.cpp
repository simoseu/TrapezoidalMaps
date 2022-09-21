#include "trapezoid.h"

Trapezoid::Trapezoid(cg3::Segment2d topSegment, cg3::Segment2d bottomSegment, cg3::Point2d leftPoint, cg3::Point2d rightPoint){
    this->topSegment = topSegment;
    this->bottomSegment = bottomSegment;
    this->leftPoint = leftPoint;
    this->rightPoint = rightPoint;
}

const cg3::Segment2d &Trapezoid::getTopSegment() const{
    return topSegment;
}

const cg3::Segment2d &Trapezoid::getBottomSegment() const{
    return bottomSegment;
}

const cg3::Point2d &Trapezoid::getLeftPoint() const{
    return leftPoint;
}

const cg3::Point2d &Trapezoid::getRightPoint() const{
    return rightPoint;
}

size_t Trapezoid::getUpperLeftNeighbor() const{
    return upperLeftNeighbor;
}

size_t Trapezoid::getLowerLeftNeighbor() const{
    return lowerLeftNeighbor;
}

size_t Trapezoid::getUpperRightNeighbor() const{
    return upperRightNeighbor;
}

size_t Trapezoid::getLowerRightNeighbor() const{
    return lowerRightNeighbor;
}

