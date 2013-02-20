/*
 *  hardware.cpp
 *
 *  Hardware setup
 *  Change this for different hardware setups
 */

#include "hardware.h"
#include "Arduino.h"

hardware_t hardware;

void hardware_init(){
  hardware.buttonPin = 4;
  hardware.powerLED = 5;
  hardware.transLED = 6;
  hardware.recordLED = 7;

  hardware.chipSelect = 10;
}

void hardware_setup(){
  pinMode(hardware.buttonPin, INPUT);
  digitalWrite(hardware.buttonPin, HIGH); //Pull button HIGH

  pinMode(hardware.powerLED, OUTPUT);
  pinMode(hardware.transLED, OUTPUT);
  pinMode(hardware.recordLED, OUTPUT);

  pinMode(hardware.chipSelect, OUTPUT);
}
