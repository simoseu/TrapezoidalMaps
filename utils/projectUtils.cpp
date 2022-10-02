#include "projectUtils.h"

namespace ProjectUtils{

/**
 * @brief order the endpoints of a given segment by their x values
 * @param[in] segment the segment to order
*/
void orderSegment(cg3::Segment2d &segment){
    if (segment.p1().x() > segment.p2().x()) {
        cg3::Point2d aux = segment.p1();
        segment.setP1(segment.p2());
        segment.setP2(aux);
    }
}

/**
 * @brief Check if the left point of a trapezoid is equal to the left endpoint of its top segment
 * @param[in] trapezoid the trapezoid to check
 * @return true if the left point is equal to the left endpoint of its top segment, false otherwise
*/
bool leftPointEqualTopLeftEndpoint(const Trapezoid &trapezoid){
    cg3::Segment2d topSegment = trapezoid.getTopSegment();
    orderSegment(topSegment);
    return trapezoid.getLeftPoint() == topSegment.p1();
}

/**
 * @brief Check if the right point of a trapezoid is equal to the right endpoint of its top segment
 * @param[in] trapezoid the trapezoid to check
 * @return true if the right point is equal to the right endpoint of its top segment, false otherwise
*/
bool rightPointEqualTopRightEndpoint(const Trapezoid &trapezoid){
    cg3::Segment2d topSegment = trapezoid.getTopSegment();
    orderSegment(topSegment);
    return trapezoid.getRightPoint() == topSegment.p2();
}

/**
 * @brief Check if the left point of a trapezoid is equal to the left endpoint of its bottom segment
 * @param[in] trapezoid the trapezoid to check
 * @return true if the left point is equal to the left endpoint of its bottom segment, false otherwise
*/
bool leftPointEqualBottomLeftEndpoint(const Trapezoid &trapezoid){
    cg3::Segment2d bottomSegment = trapezoid.getBottomSegment();
    orderSegment(bottomSegment);
    return trapezoid.getLeftPoint() == bottomSegment.p1();
}

/**
 * @brief Check if the right point of a trapezoid is equal to the right endpoint of its bottom segment
 * @param[in] trapezoid the trapezoid to check
 * @return true if the right point is equal to the right endpoint of its bottom segment, false otherwise
*/
bool rightPointEqualBottomRightEndpoint(const Trapezoid &trapezoid){
    cg3::Segment2d bottomSegment = trapezoid.getBottomSegment();
    orderSegment(bottomSegment);
    return trapezoid.getRightPoint() == bottomSegment.p2();
}

/**
 * @brief generate a random color
 * @return a random generated color
*/
const cg3::Color randomColor() {
    // Random generation of red green and blue values
    float red = std::rand()%256;
    float green = std::rand()%256;
    float blue = std::rand()%256;

    // Added a blue pastel color
    red = (red + 175) / 2;
    green = (green + 200) / 2;
    blue = (blue + 255) / 2;

    return cg3::Color(red, green, blue);
}

}
