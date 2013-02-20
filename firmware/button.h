/*
 *  button.h
 *
 *  Handle button presses and timing
 */

#ifndef button_h
#define button_h

#define OFF 1
#define ON 0

typedef struct{
  int newpress;
  int oldpress;

  long lastDebounceTime;
  long debounceDelay;
  
  int last_button_state;
  int button_state;
}button_t;
extern button_t button;

void button_init();

//Debounce does button debouncing mechanics
//Return 1 when button state changes
void read_button();

void toggle_state();

#endif
