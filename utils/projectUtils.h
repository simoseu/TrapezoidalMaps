#ifndef PROJECTUTILS_H
#define PROJECTUTILS_H

#include "data_structures/trapezoid.h"
#include <cg3/utilities/color.h>

// Some utility function
namespace ProjectUtils{

void orderSegment(cg3::Segment2d &segment);

bool leftPointEqualTopLeftEndpoint(const Trapezoid &trapezoid);

bool rightPointEqualTopRightEndpoint(const Trapezoid &trapezoid);

bool leftPointEqualBottomLeftEndpoint(const Trapezoid &trapezoid);

bool rightPointEqualBottomRightEndpoint(const Trapezoid &trapezoid);

// Utility function to generate a random color
const cg3::Color randomColor();
}

#endif // PROJECTUTILS_H
