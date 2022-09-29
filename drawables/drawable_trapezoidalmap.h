#ifndef DRAWABLE_TRAPEZOIDALMAP_H
#define DRAWABLE_TRAPEZOIDALMAP_H

#include "data_structures/trapezoidalmap.h"

#include <cg3/viewer/interfaces/drawable_object.h>

#include <cg3/utilities/color.h>
/**
 * @brief Class to draw the trapezoid.
 */

class DrawableTrapezoidalMap : public TrapezoidalMap, public cg3::DrawableObject{

public:
    DrawableTrapezoidalMap(cg3::Point2d upperLeftPointBB, cg3::Point2d lowerRightPointBB);

    void draw() const;
    cg3::Point3d sceneCenter() const;
    double sceneRadius() const;


    void addTrapezoid(Trapezoid &trapezoid);
    const cg3::Color randomColor() const;

    void setHighlightedTrap(size_t idx);

private:
    std::vector<cg3::Color> colors;
    size_t highlightedTrap;
};

#endif // DRAWABLE_TRAPEZOIDALMAP_H
