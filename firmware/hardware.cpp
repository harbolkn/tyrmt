/*
 *  hardware.cpp
 *
 *  Hardware setup
 *  Change this for different hardware setups
 */

#include "hardware.h"
#include "Arduino.h"
#include "device.h"

hardware_t hardware;

void hardware_init(){
  hardware.buttonPin = 4;
  hardware.led=5;

  hardware.chipSelect = 10;
}

void hardware_setup(){
  pinMode(hardware.buttonPin, INPUT);
  digitalWrite(hardware.buttonPin, HIGH); //Pull button HIGH

  pinMode(hardware.led, OUTPUT);

  pinMode(hardware.chipSelect, OUTPUT);
}

void led_blink(){
    digitalWrite(hardware.led, HIGH);
    delay(250);
    digitalWrite(hardware.led, LOW);
    delay(250);
}
