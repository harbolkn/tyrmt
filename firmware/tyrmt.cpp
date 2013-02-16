/*
 * tyrmt.cpp
 *
 * Control tyrmt device
 * For all device settings and function
 */
#include <SdFat.h>
#include <SdFatUtil.h>
#include <FreeSixIMU.h>
#include <FIMU_ADXL345.h>
#include <FIMU_ITG3200.h>
#include <SoftwareSerial.h>
#include <Wire.h>

#define DEBUG
#ifdef DEBUG
#include "DebugUtils.h"
#endif

#include "CommunicationUtils.h"
#include "FreeSixIMU.h"

#include "Arduino.h"
#include "tyrmt.h"
#include "print.h"

int mybluetoothTx = 2;
int mybluetoothRx= 3;
SoftwareSerial bluetooth(mybluetoothTx, mybluetoothRx);


Tyrmt::Tyrmt(){
  //Set LED pins
  //Possibly remove POWERLED and just connect to battery?
  //Value of using DDRD/PORTD registers?
  IMULED = 7;
  TRANSLED = 6;
  POWERLED = 5;

  //Data button location
  //Enabled HIGH, on press will turn LOW
  dataButton = 4;
  button_value = HIGH;

  //Current state of the device
  //Should spend most of its time OFF
  state = OFF;
  
  //Hardware variables for microSD card
  chipSelect = 10;
  data_file = "data.csv";

  //Should debounce time be here or in the *.ino?
  button_pressed_time = 0;

  //IMU object
  my3IMU = FreeSixIMU();

  //Setup
  Wire.begin();

  my3IMU.init();

  pinMode(IMULED, OUTPUT);
  pinMode(TRANSLED, OUTPUT);
  pinMode(POWERLED, OUTPUT);

  pinMode(dataButton, INPUT);
  digitalWrite(dataButton, button_value);
  
  pinMode(chipSelect, OUTPUT);
  card.init();
  volume.init(&card);
  root.openRoot(&volume);
  
  if(!sd.begin(chipSelect, SPI_HALF_SPEED))
    sd.initErrorHalt();
  if(!file.open(data_file, O_RDWR | O_CREAT | O_AT_END))
    sd.errorHalt("opening file for write failed");

  //Bluetooth always needs to be at this baud unless the chip is changed
  //Default value for RN-42 is 115200 (see manual to change)
  bluetooth.begin(115200);

}


//Change the device state for transmitting and recording data
//
//SANITY: Device cannot be turned off by machine request for data
//        Error delivered if it is requested, keep recording
//Reason: Another user of the device cannot interrupt a recording 
//        purposefully or accidentally
void Tyrmt::set_state(int mystate){
  state = mystate;
  return_state();
}

int Tyrmt::return_state(){
  return state;
}


//Set LEDs based on state of the device
void Tyrmt::process_state(){
  switch(state){
    case IMU: digitalWrite(IMULED, HIGH);
              digitalWrite(TRANSLED, LOW);
    case TRANS: digitalWrite(IMULED, LOW);
                digitalWrite(TRANSLED, HIGH);
    default:  digitalWrite(IMULED, LOW);
              digitalWrite(TRANSLED, LOW);
  }
}

//state = IMU
//Record data from the IMU and write to microSD
//
//FIX: Add values to accelerometer
bool Tyrmt::record_data(){
  my3IMU.getQ(q);

  file.open(data_file, O_CREAT | O_APPEND | O_WRITE);
  filePrintFloatArr(file, q, 4);
  file.println("");
  file.close();

  return 1;
}


//state = TRANS
//Transmit data from microSD via bluetooth to workstation
//
//FIX: Error checking over the transmission?
//FIX: Multiple file handling
bool Tyrmt::transmit_data(){
  char toSend;

  if(file.open(data_file, O_READ) > 0){
    while((toSend = file.read()) > 0){
      bluetooth.print(toSend);
    }
    file.close();
    set_state(OFF);
    return 1;
  }
  else{
    return 0;
  }
}

//Ping computer on each loop to determine if it's trying to get information
//
//Should this send a busy/free bit back to the computer when polling?
//Should computer send a busy/free bit to the firmware?
//
//FIX: variables are ints, ping is char
char Tyrmt::ping(){
  char ping = bluetooth.read();

  if(ping == (char)STATUS + '0'){
    bluetooth.print((char) return_state());
  }
  else if(ping == (char)DATA + '0'){
    if(state == IMU) bluetooth.print(ERROR);
    else if (state == OFF) {
      set_state(TRANS);
    }
  }
  
  return ping;
}


//Digital vaue of I/O pin
int Tyrmt::button_io(){
  return digitalRead(dataButton);
}

//Return button_state
int Tyrmt::button_state(){
  return button_value;
}

//Return time button was pressed
int Tyrmt::get_button_time(){
  return button_pressed_time;
}

//Button is registered as being pressed
//
//FIX: Make sure this is consistent across the system
//     Am I calling delay() anywhere?
//     Are there cases where this will not happen uniformly?
void Tyrmt::press_button(int state, int timer){
  button_value = state;

  if(button_value == HIGH)
    button_pressed_time = timer;
}
