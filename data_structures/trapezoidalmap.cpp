#include "trapezoidalmap.h"

TrapezoidalMap::TrapezoidalMap(cg3::Point2d upperLeftPointBB, cg3::Point2d lowerRightPointBB): boundingBox(cg3::BoundingBox2(upperLeftPointBB, lowerRightPointBB)){

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

Trapezoid &TrapezoidalMap::getTrapezoid(size_t index){
    return trapezoids[index];
}

bool TrapezoidalMap::replaceTrapezoid(Trapezoid &trapezoid, size_t idx){
    if(idx >= trapezoids.size()) return false;

    trapezoids[idx] = trapezoid;
    return true;
}

const cg3::BoundingBox2 &TrapezoidalMap::getBoundingBox() const{
    return boundingBox;
}

size_t TrapezoidalMap::numTrapezoids() const{
    return trapezoids.size();
}
