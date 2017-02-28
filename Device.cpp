/*
  Swarog - IoT lamp and sunrise alarm.
  Jaroslaw Surkont
*/

#include <Arduino.h>
#include "Device.h"

Button::Button() {}

Button::Button(uint8_t pin) {
  this->pin = pin;
  this->state = LOW;
  this->readout = LOW;
  this->readoutLast = LOW;
  this->lastDebounceTime = 0;
  this->debounceDelay = 50;
  pinMode(pin, INPUT);
}

boolean Button::isPressed() {
  boolean pressed = false;
  this->readout = digitalRead(pin);
  if (this->readout != this->readoutLast) {
    this->lastDebounceTime = millis();
  }
  if ((millis() - this->lastDebounceTime) > this->debounceDelay) {
    if (this->readout != this->state) {
      this->state = this->readout;
      if (this->state == HIGH) pressed = true;
    }
  }
  this->readoutLast = this->readout;
  return pressed;
}


Lamp::Lamp() {
  this->state = LAMP_BRIGHTNESS_LOW;
  this->rising = false;
  this->lastRiseUpdate = 0;
}

Lamp::Lamp(uint8_t pin) {
  this->state = LAMP_BRIGHTNESS_LOW;
  this->rising = false;
  this->lastRiseUpdate = 0;
  this->pin = pin;
  pinMode(pin, OUTPUT);
}

int Lamp::getState() {
  return this->state;
}

void Lamp::update() {
  // analogWrite(pin, map(gamma8[this->state], LAMP_BRIGHTNESS_LOW,
  //   LAMP_BRIGHTNESS_HIGH, 0, PWMRANGE));
  // Use another device to control the light
  Serial.println(gamma8[this->state]);
}

void Lamp::off() {
  if (this->rising) this->rising = false;
  this->state = LAMP_BRIGHTNESS_LOW;
  this->update();
}

void Lamp::on() {
  if (this->rising) this->rising = false;
  this->state = LAMP_BRIGHTNESS_HIGH;
  this->update();
}

void Lamp::brighten() {
  if (this->rising) this->rising = false;
  if (this->state < LAMP_BRIGHTNESS_HIGH) {
    this->state += LAMP_BRIGHTNESS_HIGH / LAMP_BRIGHTNESS_STEPS;
    if (this->state > LAMP_BRIGHTNESS_HIGH) this->state = LAMP_BRIGHTNESS_HIGH;
    this->update();
  }
}

void Lamp::dim() {
  if (this->rising) this->rising = false;
  if (this->state > LAMP_BRIGHTNESS_LOW) {
    this->state -= LAMP_BRIGHTNESS_HIGH / LAMP_BRIGHTNESS_STEPS;
    if (this->state < LAMP_BRIGHTNESS_LOW) this->state = LAMP_BRIGHTNESS_LOW;
    this->update();
  }
}

void Lamp::rise() {
  this->rising = true;
}

void Lamp::riseUpdate() {
  unsigned long now = millis();
  if (now - this->lastRiseUpdate > LAMP_RISE_UPDATE_INTERVAL) {
    this->lastRiseUpdate = now;
    if (this->state < LAMP_BRIGHTNESS_HIGH) {
      ++this->state;
      this->update();
    }
    else this->rising = false;
  }
}

boolean Lamp::isOn() {
  return this->state > LAMP_BRIGHTNESS_LOW;
}

boolean Lamp::isOff() {
  return this->state == LAMP_BRIGHTNESS_LOW;
}

boolean Lamp::isMax() {
  return this->state == LAMP_BRIGHTNESS_HIGH;
}

boolean Lamp::isRising() {
  return this->rising;
}
