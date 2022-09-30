#ifndef TRAPEZOIDALMAP_H
#define TRAPEZOIDALMAP_H

#include "trapezoid.h"
#include <vector>

#include <cg3/geometry/bounding_box2.h>

/**
 * @brief This class defines the Trapezoidal Map data structure.
 * It stores all the trapezoids in a vector, ...
 */
class TrapezoidalMap{

public:
    TrapezoidalMap(cg3::Point2d upperLeftPointBB, cg3::Point2d lowerRightPointBB);
    void addTrapezoid(Trapezoid &trapezoid);
    const std::vector<Trapezoid> &getTrapezoids() const;

    const Trapezoid &getTrapezoid(size_t idx) const;
    Trapezoid &getTrapezoid(size_t idx);

    // Necessary for replacing old trapezoids when a new segment is added
    void replaceTrapezoid(Trapezoid &trapezoid, size_t idx);

    const cg3::BoundingBox2 &getBoundingBox() const;

private:
    std::vector<Trapezoid> trapezoids; // Vector of all trapezoids
    cg3::BoundingBox2 boundingBox;  //  Bounding box
};

#endif // TRAPEZOIDALMAP_H
