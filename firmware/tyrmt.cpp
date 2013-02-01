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

int bluetoothTx = 2;
int bluetoothRx= 3;
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);


Tyrmt::Tyrmt(){
  //Set LED pins
  IMULED = 7;
  TRANSLED = 6;
  POWERLED = 5;

  //Data button location
  dataButton = 4; //Pin for data Button

  state = OFF;
  
  chipSelect = 10;
  data_file = "data.csv";

  button_pressed_time = 0;

  my3IMU = FreeSixIMU();

  //Setup
  Wire.begin();

  my3IMU.init();

  pinMode(IMULED, OUTPUT);
  pinMode(IMUButton, INPUT);
  pinMode(dataLED, OUTPUT);
  pinMode(dataButton, INPUT);
  
  pinMode(chipSelect, OUTPUT);
  card.init();
  volume.init(&card);
  root.openRoot(&volume);
  
  if(!sd.begin(chipSelect, SPI_HALF_SPEED))
    sd.initErrorHalt();
  if(!file.open(data_file, O_RDWR | O_CREAT | O_AT_END))
    sd.errorHalt("opening file for write failed");

  bluetooth.begin(9600);

}


void Tyrmt::set_state(int mystate){
  state = mystate;
  return_state();
}

int Tyrmt::return_state(){
  return state;
}

int Tyrmt::button_state(){
  return digitalRead(dataButton);
}

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

bool Tyrmt::record_data(){
  my3IMU.getQ(q);
  //Expand this for more information

  file.open(data_file, O_CREAT | O_APPEND | O_WRITE);
  filePrintFloatArr(file, q, 4);
  file.println("");
  file.close();

  return 1;
}

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

bool Tyrmt::standby(){
  //Put Arduino into standby mode
}

void Tyrmt::ping(){
  char ping = bluetooth.read();

  if(ping == STATUS){
    bluetooth.print((char) return_state());
  }
  else if(ping == DATA){
    if(state == IMU) bluetooth.print(ERROR);
    else if (state == OFF) {
      set_state(TRANS);
    }
    else //Do nothing

    //This will need to be changed with standby interrupts I think
  }
  else //Do nothing
}

int Tyrmt::get_button_time(){
  return button_pressed_time;
}

void Tyrmt::press_button(int timer){
  button_pressed_time = timer;
}
