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
    void addTrapezoid(Trapezoid trapezoid);
    std::vector<Trapezoid> getTrapezoids();
    Trapezoid getTrapezoid(size_t index);

private:
    std::vector<Trapezoid> trapezoids; // Vector of all trapezoids

};

#endif // TRAPEZOIDALMAP_H
