/*
 *  firmware.ino
 *
 *  Basic firmware program for Tyrmt hardware module
 */
 
#include <FreeSixIMU.h>
#include <FIMU_ADXL345.h>
#include <FIMU_ITG3200.h>
#include <SD.h>
#include <Wire.h>
#include "Arduino.h"

#include "hardware.h"
#include "button.h"
#include "device.h"
#include "record.h"
#include "server.h"

long timer = 0;

void setup(){
  //Serial = SoftwareSerial(TX, RX, false);
  
  Serial.begin(9600);
  
  //Hardware initializations
  hardware_init();
  hardware_setup();
  
  //Button initalization
  button_init();
  
  //Device initalization
  device_init();
  record_init();
  
  //Power is on
  digitalWrite(hardware.led, HIGH);
  
  Serial.println("Card Initialized");
}

void loop(){
  //Ping
  ping();
  
  //Button
  read_button();
  
  switch(button.button_state){
    case ON: record_toggle(timer); break;
    case OFF: break;
    default: break;
  }
  
  //Run
  switch(device.state){
    case STANDBY: 
      digitalWrite(hardware.led, HIGH);
      break;
    case IMU:
      led_blink();
      record_data();
      break;
    case TRANS:
      digitalWrite(hardware.led, HIGH);
      transmit_data();
      break;
  }
  
  //Iterator
  timer++;
}

