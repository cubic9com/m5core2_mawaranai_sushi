#pragma once

#include <M5Unified.h>
#include <SensorFusion.h>
#include "Config.h"

// 3D vector data (common for accelerometer and gyroscope)
struct Vector3D {
    float x = 0.0F;
    float y = 0.0F;
    float z = 0.0F;
    
    // Reset values
    void reset();
    
    // Apply offset
    Vector3D applyOffset(const Vector3D& offset) const;
};

// Orientation angles
struct Orientation {
    float pitch = 0.0F;
    float roll = 0.0F;
    float yaw = 0.0F;
};

// Sensor offset values
struct SensorOffsets {
    Vector3D accel;
    Vector3D gyro;
};

class SensorManager {
private:
    SF fusion;                // Sensor fusion instance
    Vector3D accelData;       // Accelerometer data
    Vector3D gyroData;        // Gyroscope data
    Orientation orientation;  // Orientation angles
    SensorOffsets offsets;    // Sensor offset values
    float deltaTime = 0.0F;   // Delta time

public:
    // Initialize
    bool init();

    // Calculate sensor offset values
    void calibrate();

    // Update sensor data
    void update();

    // Get orientation
    const Orientation& getOrientation() const;
};
