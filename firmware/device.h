/*
 * device.h
 *
 * Device functions
 */

#ifndef device_h
#define device_h

#include <FreeSixIMU.h>
#include <FIMU_ADXL345.h>
#include <FIMU_ITG3200.h>
#include <Wire.h>


#define STANDBY 0
#define IMU 1
#define TRANS 2



typedef struct{
  int state;
  FreeSixIMU my3IMU;
  float q[4];
  int raw[6];
}device_t;
extern device_t device;

void device_init();

void record_toggle(int time);

#endif
