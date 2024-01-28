#include <M5Unified.h>
#include <SensorFusion.h>
SF fusion;

const bool enable_print_yaw = true;
const uint16_t ignore_samples = 500;
const uint16_t offset_samples = 500;
const uint16_t base_x = 160;
const uint16_t base_y = 120;

M5Canvas disp;

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

float deltat = 0.0F;

void calc_offsets() {
  float sum_gyroX = 0.0F;
  float sum_gyroY = 0.0F;
  float sum_gyroZ = 0.0F;
  float sum_accX = 0.0F;
  float sum_accY = 0.0F;
  float sum_accZ = 0.0F;

  // Since the values are unstable, discard the beginning part of samples.
  for (int i = 0; i < ignore_samples; i++) {
    M5.Imu.getGyro(&gyroX, &gyroY, &gyroZ);

    M5.Imu.getAccel(&accX, &accY, &accZ);

    delay(2);
  }

  // Calculate offsets.
  for (int i = 0; i < offset_samples; i++) {
    M5.Imu.getGyro(&gyroX, &gyroY, &gyroZ);
    sum_gyroX += gyroX;
    sum_gyroY += gyroY;
    sum_gyroZ += gyroZ;

    M5.Imu.getAccel(&accX, &accY, &accZ);
    sum_accX += accX;
    sum_accY += accY;
    sum_accZ += accZ;

    delay(2);
  }
  offset_gyroX = sum_gyroX / offset_samples;
  offset_gyroY = sum_gyroY / offset_samples;
  offset_gyroZ = sum_gyroZ / offset_samples;
  offset_accX = sum_accX / offset_samples;
  offset_accY = sum_accY / offset_samples;
  offset_accZ = (sum_accZ / offset_samples) - 1.0;
}

void triangle(int offset_x, int offset_y, int orig_x1, int orig_y1, int orig_x2, int orig_y2, int orig_x3, int orig_y3, int c) {
  int new_x1, new_y1, new_x2, new_y2, new_x3, new_y3;

  float rad = (yaw - 180.0F) * DEG_TO_RAD;
  float cos_val = cos(rad);
  float sin_val = sin(rad);
  new_x1 = base_x + (offset_x + orig_x1) * cos_val - (offset_y + orig_y1) * sin_val;
  new_y1 = base_y + (offset_y + orig_y1) * cos_val + (offset_x + orig_x1) * sin_val;
  new_x2 = base_x + (offset_x + orig_x2) * cos_val - (offset_y + orig_y2) * sin_val;
  new_y2 = base_y + (offset_y + orig_y2) * cos_val + (offset_x + orig_x2) * sin_val;
  new_x3 = base_x + (offset_x + orig_x3) * cos_val - (offset_y + orig_y3) * sin_val;
  new_y3 = base_y + (offset_y + orig_y3) * cos_val + (offset_x + orig_x3) * sin_val;

  disp.fillTriangle(new_x1, new_y1, new_x2, new_y2, new_x3, new_y3, c);
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

void setup(){
  M5.begin();
  M5.Imu.init();

  calc_offsets();

  disp.setPsram(true);
  disp.createSprite(320, 240);
}

void loop() {
  M5.Imu.getAccel(&accX, &accY, &accZ);
  accX -= offset_accX;
  accY -= offset_accY;
  accZ -= offset_accZ;
  M5.Imu.getGyro(&gyroX, &gyroY, &gyroZ);
  gyroX -= offset_gyroX;
  gyroY -= offset_gyroY;
  gyroZ -= offset_gyroZ;

  deltat = fusion.deltatUpdate();
  fusion.MahonyUpdate(gyroX * DEG_TO_RAD, gyroY * DEG_TO_RAD, gyroZ * DEG_TO_RAD, accX, accY, accZ, deltat);
  yaw = fusion.getYaw();

  disp.fillScreen(TFT_BLACK);
  if (enable_print_yaw) {
    disp.setCursor(0, 0);
    disp.printf("%5.2f", yaw);
  }
  draw_sushi( 17, -40);
  draw_sushi(-17,   0);
  disp.pushSprite(&M5.Display, 0, 0);
}
