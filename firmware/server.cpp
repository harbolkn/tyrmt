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
  File entry = root.openNextFile();
  
  if(!entry){
    Serial.println("No files present.");
    device.state = STANDBY;
    return;
  }
  
  bool wait = 0;
  
  while(entry){
    
    if(wait == 0){
      Serial.print("Open/Delete File: ");
      Serial.print(entry.name());
      Serial.print("? (Y/N/D) ");
    }
    
    char response = Serial.read();
    
    if(response == 'Y' || response == 'y'){
      Serial.println("");
      File dataFile = SD.open(entry.name());
      
      while(dataFile.available())
        Serial.write(dataFile.read());
        
      dataFile.close();
      entry = root.openNextFile();
      wait = 0;
    }
    else if(response == 'N' || response == 'n'){
      Serial.println("");
      entry = root.openNextFile();
      wait = 0;
    }
    else if(response == 'D' || response == 'd'){
      Serial.println("");
      SD.remove (entry.name());
      entry = root.openNextFile();
      wait = 0;
    }
    else{
      wait = 1;
    }
  }
  
  root.rewindDirectory();
  root.close();
  Serial.println("Done transmitting files");
  device.state = STANDBY;
}

void reset_device(){
  File root = SD.open("/");
  File entry = root.openNextFile();
  
  while(entry){
    SD.remove(entry.name()); 
    entry = root.openNextFile();
  }
  
  root.rewindDirectory();
  root.close();
  Serial.println("All files cleared.");
}
