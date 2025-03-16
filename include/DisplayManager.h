#pragma once

#include <M5Unified.h>
#include "Config.h"
#include "SensorManager.h"
#include "SushiRenderer.h"

class DisplayManager {
private:
    M5Canvas canvas;
    SushiRenderer sushiRenderer;

public:
    DisplayManager();

    // Initialize
    void init();

    // Update display
    void update(const Orientation& orientation);
};
