/*
 *  record.h
 *
 *  Handle gathering data and writing it to microSD
 */

#ifndef record_h
#define record_h

#include <SD.h>

typedef struct{
  File dataFile;
  String fileName;
} record_t;
extern record_t record;

void record_init();

void record_data();
void filePrintFloatArr(float *arr, int length);
void fileFloatPrint(float f);

#endif
