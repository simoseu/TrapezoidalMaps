#ifndef DRAWABLE_TRAPEZOID_H
#define DRAWABLE_TRAPEZOID_H

#include "data_structures/trapezoid.h"

#include <cg3/viewer/interfaces/drawable_object.h>

#include <cg3/utilities/color.h>
/**
 * @brief Class to draw the trapezoid.
 */

class DrawableTrapezoid : public Trapezoid, public cg3::DrawableObject{

public:
    DrawableTrapezoid();

    void draw() const;
    cg3::Point3d sceneCenter() const;
    double sceneRadius() const;

    const cg3::Color& getColor() const;
    void setColor(const cg3::Color& value);

private:
    cg3::Color color;

};

#endif // DRAWABLE_TRAPEZOID_H
