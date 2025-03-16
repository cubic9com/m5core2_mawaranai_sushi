#pragma once

#include <cstdint>
#include <M5Unified.h>

namespace Config {
    // Display related settings
    namespace Display {
        constexpr bool ENABLE_YAW_DISPLAY = true;  // Whether to display yaw angle on screen
        constexpr uint16_t SCREEN_WIDTH = 320;     // Screen width
        constexpr uint16_t SCREEN_HEIGHT = 240;    // Screen height
        constexpr uint16_t CENTER_X = 160;         // X coordinate of screen center
        constexpr uint16_t CENTER_Y = 120;         // Y coordinate of screen center
        constexpr uint8_t TEXT_SIZE = 2;           // Text size
        constexpr uint8_t REFRESH_DELAY_MS = 10;   // Delay between screen updates
    }

    // Sensor calibration related settings
    namespace Sensor {
        constexpr uint16_t CALIBRATION_IGNORE_SAMPLES = 500;  // Number of samples to ignore during calibration
        constexpr uint16_t CALIBRATION_SAMPLE_COUNT = 500;    // Number of samples to use for calibration
        constexpr uint16_t READ_DELAY_MS = 2;                 // Delay between sensor readings
    }

    // Sushi color and shape related settings
    namespace Sushi {
        constexpr uint16_t RICE_COLOR = TFT_WHITE;  // Rice color
        constexpr uint16_t TUNA_COLOR = TFT_RED;    // Tuna color
    }
}
