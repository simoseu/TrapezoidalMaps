#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <cg3/geometry/segment2.h>
#include "data_structures/trapezoidalmap.h"
/**
 * @brief Algorithms
 */
namespace algorithms{
    void buildTrapezoidalMap();
    std::vector<Trapezoid> followSegment();
}

#endif // ALGORITHMS_H
