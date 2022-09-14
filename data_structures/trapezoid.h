#ifndef TRAPEZOID_H
#define TRAPEZOID_H

#include "node.h"
#include <cg3/geometry/segment2.h>

class Trapezoid{
private:
    //Node *node; Link to node of dag
    cg3::Segment2d top;
    cg3::Segment2d bottom;
    cg3::Point2d leftPoint;
    cg3::Point2d rightPoint;

public:
   Trapezoid(cg3::Segment2d top, cg3::Segment2d bottom, cg3::Point2d leftPoint, cg3::Point2d rightPoint);

   cg3::Segment2d getTop();
   cg3::Segment2d getBottom();
   cg3::Point2d getLeftPoint();
   cg3::Point2d getRightPoint();

};

#endif // TRAPEZOID_H
