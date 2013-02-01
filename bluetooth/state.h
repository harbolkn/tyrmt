/*
 * state.h
 *
 * Functions for determining software state
 * Also used for setting hardware states via serial
 */

#ifndef state_h
#define state_h

#define TRANSMIT 0
#define RECEIVE 1

#define DATA 0
#define SETTINGS 1

#define STATUS 1
#define DATA 2
#define WAIT 3

typedef struct{
  int transceive_state;
  int handle_state;
  int firmware_state;
}state_t;
extern state_t state;

void state_init();

int get_firmware_state();

void set_transmit_mode(int trans_state);

void set_handle_mode(int handle_state);

#endif
