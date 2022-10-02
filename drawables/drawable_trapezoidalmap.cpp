#include "drawable_trapezoidalmap.h"

#include <cg3/viewer/opengl_objects/opengl_objects2.h>
/**
 * @brief Constructor
 * @param[in] upperLeftPointBB the upper left point of the bounding box
 * @param[in] lowerRightPointBB the lower right point of the bounding box
 * Initialize also the highlighted trapezoid with a "null" index
*/
DrawableTrapezoidalMap::DrawableTrapezoidalMap(cg3::Point2d upperLeftPointBB, cg3::Point2d lowerRightPointBB): TrapezoidalMap(upperLeftPointBB, lowerRightPointBB),
    highlightedTrap(std::numeric_limits<size_t>::max())
{

}

/**
 * @brief Draw method
 * Draw all trapezoids stored in the trapezoidal map.
 * Check the shape of the trapezoid (if is a triangle), and draw a line in all the edges of the trapezoid
 * If the trapezoid need to be highlighted its color is set to a bright yellow
*/
void DrawableTrapezoidalMap::draw() const{
    size_t idx = 0;
    for(Trapezoid trap : getTrapezoids()){
        const std::vector<cg3::Point2d> corners = trap.getCorners();
        cg3::Color colore = colors[idx];
        if(highlightedTrap == idx++) colore = cg3::Color(212,255,50);
        // Check if the left endpoint of the top segment is equal to the left endpoint of the bottom segment (it is a triangle)
        if(corners[0] == corners[3]){
            cg3::opengl::drawLine2(corners[0], corners[1], cg3::Color(0,0,0), 1);
            cg3::opengl::drawLine2(corners[1], corners[2], cg3::Color(0,0,0), 1);
            cg3::opengl::drawLine2(corners[2], corners[0], cg3::Color(0,0,0), 1);
            cg3::opengl::drawTriangle2(corners[0], corners[1], corners[2], colore, 1, true);
        }else if(corners[1] == corners[2]){// Check if the right endpoint of the top segment is equal to the right endpoint of the bottom segment (it is a triangle)

            cg3::opengl::drawLine2(corners[0], corners[1], cg3::Color(0,0,0), 1);
            cg3::opengl::drawLine2(corners[1], corners[3], cg3::Color(0,0,0), 1);
            cg3::opengl::drawLine2(corners[3], corners[0], cg3::Color(0,0,0), 1);
            cg3::opengl::drawTriangle2(corners[0], corners[1], corners[3], colore, 1, true);
        }else{

            cg3::opengl::drawLine2(corners[0], corners[1], cg3::Color(0,0,0), 1);
            cg3::opengl::drawLine2(corners[1], corners[2], cg3::Color(0,0,0), 1);
            cg3::opengl::drawLine2(corners[2], corners[3], cg3::Color(0,0,0), 1);
            cg3::opengl::drawLine2(corners[3], corners[0], cg3::Color(0,0,0), 1);
            cg3::opengl::drawQuad2(corners[0], corners[1], corners[2], corners[3], colore, 1, true);
        }
    }
}

cg3::Point3d DrawableTrapezoidalMap::sceneCenter() const
{
    const cg3::BoundingBox2& boundingBox = this->getBoundingBox();
    return cg3::Point3d(boundingBox.center().x(), boundingBox.center().y(), 0);
}

double DrawableTrapezoidalMap::sceneRadius() const
{
    const cg3::BoundingBox2& boundingBox = this->getBoundingBox();
    return boundingBox.diag();
}

/**
 * @brief Add a trapezoid to the trapezoidal map and a new color associated to it
 * @param[in] trapezoid the trapezoid to add
 * Add the trapezoid to the trapezoidal map and also its associated color
*/

void DrawableTrapezoidalMap::addTrapezoid(Trapezoid &trapezoid){
    // Add the color for the trap in the vector of color
    if(colors.size()>0)colors.push_back(ProjectUtils::randomColor());
    else colors.push_back(cg3::Color(200,220,250));
    TrapezoidalMap::addTrapezoid(trapezoid);
}

/**
 * @brief Set the index of the highlighted trapezoid
 * @param[in] idx the index of the trapezoid
*/
void DrawableTrapezoidalMap::setHighlightedTrap(size_t idx){
    highlightedTrap = idx;
}

/**
 * @brief Delete all trapezoids stored in the trapezoidal map and all colors, reset also the color of the highlighted trapezoid
*/
void DrawableTrapezoidalMap::clear(){
    highlightedTrap = std::numeric_limits<size_t>::max();
    colors.clear();
    TrapezoidalMap::clear();
}
