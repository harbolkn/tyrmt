/*
 *  hardware.h
 *
 *  Values for hardware setup of the device
 *  Functions to call during setup to initialize hardware
 */

#ifndef hardware_h
#define hardware_h

typedef struct{
  int buttonPin;
  int powerLED;
  int transLED;
  int recordLED;

  int chipSelect;
}hardware_t;
extern hardware_t hardware;

void hardware_init();
void hardware_setup();

#endif
