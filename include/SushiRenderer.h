#pragma once

#include <M5Unified.h>
#include "Config.h"

// Structure representing 2D coordinates
struct Point2D {
    int x;
    int y;
};

// Structure representing triangle vertices
struct Triangle {
    Point2D p1;
    Point2D p2;
    Point2D p3;
};

// Sushi shape data
namespace SushiShape {
    // Rice (white part) triangles
    extern const Triangle RICE_TRIANGLES[];
    
    // Tuna (red part) triangles
    extern const Triangle TUNA_TRIANGLES[];
}

class SushiRenderer {
private:
    M5Canvas& canvas;
    
    // Calculate rotation matrix
    void calculateRotationMatrix(float yawAngle, float matrix[2][2]);
    
    // Rotate a point
    Point2D rotatePoint(int offsetX, int offsetY, int x, int y, const float matrix[2][2]);

public:
    SushiRenderer(M5Canvas& canvas);

    // Draw a rotated triangle (applying rotation matrix)
    void drawRotatedTriangle(int offsetX, int offsetY, const Triangle& triangle, uint16_t color, float yawAngle);
    
    // Draw sushi
    void drawSushi(int offsetX, int offsetY, float yawAngle);
};
