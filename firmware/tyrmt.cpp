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


Tyrmt::Tyrmt(int IMUButtonpin /*=7*/, int IMULEDpin /*=6*/, int dataButtonpin /*=4*/, int dataLEDpin /*=5*/){

  //Variable instantiation
  IMUButton = IMUButtonpin; //Pin for IMU Button
  IMULED = IMULEDpin; //Pin for IMU LED
  dataButton = dataButtonpin; //Pin for data Button
  dataLED = dataLEDpin; //Pin for data LED

  IMUState = OFF;
  dataState = OFF;
  state = OFF;
  
  chipSelect = 10;
  data_file = "data.csv";

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
  Serial.begin(115200);

}


void Tyrmt::set_state(int mystate){
  state = mystate;

  if(state == TRANS){
    IMUState = OFF;
    dataState = ON;
  }
  else if(state == IMU){
    IMUState = ON;
    dataState = OFF;
  }
  else{
    IMUState = OFF;
    dataState = OFF;
  }

  return_state();
}

int Tyrmt::return_state(){
  Serial.println("State: " + state);

  return state;
}

int Tyrmt::get_state(){
  if(digitalRead(dataButton) == HIGH){
    set_state(TRANS);
    digitalWrite(IMULED, LOW);
    digitalWrite(dataLED, HIGH);
  }
  else if(digitalRead(IMUButton) == HIGH){
    set_state(IMU);
    digitalWrite(IMULED, HIGH);
    digitalWrite(dataLED, LOW);
  }
  else{
    set_state(OFF);
    digitalWrite(IMULED, LOW);
    digitalWrite(dataLED, LOW);
  }

  return return_state();
}

bool Tyrmt::record_data(){
  my3IMU.getQ(q);

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
    Serial.println("Error: File not opened");
    return 0;
  }
}

bool Tyrmt::stall(){
  int temp_state = return_state();

  set_state(OFF);
  delay(500); //Delay half second
  set_state(temp_state);

  return 1;
}

bool Tyrmt::standby(){
  //Put Arduino into standby mode
  return 1;
}

