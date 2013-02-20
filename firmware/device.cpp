/*
 *  device.cpp
 *
 *  Device control
 */

#include <SoftwareSerial.h>

#include "device.h"
#include "button.h"
#include "hardware.h"
#include "Arduino.h"
#include "record.h"
#include "har_decls.h"

device_t device;

void device_init(){
  device.state = STANDBY;

  device.my3IMU = FreeSixIMU();
  device.my3IMU.init();
}

void record_toggle(int time){
  
  if(button.button_state != button.last_button_state){
    switch(device.state){
      case STANDBY: device.state = IMU;
        Serial.println("Start IMU");
        record.fileName = String(time);
        record.fileName += ".csv";
        record.fileName.toLowerCase();
        record.fileName.replace("-", "0");
        Serial.print("Writing to ");
        Serial.println(record.fileName);
        break;
      case IMU: device.state = STANDBY;
        Serial.println("Stop IMU");
        break;
    }
  }
}

