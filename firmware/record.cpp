/*
 * record.h
 *
 * Get data from IMU
 * Write data to microSD
 */
 
#include "record.h"

#include "device.h"
#include "hardware.h"
#include "har_decls.h"

#include "CommunicationUtils.h"
#include <Wire.h>

record_t record;

void record_init(){
  if(!SD.begin(hardware.chipSelect)){
    //Serial.println("Card failed, or not present");
    return;
    
  }
  else{
    //Serial.println("Card initialized.");
    ;
  }
}

void record_data(){
  device.my3IMU.getQ(device.q);
  device.my3IMU.getRawValues(device.raw);
  
  char file[record.fileName.length()+1];
  record.fileName.toCharArray(file, sizeof(file));
  
  record.dataFile = SD.open(file, FILE_WRITE);

  if(!record.dataFile){
    //Serial.println("Error opening file");
  }
  else{
    filePrintFloatArr(device.q, 4);
    filePrintFloatArr((float *)device.raw, 6);
    record.dataFile.println("");
    record.dataFile.close();
  }
  
  delay(10);
}

void filePrintFloatArr(float *arr, int length){
  for(int i = 0; i<length; i++){
    fileFloatPrint(arr[i]);
    record.dataFile.print(",");
  }
}

void fileFloatPrint(float f){
  byte *b = (byte *)&f;
  for(int i = 0; i<4; i++){
    byte b1 = (b[i] >> 4) & 0x0f;
    byte b2 = (b[i] & 0x0f);

    char c1 = (b1 < 10) ? ('0' + b1) : 'A' + b1 - 10;
    char c2 = (b2 < 10) ? ('0' + b2) : 'A' + b2 - 10;

    record.dataFile.print(c1);
    record.dataFile.print(c2);
  }
}
