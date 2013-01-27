/*
 * Acquire Data from firmware
 * Store to .csv for LabView analysis
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "serial.h"

void delay();

int main(){
  //Setup serial interface
  serial_open();
  serial_init();
  

  char input, old_input;
  FILE *fp;

  while(1){
    old_input = input;
    input = serial_read();

    if(old_input != input){
      if(input > 0){
        fp = fopen("/data/data.csv", "a+");

        fprintf(fp, "%c", input);
      }
    }

    if(input == -1) break;
  }

  serial_close();
  return 0;
}

void delay(){
  int c=1, d=1;

  for(c=1; c<=32767; c++){
    for(d=1; d<=32767; d++){}
  }
}
