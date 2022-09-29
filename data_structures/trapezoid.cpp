#include "trapezoid.h"

Trapezoid::Trapezoid(cg3::Segment2d topSegment, cg3::Segment2d bottomSegment, cg3::Point2d leftPoint, cg3::Point2d rightPoint) :
    topSegment(topSegment), bottomSegment(bottomSegment), leftPoint(leftPoint), rightPoint(rightPoint)
{

}

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

// Return the corners of the trapezoid (in clockwise order)
// Needed to draw the trapezoids
const std::vector<cg3::Point2d> Trapezoid::getCorners() const{
    std::vector<cg3::Point2d> corners;
    double topSegmentSlope = (topSegment.p2().y() - topSegment.p1().y()) / (topSegment.p2().x() - topSegment.p1().x());
    double bottomSegmentSlope = (bottomSegment.p2().y() - bottomSegment.p1().y()) / (bottomSegment.p2().x() - bottomSegment.p1().x());

    corners.push_back(cg3::Point2d(leftPoint.x(), (topSegmentSlope)*(leftPoint.x() - topSegment.p1().x()) + topSegment.p1().y() )); // Upper left corner
    corners.push_back(cg3::Point2d(rightPoint.x(), - (topSegmentSlope)*(topSegment.p2().x() - rightPoint.x()) + topSegment.p2().y() )); // Upper right corner
    corners.push_back(cg3::Point2d(rightPoint.x(), - (bottomSegmentSlope)*(bottomSegment.p2().x() - rightPoint.x()) + bottomSegment.p2().y() )); // Upper left corner
    corners.push_back(cg3::Point2d(leftPoint.x(), (bottomSegmentSlope)*(leftPoint.x() - bottomSegment.p1().x()) + bottomSegment.p1().y() )); // Upper left corner

    return corners;
}


