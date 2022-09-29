#include "drawable_trapezoidalmap.h"

#include <cg3/viewer/opengl_objects/opengl_objects2.h>


DrawableTrapezoidalMap::DrawableTrapezoidalMap(cg3::Point2d upperLeftPointBB, cg3::Point2d lowerRightPointBB): TrapezoidalMap(upperLeftPointBB, lowerRightPointBB){

}


const cg3::Color DrawableTrapezoidalMap::randomColor() const{

    int red = std::rand()%256;
    int green = std::rand()%256;
    int blue = std::rand()%256;


    red = (red + 255) / 2;
    green = (green + 255) / 2;
    blue = (blue + 255) / 2;

    const cg3::Color c = cg3::Color(red, green, blue);
    return c;
}

void DrawableTrapezoidalMap::draw() const{

    for(const Trapezoid trap : getTrapezoids()){
        const std::vector<cg3::Point2d> corners = trap.getCorners();
        cg3::Color colore = randomColor();
        // Check if the left endpoint of the top segment is equal to the left endpoint of the bottom segment (it is a triangle)
        if(corners[0] == corners[3]){
            cg3::opengl::drawLine2(corners[0], corners[1], cg3::Color(0,0,0), 3);
            cg3::opengl::drawLine2(corners[1], corners[2], cg3::Color(0,0,0), 3);
            cg3::opengl::drawLine2(corners[2], corners[0], cg3::Color(0,0,0), 3);
            cg3::opengl::drawTriangle2(corners[0], corners[1], corners[2], colore, 1, true);
        }else if(corners[1] == corners[2]){// Check if the right endpoint of the top segment is equal to the right endpoint of the bottom segment (it is a triangle)

            cg3::opengl::drawLine2(corners[0], corners[1], cg3::Color(0,0,0), 3);
            cg3::opengl::drawLine2(corners[1], corners[3], cg3::Color(0,0,0), 3);
            cg3::opengl::drawLine2(corners[3], corners[0], cg3::Color(0,0,0), 3);
            cg3::opengl::drawTriangle2(corners[0], corners[1], corners[3], colore, 1, true);
        }else{

            cg3::opengl::drawLine2(corners[0], corners[1], cg3::Color(0,0,0), 3);
            cg3::opengl::drawLine2(corners[1], corners[2], cg3::Color(0,0,0), 3);
            cg3::opengl::drawLine2(corners[2], corners[3], cg3::Color(0,0,0), 3);
            cg3::opengl::drawLine2(corners[3], corners[0], cg3::Color(0,0,0), 3);
            cg3::opengl::drawQuad2(corners[0], corners[1], corners[2], corners[3], colore, 1, true);
        }
    }
}

cg3::Point3d DrawableTrapezoidalMap::sceneCenter() const
{
    const cg3::BoundingBox2& boundingBox = cg3::BoundingBox2();
    return cg3::Point3d(boundingBox.center().x(), boundingBox.center().y(), 0);
}

double DrawableTrapezoidalMap::sceneRadius() const
{
    const cg3::BoundingBox2& boundingBox = cg3::BoundingBox2();
    return boundingBox.diag();
}
