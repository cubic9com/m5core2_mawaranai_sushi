#include <M5Core2.h>

#define BASE_X 160
#define BASE_Y 120
#define PI 3.141592653589793

float accX = 0.0F;
float accY = 0.0F;
float accZ = 0.0F;

float gyroX = 0.0F;
float gyroY = 0.0F;
float gyroZ = 0.0F;

float pitch = 0.0F;
float roll  = 0.0F;
float yaw   = 0.0F;

float temp = 0.0F;

TFT_eSprite disp(&M5.Lcd);

void setup(){
  M5.begin();
  
  M5.IMU.Init();

  disp.createSprite(320, 240);
}

void draw_sushi(int offset_x, int offset_y) {
  triangle(offset_x, offset_y, -50, -10,  35,  30,  20,  50, TFT_WHITE);
  triangle(offset_x, offset_y, -50, -10, -45,  30,  20,  50, TFT_WHITE);
  triangle(offset_x, offset_y, -70,  10, -40, -28, -40,   0, TFT_RED);
  triangle(offset_x, offset_y,   0, -30, -40, -28, -40,   0, TFT_RED);
  triangle(offset_x, offset_y,   0, -30,   0,  15, -40,   0, TFT_RED);
  triangle(offset_x, offset_y,   0, -30,   0,  15,  30, -10, TFT_RED);
  triangle(offset_x, offset_y,   0,  15,  30,  45,  30, -10, TFT_RED);
  triangle(offset_x, offset_y,  50,  30,  30,  45,  30, -10, TFT_RED);
  triangle(offset_x, offset_y,  50,  30,  30,  45,  50,  55, TFT_RED);
  triangle(offset_x, offset_y,  50,  30,  70,  45,  50,  55, TFT_RED);
}

void triangle(int offset_x, int offset_y, int orig_x1, int orig_y1, int orig_x2, int orig_y2, int orig_x3, int orig_y3, int c) {
  int new_x1, new_y1, new_x2, new_y2, new_x3, new_y3;

  float rad = yaw * PI / 180;
  float cos_val = cos(rad);
  float sin_val = sin(rad);
  new_x1 = BASE_X + (offset_x + orig_x1) * cos_val - (offset_y + orig_y1) * sin_val;
  new_y1 = BASE_Y + (offset_y + orig_y1) * cos_val + (offset_x + orig_x1) * sin_val;
  new_x2 = BASE_X + (offset_x + orig_x2) * cos_val - (offset_y + orig_y2) * sin_val;
  new_y2 = BASE_Y + (offset_y + orig_y2) * cos_val + (offset_x + orig_x2) * sin_val;
  new_x3 = BASE_X + (offset_x + orig_x3) * cos_val - (offset_y + orig_y3) * sin_val;
  new_y3 = BASE_Y + (offset_y + orig_y3) * cos_val + (offset_x + orig_x3) * sin_val;
  
  disp.fillTriangle(new_x1, new_y1, new_x2, new_y2, new_x3, new_y3, c);
}

void loop() {
  M5.IMU.getAhrsData(&pitch, &roll, &yaw);

  disp.fillScreen(TFT_BLACK);
  draw_sushi( 17, -40);
  draw_sushi(-17,   0);
  disp.pushSprite(0, 0);
}
