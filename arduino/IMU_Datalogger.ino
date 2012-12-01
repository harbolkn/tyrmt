#include <SdFat.h>
#include <SdFatUtil.h>
#include <FreeSixIMU.h>
#include <FIMU_ADXL345.h>
#include <FIMU_ITG3200.h>
#include <SoftwareSerial.h>

//BLUETOOTH VARIABLES
int bluetoothTx = 2;
int bluetoothRx = 3;

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

//IMU SETUP
#define DEBUG
#ifdef DEBUG
#include "DebugUtils.h"
#endif

#include "CommunicationUtils.h"
#include "FreeSixIMU.h"

#include <Wire.h>

//IMU VARIABLES
float q[4];
FreeSixIMU my3IMU = FreeSixIMU();

//SD VARIABLES
const int chipSelect = 10;
SdFat sd;
Sd2Card card;
SdVolume volume;
SdFile root;
SdFile file;

char name[] = "data.csv";

//BUTTONS
const int IMUButton = 7;
const int IMULED = 6;
const int dataLED = 5;
const int dataButton = 4;

char state = '0';
int IMUButtonState;
int IMUledState;

int dataButtonState;
int dataledState = HIGH;

void setup() {
  Wire.begin();
  
  //SD CARD INIT
  pinMode(chipSelect, OUTPUT); //SD Card output
  card.init();
  volume.init(card);
  root.openRoot(volume);
  
  if(!sd.begin(chipSelect, SPI_HALF_SPEED))
    sd.initErrorHalt();
  if(!file.open(name, O_RDWR | O_CREAT | O_AT_END)) 
    sd.errorHalt("opening file for write failed");
  file.close();
  
  //IMU INIT
  delay(5);
  my3IMU.init();
  delay(5);
  
  //BUTTON INIT
  pinMode(IMULED, OUTPUT);
  pinMode(IMUButton, INPUT);
  pinMode(dataLED, OUTPUT);
  pinMode(dataButton, INPUT);
  
  //BLUETOOTH INIT
  bluetooth.begin(9600);
  
  //Serial
  Serial.begin(115200);
}





//State Definitions
//0 - OFF
//1 - IMU RECORDING
//2 - DATA TRANSFER

void loop() {
  int IMUButtonState = digitalRead(IMUButton);
  int dataButtonState = digitalRead(dataButton); 
  
  //State  
  if(dataButtonState == HIGH){
    IMUButtonState = LOW;
    state = '2';
  }
  else if(IMUButtonState == HIGH){
    dataButtonState = LOW;
    state = '1';
  }
  else
    state = '0';
    

  digitalWrite(IMULED, IMUButtonState);
  digitalWrite(dataLED, dataButtonState);
  
  
  //Run  
  if(state == '1'){ //IMU RECORD
    bluetooth.println("State 1");
    my3IMU.getQ(q);
  
    file.open(root, name, O_CREAT | O_APPEND | O_WRITE);
    filePrintFloatArr(q, 4);
    file.println("");
    file.close();
    
    delay(250);
  }
  else if(state == '2'){//DATA TRANSFER W/ BLUETOOTH
    char toSend = 0;
    
    if(file.open(name, O_READ) > 0){
      while((toSend = file.read()) > 0) {
        bluetooth.print(toSend);
        wordCounter(toSend);
      }
      file.close();
      state = '0';
      dataButtonState = 0;
    }
    else
      bluetooth.println("Error opening file");
    
    delay(250);
  }
  else if (state == '0'){ //DEVICE STANDBY MODE
    delay(250);
  }
  
  digitalWrite(dataLED, LOW);
}


const int word_per_line = 4;
int current_word = 0;

void wordCounter(char car){
  if(car == ','){
    current_word++;
    bluetooth.print(" ");
  }
  if(current_word == word_per_line){
    current_word = 0;
  }
}



//Metadata for writing to a file
void filePrintFloatArr(float *arr, int length) {
  for(int i=0; i<length; i++) {
    fileFloatPrint(arr[i]);
    file.print(",");
  }
}

void fileFloatPrint(float f) {
  byte *b = (byte *) &f;
  for(int i=0; i<4; i++) {
    
    byte b1 = (b[i] >> 4) & 0x0f;
    byte b2 = (b[i] & 0x0f);
    
    char c1 = (b1 < 10) ? ('0' + b1) : 'A' + b1 - 10;
    char c2 = (b1 < 10) ? ('0' + b2) : 'A' + b1 - 10;
    
    file.print(c1);
    file.print(c2);
  }
}
