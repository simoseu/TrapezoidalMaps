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
    void buildTrapezoidalMap(const cg3::Segment2d &segment, const Dag &dag, const TrapezoidalMap &trapezoidalMap, const TrapezoidalMapDataset &TrapezoidalMapData);
    std::vector<size_t> followSegment(const cg3::Segment2d &segment, const Dag &dag, const TrapezoidalMap &trapezoidalMap, const TrapezoidalMapDataset &trapezoidalMapData);

    size_t queryPoint(const cg3::Point2d &q, const Dag &dag, const TrapezoidalMapDataset &trapezoidalMapData);
}

#endif // ALGORITHMS_H
