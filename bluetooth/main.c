/*
 * Acquire Data from firmware
 * Store to .csv for LabView analysis
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "serial.h"



int main(int argc, char *argv[]){
  //Setup serial interface
  char *port_name = argv[1];
  serial_open(port_name);
  serial_init();

  printf("Serial setup complete\n");
  serial_write(' ');

  int first_loop = 1;
  int get_file_name = 0;
  int record_file = 0;
  char *file_name = (char *)malloc(sizeof(char)*10);
  int i=0;
  

  char input, old_input;
  FILE *fp;

  while(1){
    input = serial_read();

    if((input >= 'A' && input <='z') || (input == ' ') || (input == '\n') ||(input == '.') || (input >= '0' && input<='9') || (input == ',')){
      if(first_loop == 1){
        printf("%c", input);
      }

      //Acquire file name
      else if(get_file_name == 1){
        if(input == '\n'){
          get_file_name = 0;
          record_file = 1;
          printf("File name: %s \n", file_name);
        }
        else{
          file_name[i] = input;
          i++;
        }
      }

      //Record file
      else if(record_file == 1){
        i=0; //Should have just gotten file name

        fp = fopen(file_name, "a+");

        if(fp == NULL) printf("\nERROR: %s failed to open\n", file_name);
        else{
          fprintf(fp, "%c", input);
        
          fclose(fp);
        }
      }
      
      //Determine New File
      if(input == '\n' && old_input == '\n'){
        record_file = 0;
        printf("%c", input);
        get_file_name = 1;
      }

      old_input = input;
    }
    else{
      if(first_loop == 1){
        serial_write('2');
        first_loop = 0;
      }
      else{
        //break;
      }
    }

    
  }

  serial_close();
  return 0;
}

