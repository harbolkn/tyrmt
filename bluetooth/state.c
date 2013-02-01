/*
 * state.c
 *
 * Determine state
 * Set states
 */

state_t state;

void state_init(){
  state.transceive_state = 0;
  state.handle_state = 0;

  state.firmware_state = get_firmware_state();
}

int get_firmware_state(){
  
}

void set_transmit_mode(int trans_state){
  state.transceive_state = trans_state;
}

void set_handle_mode(int handle_state){
  state.handle_state = handle_state;
}
  

