#ifndef TRAPEZOIDALMAP_H
#define TRAPEZOIDALMAP_H

#include "trapezoid.h"
#include <vector>

/**
 * @brief This class defines the Trapezoidal Map data structure.
 * It stores all the trapezoids in a vector, ...
 */
class TrapezoidalMap{

public:
    TrapezoidalMap();
    void addTrapezoid(Trapezoid &trapezoid);
    const std::vector<Trapezoid> &getTrapezoids() const;
    const Trapezoid &getTrapezoid(size_t idx) const;

    // Necessary for replacing old trapezoids when a new segment is added
    void replaceTrapezoid(Trapezoid &trapezoid, size_t idx);

private:
    std::vector<Trapezoid> trapezoids; // Vector of all trapezoids

};

#endif // TRAPEZOIDALMAP_H
