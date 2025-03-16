#include "DisplayManager.h"

DisplayManager::DisplayManager() : sushiRenderer(canvas) {
    // Initialize offscreen buffer
    canvas.setColorDepth(8);  // Set 8-bit color depth
    canvas.createSprite(Config::Display::SCREEN_WIDTH, Config::Display::SCREEN_HEIGHT);
    canvas.setTextSize(Config::Display::TEXT_SIZE);
}

void DisplayManager::init() {
    // Initialize display and optimize settings
    M5.Display.init();
    M5.Display.setColorDepth(8);  // Set 8-bit color depth
    M5.Display.startWrite();
}

void DisplayManager::update(const Orientation& orientation) {
    // Clear the screen
    canvas.fillScreen(TFT_BLACK);
    
    // Display yaw angle (if enabled)
    if (Config::Display::ENABLE_YAW_DISPLAY) {
        canvas.setCursor(0, 0);
        canvas.printf("%5.2f", orientation.yaw);
    }
    
    // Draw sushi (two pieces at different positions)
    sushiRenderer.drawSushi( 17, -40, orientation.yaw);
    sushiRenderer.drawSushi(-17,   0, orientation.yaw);
    
    // Push to display
    canvas.pushSprite(&M5.Display, 0, 0);
}
