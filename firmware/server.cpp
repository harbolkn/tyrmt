/*
 * server.cpp
 *
 * Serial communication
 */

#include <SD.h>

#include "server.h"

#include "device.h"
#include "hardware.h"
#include "har_decls.h"

void ping(){
  char ping = Serial.read();

  if(ping == (char)STATUS + '0'){
    Serial.println(device.state);
  }
  else if(ping == (char)DATA + '0'){
    if(device.state == IMU){
      Serial.print("ERROR");
    }
    else if(device.state == STANDBY){
      device.state = TRANS;
    }
  }
  else if(ping == (char)RESET + '0'){
    if(device.state == IMU)
      Serial.print(ERROR);
    else{
      device.state = TRANS;
      reset_device();
      device.state = STANDBY;
    }
  }
}

void transmit_data(){
  File root = SD.open("/");
  root.seek(0);
  
  printDirectory(root, 0);
  
  root.rewindDirectory();
  root.close();
  Serial.println("Done transmitting files");
  device.state = STANDBY;
}

void reset_device(){
  File root = SD.open("/");
  root.seek(0);
    
  File entry = root.openNextFile();
  
  while(entry){
    SD.remove(entry.name()); 
    if(entry){
      entry.close();
    }
    
    entry.close();  
    entry = root.openNextFile();
  }
  
  entry.close();
  root.rewindDirectory();
  root.close();
  Serial.println("All files cleared.");
}

void printDirectory(File dir, int numTabs){
  dir.seek(0);
  
  while(true){
    File entry = dir.openNextFile();
    if(!entry){
      entry.close();
      dir.rewindDirectory();
      break;
    }
    for(uint8_t i=0; i<numTabs; i++){
      Serial.print('\t');
    }
    
    if(entry.isDirectory()){
    }
    else{
      //Print directory
      Serial.println("");
      Serial.println(entry.name());
     
      File dataFile = SD.open(entry.name());
      
      while(dataFile.available())
        Serial.write(dataFile.read());
        
      dataFile.close();
      SD.remove(entry.name());      
    }
    entry.close();
  }
}

