#include "trapezoid.h"

/**
 * @brief Constructor
 * @param[in] topSegment the top edge of the trapezoid
 * @param[in] bottomSegment the bottom edge of the trapezoid
 * @param[in] leftPoint the left point of the trapezoid
 * @param[in] rightPoint the right point of the trapezoid
*/
Trapezoid::Trapezoid(cg3::Segment2d topSegment, cg3::Segment2d bottomSegment, cg3::Point2d leftPoint, cg3::Point2d rightPoint) :
    topSegment(topSegment), bottomSegment(bottomSegment), leftPoint(leftPoint), rightPoint(rightPoint)
{

}

/**
 * @brief Constructor
 * @param[in] topSegment the top edge of the trapezoid
 * @param[in] bottomSegment the bottom edge of the trapezoid
 * @param[in] leftPoint the left point of the trapezoid
 * @param[in] rightPoint the right point of the trapezoid
 * @param[in] upperLeftNeighbor the index of the upper left neighbor
 * @param[in] lowerLeftNeighbor the index of the lower left neighbor
 * @param[in] upperRightNeighbor the index of the upper right neighbor
 * @param[in] lowerRightNeighbor the index of the lower right neighbor
 * @param[in] nodeIdx the index of the dag that represent the trapezoid
*/
Trapezoid::Trapezoid(cg3::Segment2d topSegment, cg3::Segment2d bottomSegment, cg3::Point2d leftPoint, cg3::Point2d rightPoint,
                     size_t upperLeftNeighbor, size_t lowerLeftNeighbor, size_t upperRightNeighbor, size_t lowerRightNeighbor,
                     size_t nodeIdx) :
    topSegment(topSegment), bottomSegment(bottomSegment), leftPoint(leftPoint), rightPoint(rightPoint),
    upperLeftNeighbor(upperLeftNeighbor), lowerLeftNeighbor(lowerLeftNeighbor), upperRightNeighbor(upperRightNeighbor), lowerRightNeighbor(lowerRightNeighbor),
    nodeIdx(nodeIdx)
{

}

/**
 * @brief Set the top edge of the trapezoid
 * @param[in] s the segment representing the top edge
*/
void Trapezoid::setTopSegment(cg3::Segment2d s){
    topSegment = s;
}

/**
 * @brief get the top edge of the trapezoid
 * @return the segment representing the top edge
*/
const cg3::Segment2d &Trapezoid::getTopSegment() const{
    return topSegment;
}

/**
 * @brief Set the bottom edge of the trapezoid
 * @param[in] s the segment representing the bottom edge
*/
void Trapezoid::setBottomSegment(cg3::Segment2d s){
    bottomSegment = s;
}

/**
 * @brief Set the bottom edge of the trapezoid
 * @return the segment representing the bottom edge
*/
const cg3::Segment2d &Trapezoid::getBottomSegment() const{
    return bottomSegment;
}

/**
 * @brief Set the left point of the trapezoid
 * @param[in] p the point representing the left point
*/
void Trapezoid::setLeftPoint(cg3::Point2d p){
    leftPoint = p;
}

/**
 * @brief Get the left point of the trapezoid
 * @return the point representing the left point
*/
const cg3::Point2d &Trapezoid::getLeftPoint() const{
    return leftPoint;
}

/**
 * @brief Set the right point of the trapezoid
 * @param[in] p the point representing the right point
*/
void Trapezoid::setRightPoint(cg3::Point2d p){
    rightPoint = p;
}

/**
 * @brief Set the right point of the trapezoid
 * @return the point representing the right point
*/
const cg3::Point2d &Trapezoid::getRightPoint() const{
    return rightPoint;
}

/**
 * @brief Set the upper left neighbor of the trapezoid
 * @param[in] idx the index representing the upper left neighbor
*/
void Trapezoid::setUpperLeftNeighbor(size_t idx){
    upperLeftNeighbor = idx;
}

/**
 * @brief Get the upper left neighbor of the trapezoid
 * @return the index representing the upper left neighbor
*/
size_t Trapezoid::getUpperLeftNeighbor() const{
    return upperLeftNeighbor;
}

/**
 * @brief Set the lower left neighbor of the trapezoid
 * @param[in] idx the index representing the lower left neighbor
*/
void Trapezoid::setLowerLeftNeighbor(size_t idx){
    lowerLeftNeighbor = idx;
}

/**
 * @brief Get the lower left neighbor of the trapezoid
 * @return the index representing the lower left neighbor
*/
size_t Trapezoid::getLowerLeftNeighbor() const{
    return lowerLeftNeighbor;
}

/**
 * @brief Set the upper right neighbor of the trapezoid
 * @param[in] idx the index representing the upper right neighbor
*/
void Trapezoid::setUpperRightNeigbor(size_t idx){
    upperRightNeighbor = idx;
}

/**
 * @brief Get the upper right neighbor of the trapezoid
 * @return the index representing the upper right neighbor
*/
size_t Trapezoid::getUpperRightNeighbor() const{
    return upperRightNeighbor;
}

/**
 * @brief Set the lower right neighbor of the trapezoid
 * @param[in] idx the index representing the lower right neighbor
*/
void Trapezoid::setLowerRightNeighbor(size_t idx){
    lowerRightNeighbor = idx;
}

/**
 * @brief Get the lower right neighbor of the trapezoid
 * @return the index representing the lower right neighbor
*/
size_t Trapezoid::getLowerRightNeighbor() const{
    return lowerRightNeighbor;
}

/**
 * @brief Set the index of the dag that represent the trapezoid
 * @param[in] idx the index of the dag that represent the trapezoid
*/
void Trapezoid::setNodeIdx(size_t idx){
    nodeIdx = idx;
}

/**
 * @brief Get the index of the dag that represent the trapezoid
 * @return the index of the dag that represent the trapezoid
*/
size_t Trapezoid::getNodeIdx() const{
    return nodeIdx;
}

/**
 * @brief Return the corners of the trapezoid (in clockwise order)
 * @return a vector containing the corners of the trapezoid in clockwise order
 * Needed for the drawing
*/
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


