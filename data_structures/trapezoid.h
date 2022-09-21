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

    const cg3::Segment2d &getTopSegment() const;
    const cg3::Segment2d &getBottomSegment() const;
    const cg3::Point2d &getLeftPoint() const;
    const cg3::Point2d &getRightPoint() const;
    size_t getUpperLeftNeighbor() const;
    size_t getLowerLeftNeighbor() const;
    size_t getUpperRightNeighbor() const;
    size_t getLowerRightNeighbor() const;

private:
    //Node *node; Link to node of dag
    cg3::Segment2d topSegment;
    cg3::Segment2d bottomSegment;
    cg3::Point2d leftPoint;
    cg3::Point2d rightPoint;

    // Storing adjacent trapezoid with their indexes
    size_t upperLeftNeighbor, lowerLeftNeighbor, upperRightNeighbor, lowerRightNeighbor;

};

#endif // TRAPEZOID_H
