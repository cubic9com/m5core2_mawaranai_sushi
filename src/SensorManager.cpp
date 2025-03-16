#include "SensorManager.h"

// Implementation of Vector3D methods
void Vector3D::reset() {
    x = y = z = 0.0F;
}

Vector3D Vector3D::applyOffset(const Vector3D& offset) const {
    Vector3D result;
    result.x = x - offset.x;
    result.y = y - offset.y;
    result.z = z - offset.z;
    return result;
}

// Implementation of SensorManager methods
bool SensorManager::init() {
    return M5.Imu.init();
}

void SensorManager::calibrate() {
    // Explicitly initialize sum values
    Vector3D gyroSum;
    gyroSum.reset();  // Initialize x, y, z to 0
    
    Vector3D accelSum;
    accelSum.reset();  // Initialize x, y, z to 0
    
    float tempGyroX, tempGyroY, tempGyroZ;
    float tempAccX, tempAccY, tempAccZ;
    int validSamples = 0;  // Count valid samples
    const float GRAVITY_COMPENSATION = 1.0F;  // Gravity compensation value (depends on units)

    // Discard initial unstable values
    Serial.println("Sensor calibration starting...");
    Serial.println("Discarding initial samples...");
    for (int i = 0; i < Config::Sensor::CALIBRATION_IGNORE_SAMPLES; i++) {
        M5.Imu.getGyro(&tempGyroX, &tempGyroY, &tempGyroZ);
        M5.Imu.getAccel(&tempAccX, &tempAccY, &tempAccZ);
        delay(Config::Sensor::READ_DELAY_MS);
    }

    // Collect samples for offset calculation
    Serial.println("Collecting samples for offset calculation...");
    for (int i = 0; i < Config::Sensor::CALIBRATION_SAMPLE_COUNT; i++) {
        // Get gyroscope values
        if (M5.Imu.getGyro(&tempGyroX, &tempGyroY, &tempGyroZ)) {
            gyroSum.x += tempGyroX;
            gyroSum.y += tempGyroY;
            gyroSum.z += tempGyroZ;
        } else {
            Serial.println("Failed to read gyroscope sensor");
            continue;  // Skip this sample
        }

        // Get accelerometer values
        if (M5.Imu.getAccel(&tempAccX, &tempAccY, &tempAccZ)) {
            accelSum.x += tempAccX;
            accelSum.y += tempAccY;
            accelSum.z += tempAccZ;
            validSamples++;  // Increment valid sample count
        } else {
            Serial.println("Failed to read accelerometer sensor");
            continue;  // Skip this sample
        }

        delay(Config::Sensor::READ_DELAY_MS);
    }
    
    // Error if no valid samples
    if (validSamples == 0) {
        Serial.println("Calibration failed: No valid samples");
        return;
    }
    
    // Calculate average and set as offset
    offsets.gyro.x = gyroSum.x / validSamples;
    offsets.gyro.y = gyroSum.y / validSamples;
    offsets.gyro.z = gyroSum.z / validSamples;
    offsets.accel.x = accelSum.x / validSamples;
    offsets.accel.y = accelSum.y / validSamples;
    offsets.accel.z = (accelSum.z / validSamples) - GRAVITY_COMPENSATION; // Gravity compensation

    Serial.println("Sensor calibration complete");
    Serial.printf("Valid samples: %d/%d\n", validSamples, Config::Sensor::CALIBRATION_SAMPLE_COUNT);
    Serial.printf("Gyro offsets: X=%.3f, Y=%.3f, Z=%.3f\n", offsets.gyro.x, offsets.gyro.y, offsets.gyro.z);
    Serial.printf("Accel offsets: X=%.3f, Y=%.3f, Z=%.3f\n", offsets.accel.x, offsets.accel.y, offsets.accel.z);
}

void SensorManager::update() {
    float rawAccX, rawAccY, rawAccZ;
    float rawGyroX, rawGyroY, rawGyroZ;
    bool accelSuccess = false;
    bool gyroSuccess = false;
    
    // Get accelerometer values
    accelSuccess = M5.Imu.getAccel(&rawAccX, &rawAccY, &rawAccZ);
    if (accelSuccess) {
        accelData.x = rawAccX - offsets.accel.x;
        accelData.y = rawAccY - offsets.accel.y;
        accelData.z = rawAccZ - offsets.accel.z;
    } else {
        // Keep previous values on read failure
        Serial.println("Failed to read accelerometer sensor");
    }
    
    // Get gyroscope values
    gyroSuccess = M5.Imu.getGyro(&rawGyroX, &rawGyroY, &rawGyroZ);
    if (gyroSuccess) {
        gyroData.x = rawGyroX - offsets.gyro.x;
        gyroData.y = rawGyroY - offsets.gyro.y;
        gyroData.z = rawGyroZ - offsets.gyro.z;
    } else {
        // Keep previous values on read failure
        Serial.println("Failed to read gyroscope sensor");
    }

    // Update delta time
    deltaTime = fusion.deltatUpdate();
    
    // Only update orientation if both sensors read successfully
    if (accelSuccess && gyroSuccess) {
        // Calculate orientation using sensor fusion
        fusion.MahonyUpdate(
            gyroData.x * DEG_TO_RAD, 
            gyroData.y * DEG_TO_RAD, 
            gyroData.z * DEG_TO_RAD, 
            accelData.x, 
            accelData.y, 
            accelData.z, 
            deltaTime
        );
        
        // Update orientation angles
        orientation.yaw = fusion.getYaw();
        orientation.pitch = fusion.getPitch();
        orientation.roll = fusion.getRoll();
    }
}

const Orientation& SensorManager::getOrientation() const {
    return orientation;
}
