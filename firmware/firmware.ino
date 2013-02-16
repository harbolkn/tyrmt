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
int timer = 0;

int oldpress = 0;
long lastDebounceTime = 0;
long debounceDelay = 50;


void setup() {
  Serial.begin(115200);
  timer = 0;
}



void loop() {
  // Check bluetooth for status update
  char ping = tracker.ping();

  //Ping here will either be '0' or '1'
  //'0' = STATUS
  //'1' = DATA
  Serial.println(ping);
  

  //Update time
  timer = timer + 1;
  Serial.println(timer);


  //Check button state
  //Debounce here
  int reading = tracker.button_io();

  if(reading != oldpress)
    lastDebounceTime = millis();

  if((millis() - lastDebounceTime) > debounceDelay){
    tracker.press_button(reading, timer);
  }

  //Handle button press
  //Set state based on amount of time button is pressed
  if(tracker.button_state() == LOW){
    //Button pressed for 3 seconds - IMU ON - Turn OFF
    if((timer - tracker.get_button_time()) == 3000 && tracker.return_state() == IMU){
      Serial.println("Stop IMU");
      tracker.set_state(OFF);
      tracker.process_state();
      tracker.press_button(HIGH, 0); //SANITY: Button has been pressed now restart counting
    }

    //Button pressed for 3 seconds - IMU OFF - Turn ON
    else if((timer - tracker.get_button_time()) == 3000 && tracker.return_state() == OFF) {
        //Serial.println("Start IMU");
        //tracker.set_state(IMU);
        //tracker.process_state();
        //tracker.press_button(HIGH, 0);
    }
  }


  //LEDs and state set
  //Run process based on state
  if (tracker.return_state() == IMU){ //IMU RECORD
    tracker.record_data();
  }
  else if (tracker.return_state() == TRANS){//DATA TRANSFER W/ BLUETOOTH
    tracker.transmit_data();
  }


  oldpress = tracker.button_state();
}


