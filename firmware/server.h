/*
 * server.h
 *
 * Handle bluetooth communication
 */

#ifndef server_h
#define server_h

#define ERROR 0
#define STATUS 1
#define DATA 2
#define RESET 3

void ping();

void transmit_data();
void reset_device();

#endif
