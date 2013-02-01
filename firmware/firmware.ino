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

Tyrmt myTracker;
double timer = 0;


void setup() {
  Serial.begin(115200);
  myTracker = Tyrmt();
  timer = 0;
}



void loop() {
  // Check bluetooth for status update
  Tyrmt.ping();
  

  //Check button state
  timer = timer + 1;
  if(myTracker.button_state() == HIGH){
    if(myTracker.get_button_time() == 0) myTracker.press_button(timer);
    else if(((timer - myTracker.get_button_timer()) == 1000) && (myTracker.return_statex == IMU)){
        myTracker.set_state(OFF);
        myTracker.process_state();
        myTracker.press_button(0);
    }
    else if(((timer - myTracker.get_button_timer()) == 3000) && (myTracker.return_state == OFF)){
        myTracker.set_state(IMU);
        myTracker.process_state();
        myTracker.press_button(0);
    }
  }

  //Run  
  if (myTracker.return_state() == IMU){ //IMU RECORD
    myTracker.record_data();
  }
  else if (myTracker.return_state() == TRANS){//DATA TRANSFER W/ BLUETOOTH
    myTracker.transmit_data();
  }
  else{ 
    //Do nothing
  }


  delay(1);
}


