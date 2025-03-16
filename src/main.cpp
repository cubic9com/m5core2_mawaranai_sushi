#include <M5Unified.h>
#include "Config.h"
#include "SensorManager.h"
#include "DisplayManager.h"

// Instances of each manager
SensorManager sensorManager;
DisplayManager displayManager;

// Setup
void setup() {
  // Initialize M5Stack
  M5.begin();
  
  // Initialize serial communication
  Serial.begin(115200);
  Serial.println("Non-rotating Sushi - Initializing...");

  // Initialize display
  displayManager.init();

  // Initialize IMU sensor
  if (!sensorManager.init()) {
    Serial.println("IMU sensor initialization failed");
    while (1) {
      M5.Display.clear();
      M5.Display.setCursor(0, 0);
      M5.Display.println("IMU Sensor Error");
      M5.Display.println("Please restart the device");
      delay(1000);
    }
  }
  
  // Calibrate sensors
  sensorManager.calibrate();
  
  Serial.println("Initialization complete");
}

// Main loop
void loop() {
  // Update button states
  M5.update();
  
  // Update sensor data
  sensorManager.update();
  
  // Update display
  displayManager.update(sensorManager.getOrientation());
  
  // Small delay
  delay(Config::Display::REFRESH_DELAY_MS);
}
