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

    const cg3::Color& getColor() const;
    void setColor(const cg3::Color& value);

    const cg3::Color randomColor() const;

private:
    cg3::Color color;

};

#endif // DRAWABLE_TRAPEZOIDALMAP_H
