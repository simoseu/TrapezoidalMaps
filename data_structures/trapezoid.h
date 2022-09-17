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

    cg3::Segment2d getTopSegment();
    cg3::Segment2d getBottomSegment();
    cg3::Point2d getLeftPoint();
    cg3::Point2d getRightPoint();
    size_t getUpperLeftNeighbor();
    size_t getLowerLeftNeighbor();
    size_t getUpperRightNeighbor();
    size_t getLowerRightNeighbor();

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
