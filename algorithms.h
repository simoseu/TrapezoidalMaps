#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <cg3/geometry/segment2.h>
#include "data_structures/trapezoidalmap_dataset.h"
#include "data_structures/trapezoidalmap.h"
#include "data_structures/dag.h"

/**
 * @brief Algorithms
 */
namespace algorithms{
    void initializeStructures(Dag &dag, TrapezoidalMap &trapezoidalMap);

    void buildTrapezoidalMap(const cg3::Segment2d &segment, Dag &dag, TrapezoidalMap &trapezoidalMap, TrapezoidalMapDataset &trapezoidalMapData);

    std::vector<size_t> followSegment(const cg3::Segment2d &segment, const Dag &dag, const TrapezoidalMap &trapezoidalMap, const TrapezoidalMapDataset &trapezoidalMapData);

    size_t queryPoint(const cg3::Point2d &q, const Dag &dag, const TrapezoidalMapDataset &trapezoidalMapData);

    bool leftPointEqualLeftEndpoint(Trapezoid trapezoid);

    bool leftPointEqualRightEndpoint(Trapezoid trapezoid);

    bool leftPointEqualBottomLeftEndpoint(Trapezoid trapezoid);

    bool rightPointEqualBottomRightEndpoint(Trapezoid trapezoid);

}

#endif // ALGORITHMS_H
