/*
 * firmware.ino
 *
 * arduino program for tyrmt system
 */

#include <SdFat.h>
#include <SdFatUtil.h>
#include <SdVolume.h>
#include <FreeSixIMU.h>
#include <FIMU_ADXL345.h>
#include <FIMU_ITG3200.h>
#include <SoftwareSerial.h>
#include <Wire.h>

#include "tyrmt.h"
#include "print.h"

Tyrmt tracker = Tyrmt();
double timer = 0;


void setup() {
  Serial.begin(115200);
  timer = 0;
}



void loop() {
  // Check bluetooth for status update
  tracker.ping();
  

  //Check button state
  timer = timer + 1;
  if(tracker.button_state() == HIGH){
    if(tracker.get_button_time() == 0) tracker.press_button(timer);
    else if(((timer - tracker.get_button_time()) == 1000) && (tracker.return_state() == IMU)){
        //Stop IMU recording
        tracker.set_state(OFF);
        tracker.process_state();
        tracker.press_button(0);
    }
    else if(((timer - tracker.get_button_time()) == 3000) && (tracker.return_state() == OFF)){
        //Start IMU recording
        tracker.set_state(IMU);
        tracker.process_state();
        tracker.press_button(0);
    }
  }

  //Run  
  if (tracker.return_state() == IMU){ //IMU RECORD
    tracker.record_data();
  }
  else if (tracker.return_state() == TRANS){//DATA TRANSFER W/ BLUETOOTH
    tracker.transmit_data();
  }
  else{ 
    ;//Do nothing
  }


  delay(1);
}


