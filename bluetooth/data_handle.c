/*
 * data_handle.c
 *
 * Gets data from arduino through serial
 * Writes data to file data.csv
 *
 * This will need to change later to be broken up into different files
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "serial.h"

void get_data(){
  FILE *fp;
  char input, old_input;

  while((input=serial_read()) > 0){
    fp = fopen("/data/data.csv", "a+");
    fprintf(fp, "%c", input);
  }
}
