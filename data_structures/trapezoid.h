#ifndef TRAPEZOID_H
#define TRAPEZOID_H

#include "node.h"
#include <cg3/geometry/segment2.h>

class Trapezoid{
private:
    //Node *node; Link to node of dag
    cg3::Segment2d topSegment;
    cg3::Segment2d bottomSegment;
    cg3::Point2d leftPoint;
    cg3::Point2d rightPoint;

    // Storing adjacent trapezoid with theri indexes
    size_t upperLeftTrapezoid, lowerLeftTrapezoid, upperRightTrapezoid, lowerRightTrapezoid;

public:
    Trapezoid(cg3::Segment2d top, cg3::Segment2d bottom, cg3::Point2d leftPoint, cg3::Point2d rightPoint);

    cg3::Segment2d getTop();
    cg3::Segment2d getBottom();
    cg3::Point2d getLeftPoint();
    cg3::Point2d getRightPoint();
    size_t getUpperLeftTrapezoid();
    size_t getLowerLeftTrapezoid();
    size_t getUpperRightTrapezoid();
    size_t getLowerRightTrapezoid();

};

#endif // TRAPEZOID_H
