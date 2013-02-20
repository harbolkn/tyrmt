/*
 *  button.cpp
 *
 *  Button handling
 */

#include "button.h"
#include "hardware.h"
#include "Arduino.h"

button_t button;

void button_init(){
  button.newpress = 0;
  button.oldpress = 0;

  button.lastDebounceTime = 0;
  button.debounceDelay = 1000;

  button.button_state = OFF;
  button.last_button_state = OFF;
}

void read_button(){
  button.newpress = digitalRead(hardware.buttonPin);

  if(button.newpress!= button.oldpress)
    button.lastDebounceTime = millis();

  if((millis() - button.lastDebounceTime) > button.debounceDelay){
      button.last_button_state = button.button_state;
      button.button_state = button.newpress;
  }

  button.oldpress = button.newpress;
}
