#include "trapezoidalmap.h"

TrapezoidalMap::TrapezoidalMap(){

}

void TrapezoidalMap::addTrapezoid(Trapezoid &trapezoid){
    trapezoids.push_back(trapezoid);
}

const std::vector<Trapezoid> &TrapezoidalMap::getTrapezoids() const{
    return trapezoids;
}

const Trapezoid &TrapezoidalMap::getTrapezoid(size_t index) const{
    return trapezoids[index];
}

void TrapezoidalMap::replaceTrapezoid(Trapezoid &trapezoid, size_t idx){
    trapezoids[idx] = trapezoid;
}
