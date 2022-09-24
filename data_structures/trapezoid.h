#ifndef TRAPEZOID_H
#define TRAPEZOID_H

#include "node.h"
#include <cg3/geometry/segment2.h>

/**
 * @brief This class define the Trapezoid data structure.
 * It stores the top and the bottom edge of the trapezoid, and the left and the right point.
 * Also stores the index of adjacent trapezoids.
 */
class Trapezoid{

public:
    Trapezoid(cg3::Segment2d topSegment, cg3::Segment2d bottomSegment, cg3::Point2d leftPoint, cg3::Point2d rightPoint);
    Trapezoid(cg3::Segment2d topSegment, cg3::Segment2d bottomSegment, cg3::Point2d leftPoint, cg3::Point2d rightPoint,
              size_t upperLeftNeighbor, size_t lowerLeftNeighbor, size_t upperRightNeighbor, size_t lowerRightNeighbor,
              size_t nodeIdx);

    void setTopSegment(cg3::Segment2d s);
    const cg3::Segment2d &getTopSegment() const;
    void setBottomSegment(cg3::Segment2d s);
    const cg3::Segment2d &getBottomSegment() const;
    void setLeftPoint(cg3::Point2d p);
    const cg3::Point2d &getLeftPoint() const;
    void setRightPoint(cg3::Point2d p);
    const cg3::Point2d &getRightPoint() const;

    void setUpperLeftNeighbor(size_t idx);
    size_t getUpperLeftNeighbor() const;
    void setLowerLeftNeighbor(size_t idx);
    size_t getLowerLeftNeighbor() const;
    void setUpperRightNeigbor(size_t idx);
    size_t getUpperRightNeighbor() const;
    void setLowerRightNeighbor(size_t idx);
    size_t getLowerRightNeighbor() const;

    void setNodeIdx(size_t idx);
    size_t getNodeIdx() const;

private:
    cg3::Segment2d topSegment;
    cg3::Segment2d bottomSegment;
    cg3::Point2d leftPoint;
    cg3::Point2d rightPoint;

    // Storing adjacent trapezoid with their indexes
    size_t upperLeftNeighbor, lowerLeftNeighbor, upperRightNeighbor, lowerRightNeighbor;
    size_t nodeIdx; // Link to node of dag storing its idx

};

#endif // TRAPEZOID_H
