/*
 * print.cpp
 *
 * Print data to file
 */
 
#include "print.h"

void filePrintFloatArr(SdFile file, float *arr, int length){
  for(int i=0; i<length; i++){
    fileFloatPrint(file, arr[i]);
    file.print(",");
  }
}

void fileFloatPrint(SdFile file, float f){
  byte *b = (byte *) &f;
  for(int i=0; i<4; i++){
     byte b1 = (b[i] >> 4) & 0x0f;
     byte b2 = (b[i] & 0x0f);

     char c1 = (b1 < 10) ? ('0' + b1) : 'A' + b1 - 10;
     char c2 = (b2 < 10) ? ('0' + b2) : 'A' + b2 - 10;

     file.print(c1);
     file.print(c2);
  }
}
