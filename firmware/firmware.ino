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

void setup() {
  myTracker = Tyrmt(); 
}



void loop() {
  //Determine State
  myTracker.get_state();

  int state = myTracker.return_state();
  
  //Run  
  if (state == IMU){ //IMU RECORD
    myTracker.record_data();
  }
  else if (state == TRANS){//DATA TRANSFER W/ BLUETOOTH
    myTracker.transmit_data();
  }
  else if (state == OFF){ //DEVICE STANDBY MODE
    myTracker.stall();
  }
  else{
    myTracker.standby();
  }
}

