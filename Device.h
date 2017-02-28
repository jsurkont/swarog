/*
  Swarog - IoT lamp and sunrise alarm.
  Jaroslaw Surkont
*/

#ifndef Device_h
#define Device_h

#define LAMP_BRIGHTNESS_LOW 0
#define LAMP_BRIGHTNESS_HIGH 255
#define LAMP_BRIGHTNESS_STEPS 5
#define LAMP_RISE_UPDATE_INTERVAL 2353  // rise in ~10min with 255 steps

class Button {
public:
  Button();
  Button(uint8_t pin);
  boolean isPressed();
private:
  uint8_t pin;
  uint8_t state;
  uint8_t readout;
  uint8_t readoutLast;
  unsigned long lastDebounceTime;
  unsigned long debounceDelay;
};

class Lamp {
public:
  Lamp();
  Lamp(uint8_t pin);
  int getState();
  void on();
  void off();
  void brighten();
  void dim();
  void rise();
  void riseUpdate();
  boolean isOn();
  boolean isOff();
  boolean isMax();
  boolean isRising();
private:
  uint8_t pin;
  uint16_t state;
  boolean rising;
  unsigned long lastRiseUpdate;
  void update();
};

const uint8_t gamma8[] = {
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
   1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
   2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
   5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
  10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
  17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
  25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
  37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
  51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
  69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
  90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
 115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
 144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
 177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
 215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

#endif
