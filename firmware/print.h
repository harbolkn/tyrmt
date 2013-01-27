/*
 * print.h
 *
 * Modified files from CommunicationUtils.h
 * Write information to SD Card
 */

#ifndef print_h
#define print_h

#include "Arduino.h"
#include <SdFat.h>
#include <SdFatUtil.h>

#define LINE_SIZE 4

void filePrintFloatArr(SdFile file, float *arr, int length);

void fileFloatPrint(SdFile file, float f);

#endif
