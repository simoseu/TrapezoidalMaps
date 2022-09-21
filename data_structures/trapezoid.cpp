#include "trapezoid.h"

Trapezoid::Trapezoid(cg3::Segment2d topSegment, cg3::Segment2d bottomSegment, cg3::Point2d leftPoint, cg3::Point2d rightPoint,
                     size_t upperLeftNeighbor, size_t lowerLeftNeighbor, size_t upperRightNeighbor, size_t lowerRightNeighbor,
                     size_t nodeIdx) :
    topSegment(topSegment), bottomSegment(bottomSegment), leftPoint(leftPoint), rightPoint(rightPoint),
    upperLeftNeighbor(upperLeftNeighbor), lowerLeftNeighbor(lowerLeftNeighbor), upperRightNeighbor(upperRightNeighbor), lowerRightNeighbor(lowerRightNeighbor),
    nodeIdx(nodeIdx)
{

}

void Trapezoid::setTopSegment(cg3::Segment2d s){
    topSegment = s;
}

const cg3::Segment2d &Trapezoid::getTopSegment() const{
    return topSegment;
}

void Trapezoid::setBottomSegment(cg3::Segment2d s){
    bottomSegment = s;
}

const cg3::Segment2d &Trapezoid::getBottomSegment() const{
    return bottomSegment;
}

void Trapezoid::setLeftPoint(cg3::Point2d p){
    leftPoint = p;
}

const cg3::Point2d &Trapezoid::getLeftPoint() const{
    return leftPoint;
}

void Trapezoid::setRightPoint(cg3::Point2d p){
    rightPoint = p;
}

const cg3::Point2d &Trapezoid::getRightPoint() const{
    return rightPoint;
}

void Trapezoid::setUpperLeftNeighbor(size_t idx){
    upperLeftNeighbor = idx;
}

size_t Trapezoid::getUpperLeftNeighbor() const{
    return upperLeftNeighbor;
}

void Trapezoid::setLowerLeftNeighbor(size_t idx){
    lowerLeftNeighbor = idx;
}

size_t Trapezoid::getLowerLeftNeighbor() const{
    return lowerLeftNeighbor;
}

void Trapezoid::setUpperRightNeigbor(size_t idx){
    upperRightNeighbor = idx;
}

size_t Trapezoid::getUpperRightNeighbor() const{
    return upperRightNeighbor;
}

void Trapezoid::setLowerRightNeighbor(size_t idx){
    lowerRightNeighbor = idx;
}

size_t Trapezoid::getLowerRightNeighbor() const{
    return lowerRightNeighbor;
}

void Trapezoid::setNodeIdx(size_t idx){
    nodeIdx = idx;
}

size_t Trapezoid::getNodeIdx() const{
    return nodeIdx;
}


