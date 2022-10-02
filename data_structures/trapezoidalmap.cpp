#include "trapezoidalmap.h"

/**
 * @brief Constructor
 * @param[in] upperLeftPointBB the upper left point of the bounding box
 * @param[in] lowerRightPointBB the lower right point of the bounding box
*/
TrapezoidalMap::TrapezoidalMap(cg3::Point2d upperLeftPointBB, cg3::Point2d lowerRightPointBB): boundingBox(cg3::BoundingBox2(upperLeftPointBB, lowerRightPointBB)){

}

/**
 * @brief Add a trapezoid in the trapezoidal map
 * @param[in] trapezoid the trapezoid to be inserted
*/
void TrapezoidalMap::addTrapezoid(Trapezoid &trapezoid){
    trapezoids.push_back(trapezoid);
}

/**
 * @brief Get the trapezoids stored in the trapezoidal map
 * @return the vector storing all the trapezoids
*/
const std::vector<Trapezoid> &TrapezoidalMap::getTrapezoids() const{
    return trapezoids;
}

/**
 * @brief Get a trapezoid given its index
 * @param[in] index the index of the trapezoid
 * @return the trapezoid stored in the given index
*/
const Trapezoid &TrapezoidalMap::getTrapezoid(size_t index) const{
    return trapezoids[index];
}
Trapezoid &TrapezoidalMap::getTrapezoid(size_t index){
    return trapezoids[index];
}

/**
 * @brief Replace an old trapezoid with a given one, given the index of the trapezoid to replace
 * @param[in] trapezoid the trapezoid to insert
 * @param[in] idx the index of the old trapezoid to replace
 * @return a boolean - true if the replace can take place - false if is not possible to execute the replace since the given index excesses the number of stored trapezoid
*/
bool TrapezoidalMap::replaceTrapezoid(Trapezoid &trapezoid, size_t idx){
    if(idx >= trapezoids.size()) return false;

    trapezoids[idx] = trapezoid;
    return true;
}

/**
 * @brief Get the bounding box
 * @return the bounding box
*/
const cg3::BoundingBox2 &TrapezoidalMap::getBoundingBox() const{
    return boundingBox;
}

/**
 * @brief Get the number of stored trapezoids
 * @return the number of trapezoid stored in the vector
*/
size_t TrapezoidalMap::numTrapezoids() const{
    return trapezoids.size();
}

/**
 * @brief Remove all the stored trapezoids
*/
void TrapezoidalMap::clear(){
    trapezoids.clear();
}
