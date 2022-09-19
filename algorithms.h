#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <cg3/geometry/segment2.h>
#include "data_structures/trapezoidalmap.h"
#include "data_structures/dag.h"

/**
 * @brief Algorithms
 */
namespace algorithms{
    void buildTrapezoidalMap();
    std::vector<Trapezoid> followSegment();

    size_t queryPoint(cg3::Point2d &q, Dag &dag, TrapezoidalMap &trapezoidalMap);
}

#endif // ALGORITHMS_H
