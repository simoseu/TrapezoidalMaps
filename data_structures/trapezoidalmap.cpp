#include "trapezoidalmap.h"

TrapezoidalMap::TrapezoidalMap(){

}

void TrapezoidalMap::addTrapezoid(Trapezoid trapezoid){
    trapezoids.push_back(trapezoid);
}

std::vector<Trapezoid> TrapezoidalMap::getTrapezoids(){
    return trapezoids;
}

Trapezoid TrapezoidalMap::getTrapezoid(size_t index){
    return trapezoids[index];
}
