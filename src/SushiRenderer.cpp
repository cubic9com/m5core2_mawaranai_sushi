#include "SushiRenderer.h"

// Definition of sushi shape data
namespace SushiShape {
    // Rice (white part) triangles
    const Triangle RICE_TRIANGLES[] = {
        { {-50, -10}, { 35,  30}, { 20,  50} },
        { {-50, -10}, {-45,  30}, { 20,  50} }
    };
    
    // Tuna (red part) triangles
    const Triangle TUNA_TRIANGLES[] = {
        { {-70,  10}, {-40, -28}, {-40,   0} },
        { {  0, -30}, {-40, -28}, {-40,   0} },
        { {  0, -30}, {  0,  15}, {-40,   0} },
        { {  0, -30}, {  0,  15}, { 30, -10} },
        { {  0,  15}, { 30,  45}, { 30, -10} },
        { { 50,  30}, { 30,  45}, { 30, -10} },
        { { 50,  30}, { 30,  45}, { 50,  55} },
        { { 50,  30}, { 70,  45}, { 50,  55} }
    };
}

// Constructor
SushiRenderer::SushiRenderer(M5Canvas& canvas) : canvas(canvas) {}

// Calculate rotation matrix
void SushiRenderer::calculateRotationMatrix(float yawAngle, float matrix[2][2]) {
    // Convert yaw angle to radians (with 180 degree offset)
    float radians = (yawAngle - 180.0F) * DEG_TO_RAD;
    
    // Calculate rotation matrix elements
    matrix[0][0] = cos(radians);
    matrix[0][1] = -sin(radians);
    matrix[1][0] = sin(radians);
    matrix[1][1] = cos(radians);
}

// Rotate a point
Point2D SushiRenderer::rotatePoint(int offsetX, int offsetY, int x, int y, const float matrix[2][2]) {
    Point2D result;
    int translatedX = offsetX + x;
    int translatedY = offsetY + y;
    
    // Apply rotation matrix
    result.x = Config::Display::CENTER_X + translatedX * matrix[0][0] + translatedY * matrix[0][1];
    result.y = Config::Display::CENTER_Y + translatedX * matrix[1][0] + translatedY * matrix[1][1];
    
    return result;
}

// Draw a rotated triangle (applying rotation matrix)
void SushiRenderer::drawRotatedTriangle(int offsetX, int offsetY, const Triangle& triangle, uint16_t color, float yawAngle) {
    float rotMatrix[2][2];
    calculateRotationMatrix(yawAngle, rotMatrix);
    
    // Rotate each vertex
    Point2D p1 = rotatePoint(offsetX, offsetY, triangle.p1.x, triangle.p1.y, rotMatrix);
    Point2D p2 = rotatePoint(offsetX, offsetY, triangle.p2.x, triangle.p2.y, rotMatrix);
    Point2D p3 = rotatePoint(offsetX, offsetY, triangle.p3.x, triangle.p3.y, rotMatrix);

    // Draw the triangle
    canvas.fillTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color);
}

// Draw sushi
void SushiRenderer::drawSushi(int offsetX, int offsetY, float yawAngle) {
    // Rice (white part)
    for (const auto& triangle : SushiShape::RICE_TRIANGLES) {
        drawRotatedTriangle(offsetX, offsetY, triangle, Config::Sushi::RICE_COLOR, yawAngle);
    }
    
    // Tuna (red part)
    for (const auto& triangle : SushiShape::TUNA_TRIANGLES) {
        drawRotatedTriangle(offsetX, offsetY, triangle, Config::Sushi::TUNA_COLOR, yawAngle);
    }
}
