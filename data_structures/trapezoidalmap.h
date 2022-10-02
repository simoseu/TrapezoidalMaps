#ifndef TRAPEZOIDALMAP_H
#define TRAPEZOIDALMAP_H

#include "trapezoid.h"
#include <vector>

#include <cg3/geometry/bounding_box2.h>

/**
 * @brief This class defines the Trapezoidal Map data structure.
 * It stores all the trapezoids in a vector, is possible to add a trapezoid, replace an existing trapezoid, get the vector of the trapezoids, get a trapezoid by its index and get the number of
 * trapezoids stored in the trapezoidalmap. Store also a bounding box needed for the drawable version of the trapezoidal map
 */
class TrapezoidalMap{

public:
    // Constructor
    TrapezoidalMap(cg3::Point2d upperLeftPointBB, cg3::Point2d lowerRightPointBB);
    // Add a trapezoid to the vector
    void addTrapezoid(Trapezoid &trapezoid);
    // Get all stored trapezoids
    const std::vector<Trapezoid> &getTrapezoids() const;
    // Get a stored trapezoid by its index
    const Trapezoid &getTrapezoid(size_t idx) const;
    Trapezoid &getTrapezoid(size_t idx);
    // Get the number of trapezoids stored
    size_t numTrapezoids() const;
    // Replace old trapezoid with a new one
    bool replaceTrapezoid(Trapezoid &trapezoid, size_t idx);
    // Get the bounding box of the trapezoidal map
    const cg3::BoundingBox2 &getBoundingBox() const;
    // Remove all the stored trapezoids
    void clear();
private:
    std::vector<Trapezoid> trapezoids; // Vector of all trapezoids
    cg3::BoundingBox2 boundingBox;  //  Bounding box
};

#endif // TRAPEZOIDALMAP_H
