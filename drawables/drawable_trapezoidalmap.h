#ifndef DRAWABLE_TRAPEZOIDALMAP_H
#define DRAWABLE_TRAPEZOIDALMAP_H

#include "data_structures/trapezoidalmap.h"

#include <cg3/viewer/interfaces/drawable_object.h>

#include "utils/projectUtils.h"


/**
 * @brief Class to draw the trapezoid, drawable version of the trapezoidal map.
 * It stores a vector containing the color associated to each trapezoid in the trapezoidal map,
 * the index of the highlighted trapezoid (the trapezoid found when querying the trapezoidal map)
 */
class DrawableTrapezoidalMap : public TrapezoidalMap, public cg3::DrawableObject{

public:
    // Constructor
    DrawableTrapezoidalMap(cg3::Point2d upperLeftPointBB, cg3::Point2d lowerRightPointBB);

    void draw() const;
    cg3::Point3d sceneCenter() const;
    double sceneRadius() const;
    // Add trapezoid adapted for adding also a color when a trapezoid is added
    void addTrapezoid(Trapezoid &trapezoid);
    // Set the index of the trapezoid to highlight
    void setHighlightedTrap(size_t idx);
    // Delete all trapezoids and all colors stored
    void clear();
private:
    std::vector<cg3::Color> colors;
    size_t highlightedTrap;
};

#endif // DRAWABLE_TRAPEZOIDALMAP_H
