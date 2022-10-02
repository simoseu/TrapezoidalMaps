#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <cg3/geometry/segment2.h>
#include "data_structures/trapezoidalmap_dataset.h"
#include "data_structures/trapezoidalmap.h"
#include "data_structures/dag.h"
#include "drawables/drawable_trapezoidalmap.h"

/**
 * @brief Algorithms
 */
namespace algorithms{
    void initializeStructures(Dag &dag, DrawableTrapezoidalMap &trapezoidalMap);

    size_t queryPoint(const cg3::Point2d &q, const Dag &dag, const TrapezoidalMapDataset &trapezoidalMapData);

    size_t querySegment(const cg3::Segment2d &segment, const Dag &dag, const TrapezoidalMapDataset &trapezoidalMapData);

    std::vector<size_t> followSegment(const cg3::Segment2d &segment, const Dag &dag, const TrapezoidalMap &trapezoidalMap, const TrapezoidalMapDataset &trapezoidalMapData);

    void buildTrapezoidalMap(const cg3::Segment2d &segment, Dag &dag, DrawableTrapezoidalMap &trapezoidalMap, TrapezoidalMapDataset &trapezoidalMapData);

    void oneIntersectedTrapezoid(const cg3::Segment2d &segment, size_t intersectedTrapIdx, Dag &dag, DrawableTrapezoidalMap &trapezoidalMap, TrapezoidalMapDataset &trapezoidalMapData);

    void moreIntersectedTrapezoids(const cg3::Segment2d &segment, std::vector<size_t> intersectedTraps, Dag &dag, DrawableTrapezoidalMap &trapezoidalMap, TrapezoidalMapDataset &trapezoidalMapData);


    bool leftPointEqualLeftEndpoint(const Trapezoid &trapezoid);

    bool leftPointEqualRightEndpoint(const Trapezoid &trapezoid);

    bool leftPointEqualBottomLeftEndpoint(const Trapezoid &trapezoid);

    bool rightPointEqualBottomRightEndpoint(const Trapezoid &trapezoid);
}

#endif // ALGORITHMS_H
