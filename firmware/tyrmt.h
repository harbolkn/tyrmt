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

//General definitions
#define ERROR -1
#define OFF 0 //Off or receive
#define ON 1 //On or transmit

//Bluetooth Com definitions
#define STATUS 0
#define DATA 1

class Tyrmt{
private:
  int IMULED; //IMU ON/OFF
  int TRANSLED; //TRANSMIT ON/OFF
  int POWERLED; //POWER ON/OFF

  int dataButton; //Turning on/off data transmission

  int state; //State of entire system

  float button_pressed_time;

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
  int button_state();
  void process_state();

  bool record_data();
  bool transmit_data();

  void ping();

  int get_button_time();
  void press_button(int timer);
};

#endif

