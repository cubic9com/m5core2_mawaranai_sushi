#include <M5Core2.h>

#define ENABLE_OFFSETS 1
#define ENABLE_PRINT_YAW 1
#define IGNORE_SAMPLES 500
#define OFFSET_SAMPLES 500
#define BASE_X 160
#define BASE_Y 120

TFT_eSprite disp(&M5.Lcd);

float accX = 0.0F;
float accY = 0.0F;
float accZ = 0.0F;

float gyroX = 0.0F;
float gyroY = 0.0F;
float gyroZ = 0.0F;

float pitch = 0.0F;
float roll  = 0.0F;
float yaw   = 0.0F;

float offset_accX = 0.0F;
float offset_accY = 0.0F;
float offset_accZ = 0.0F;

float offset_gyroX = 0.0F;
float offset_gyroY = 0.0F;
float offset_gyroZ = 0.0F;

void calc_offsets() {
  float sum_gyroX = 0.0F;
  float sum_gyroY = 0.0F;
  float sum_gyroZ = 0.0F;
  float sum_accX = 0.0F;
  float sum_accY = 0.0F;
  float sum_accZ = 0.0F;

  // Since the values are unstable, discard the beginning part of samples.
  for (int i = 0; i < IGNORE_SAMPLES; i++) {
    M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ);

    M5.IMU.getAccelData(&accX, &accY, &accZ);

    delay(2);
  }

  // Calculate offsets.
  for (int i = 0; i < OFFSET_SAMPLES; i++) {
    M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ);
    sum_gyroX += gyroX;
    sum_gyroY += gyroY;
    sum_gyroZ += gyroZ;

    M5.IMU.getAccelData(&accX, &accY, &accZ);
    sum_accX += accX;
    sum_accY += accY;
    sum_accZ += accZ;

    delay(2);
  }
  offset_gyroX = sum_gyroX / OFFSET_SAMPLES;
  offset_gyroY = sum_gyroY / OFFSET_SAMPLES;
  offset_gyroZ = sum_gyroZ / OFFSET_SAMPLES;
  offset_accX = sum_accX / OFFSET_SAMPLES;
  offset_accY = sum_accY / OFFSET_SAMPLES;
  offset_accZ = (sum_accZ / OFFSET_SAMPLES) - 1.0;
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

  float rad = yaw * DEG_TO_RAD;
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

void setup(){
  M5.begin();
  M5.IMU.Init();

  if (ENABLE_OFFSETS == 1) {
    calc_offsets();
  }

  disp.createSprite(320, 240);
}

void loop() {
  if (ENABLE_OFFSETS == 1) {
    M5.IMU.getAccelData(&accX, &accY, &accZ);
    accX -= offset_accX;
    accY -= offset_accY;
    accZ -= offset_accZ;
    M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ);
    gyroX -= offset_gyroX;
    gyroY -= offset_gyroY;
    gyroZ -= offset_gyroZ;
    MahonyAHRSupdateIMU(gyroX * DEG_TO_RAD, gyroY * DEG_TO_RAD, gyroZ * DEG_TO_RAD, accX, accY, accZ, &pitch, &roll, &yaw);
  } else {
    M5.IMU.getAhrsData(&pitch, &roll, &yaw);
  }

  disp.fillScreen(TFT_BLACK);
  if (ENABLE_PRINT_YAW == 1) {
    disp.setCursor(0, 0);
    disp.printf("%5.2f", yaw);
  }
  draw_sushi( 17, -40);
  draw_sushi(-17,   0);
  disp.pushSprite(0, 0);
}
