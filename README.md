# Swarog

Driver for an IoT lamp and sunrise alarm. Note, in addition to the remote
control, some functions of the lamp may also be controlled manually via
physical buttons.

## Compatible hardware

This code is written for the ESP8266 microcontroller, but it also should work,
with small modifications, with Arduino boards.

NOTE: Due to the insufficient (software-based) analog write performance of
ESP8266, it is preferable to control the light indirectly via another
controller (for example, Arduino pro mini).

## Configuration

Configure `swarog.ino` to match your WiFi and MQTT settings.
