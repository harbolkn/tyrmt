/*
 * tyrmt.h
 *
 * Sets up Tyrmt pin constants and variables
 * Declares all functions of the tyrmt device
 */

#ifndef setup_h
#define setup_h

#include "Arduino.h"
#include <SdFat.h>
#include <SdFatUtil.h>

//State definitions
#define OFF 0
#define IMU 1
#define TRANS 2

//General defitions
#define OFF 0 //Off or receive
#define ON 1 //On or transmit

class Tyrmt{
private:
  int IMUButton; //Used for turning on/off the IMU
  int IMULED; //Turns on/off LED based on IMU state
  int IMUState; //Current state of the IMU (on/off)

  int dataButton; //Turning on/off data transmission
  int dataLED; //turn on/off LED based on if transmitting data
  int dataState; //Current state of transmitter (transmit/recieve)

  int state; //State of entire system

  //IMU Data
  //Maybe make this into another file?
  float q[4];
  FreeSixIMU my3IMU; 
  
  //SD Card data
  int chipSelect; //pin for chip select
  SdFat sd;
  Sd2Card card;
  SdVolume volume;
  SdFile root;
  SdFile file;

  char *data_file;
  
public:
  Tyrmt();

  void set_state(int state);
  int return_state();
  int get_state();

  bool record_data();
  bool transmit_data();
  bool stall();
  bool standby();
};

#endif

