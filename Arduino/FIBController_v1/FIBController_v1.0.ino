/*
  FIBcontroller
  Version 1.0

  Uses a combination of keyboard buttons, a rotary encoder and a toggle switch to send keyboard shortcuts. 
  Customized here for a Hitachi NX5000, with the toggle switch used to select keyboard shortcut scheme.
  Possible schemes are Classical or Modern

  created 24 June 2022
  by Aleksander B. Mosberg, SuperSTEM
  
*/
#include "Keyboard.h"

// set pin numbers for keyboard switches 1-12
const int KEY1 = 7;
const int KEY2 = 8;
const int KEY3 = 20;
const int KEY4 = 21;
const int KEY5 = 5;
const int KEY6 = 6;
const int KEY7 = 22;
const int KEY8 = 23;
const int KEY9 = 4;
const int KEY10 = 14;
// const int KEY11 = 17; No point in defining key11, pin 17 is TX LED and can't be used with internal pullup
const int KEY12 = 15;
// pins for toggle switch
const int SW1_1 = 12;
const int SW1_3 = 11;
// pins for rotary encoder
const int RE_A = 3;
const int RE_B = 2;
const int SW2 = 16;
// pin for BRB
const int BRB = 19;

// set how long keys should be sent for before triggering releaseAll
const int press_time = 100;
// how long to wait after pressing keys to prevent debounce
const int debounce_time = 250;

int rotation;
int value;
bool submain;
char mode;

void setup() {
  // initialize pins: KEY1-12 are all inputs using the internal pullup resistors
  // when connected these are all bridged to ground
  pinMode(KEY1, INPUT_PULLUP);
  pinMode(KEY2, INPUT_PULLUP);
  pinMode(KEY3, INPUT_PULLUP);
  pinMode(KEY4, INPUT_PULLUP);
  pinMode(KEY5, INPUT_PULLUP);
  pinMode(KEY6, INPUT_PULLUP);
  pinMode(KEY7, INPUT_PULLUP);
  pinMode(KEY8, INPUT_PULLUP);
  pinMode(KEY9, INPUT_PULLUP);
  pinMode(KEY10, INPUT_PULLUP);
  // pinMode(KEY11, INPUT_PULLUP); Pin cannot be used with internal pullup, TX LED.
  pinMode(KEY12, INPUT_PULLUP);
  // the same applies for SW1, SW2 and the BRB
  pinMode(SW1_1, INPUT_PULLUP);
  pinMode(SW1_3, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
  pinMode(BRB, INPUT_PULLUP);
  
  // the encoder does not use the internal pullup
  // NB: MAY HAVE TO CHANGE THIS TO MAKE RE WORK MORE RELIABLY
  pinMode(RE_A, INPUT);
  pinMode(RE_B, INPUT);

  // read initial state of encoder
  rotation = digitalRead(RE_A);

  // set initial state of encoder action
  submain = false; // defined as false: sub controller, true: main controller

  // start emulating keyboard
  Keyboard.begin();
}



void loop() {
  
  // first, if the BRB is pressed send the stop stage command
  if (digitalRead(BRB) == LOW) {
    Keyboard.press(KEY_PAUSE);
    delay(press_time);
    Keyboard.releaseAll();
    delay(debounce_time);
  }

  // second, check which mode/keymap we are in
  if (digitalRead(SW1_3) == LOW) {
    mode = 'c'; // classical keymap
  }
  else if (digitalRead(SW1_1) == LOW) {
    mode = 'm'; // modern keymap
  
  }
  else { // neither pole is shorted, switch is in center 
    mode = 'o'; // neutral/'off' mode
  }
  
  // if the encoder wheel is pressed, toggle between sub and main magnification shortcuts
  if (digitalRead(SW2) == LOW) {
    submain = !submain;
    delay(debounce_time);
  }

  // first we check the encoder for changes
    value = digitalRead(RE_A);
      if (value != rotation) {
        rotation = value;
        if (digitalRead(RE_B) != value) { // RE is going clockwise
          if (mode == 'c') { // classical keymap
            if (submain == false) { // sub controller
              Keyboard.press(KEY_LEFT_CTRL);
              Keyboard.press(KEY_LEFT_SHIFT);
              Keyboard.press(KEY_KP_PLUS);
            }
            else { // main controller
              Keyboard.press(KEY_LEFT_CTRL);
              Keyboard.press(KEY_KP_PLUS);
            }
          }
          else if (mode == 'm') { // modern keymap
            if (submain == false) { // sub controller
              Keyboard.press(KEY_LEFT_SHIFT);
              Keyboard.press(KEY_LEFT_ALT);
              Keyboard.press(KEY_KP_PLUS);
            }
            else { // main controller
              Keyboard.press(KEY_LEFT_ALT);
              Keyboard.press(KEY_KP_PLUS);
            }
          }
          else { // mode is neither 'c' or 'm'. Should be 'n', so no action
            return;
          }
        }
        else { // RE is going counterclockwise
          if (mode == 'c') { // classical keymap
            if (submain ==false) { // sub controller
              Keyboard.press(KEY_LEFT_CTRL);
              Keyboard.press(KEY_LEFT_SHIFT);
              Keyboard.press(KEY_KP_MINUS);  
            }
            else { // main controller
              Keyboard.press(KEY_LEFT_CTRL);
              Keyboard.press(KEY_KP_MINUS);  
            }
          }
          else if (mode == 'm') { //modern keymap
            if (submain == false) { // sub controller
              Keyboard.press(KEY_LEFT_SHIFT);
              Keyboard.press(KEY_LEFT_ALT);
              Keyboard.press(KEY_KP_MINUS);
            }
            else { // main controller
              Keyboard.press(KEY_LEFT_ALT);
              Keyboard.press(KEY_KP_MINUS);
            }
          }
          else { // mode is neither 'c' or 'm'. Should be 'n', so no action
            return;
          }
        }
        delay(25); // small delays here so we can keep spinning
        Keyboard.releaseAll();
        delay(25);
        return; // restart the loop without checking anything else, to avoid any other wait times and be ready for the next trigger
      }

  // 'Classical' mode
  if (mode == 'c') {
    if (digitalRead(KEY1) == LOW) { // Key1: start/stop scanning sub
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press(KEY_F12);
      delay(press_time);
      Keyboard.releaseAll();
      delay(debounce_time);
      }
    else if (digitalRead(KEY2) == LOW) { // Key2: single frame sub
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press(KEY_F11);
      delay(press_time);
      Keyboard.releaseAll();
      delay(debounce_time);
      }
    else if (digitalRead(KEY3) == LOW) { // Key3: start/stop scanning main
      Keyboard.press(KEY_F12);
      delay(press_time);
      Keyboard.releaseAll();
      delay(debounce_time);
      }
    else if (digitalRead(KEY4) == LOW) { // Key4: single frame Main
      Keyboard.press(KEY_F11);
      delay(press_time);
      Keyboard.releaseAll();
      delay(debounce_time);
      }
    else if (digitalRead(KEY5) == LOW) { // Key5: reduce scan sub
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press(KEY_F4);
      delay(press_time);
      Keyboard.releaseAll();
      delay(debounce_time);
      }
    else if (digitalRead(KEY6) == LOW) { // Key6: ACB sub
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_F9);
      delay(press_time);
      Keyboard.releaseAll();
      delay(debounce_time);
      }
    else if (digitalRead(KEY7) == LOW) { // Key7: reduce scan main
      Keyboard.press(KEY_F4);
      delay(press_time);
      Keyboard.releaseAll();
      delay(debounce_time);
      }
    else if (digitalRead(KEY8) == LOW) { // Key8: ACB main
      Keyboard.press(KEY_F9);
      delay(press_time);
      Keyboard.releaseAll();
      delay(debounce_time);
      }
    else if (digitalRead(KEY9) == LOW) { // Key9: photo scan sub
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_F11);
      delay(press_time);
      Keyboard.releaseAll();
      delay(debounce_time);
      }
    else if (digitalRead(KEY10) == LOW) { // Key10: photo scan main
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_F11);
      delay(press_time);
      Keyboard.releaseAll();
      delay(debounce_time);
      }
      // NB: KEY 11 IS DISABLED, AS THE PIN IT CONNECTS TO DRIVES AN INTERNAL LED
      // THROUGH A 1K RESISTOR MAKING THE INTERNAL PULLUP USELESS.
//    else if (digitalRead(KEY11) == LOW) { // Key11: eucentric calc display
//      Keyboard.press(KEY_LEFT_CTRL);
//      Keyboard.press(KEY_F1);
//      delay(press_time);
//      Keyboard.releaseAll();
//      delay(debounce_time);
//      }
    else if (digitalRead(KEY12) == LOW) { // Key12: MPS controller display
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('3');
      delay(press_time);
      Keyboard.releaseAll();
      delay(debounce_time);
      }
    else {
      delay(50);
    }
  }
  // If 'modern' mode
  else if (mode == 'm') {
        if (digitalRead(KEY1) == LOW) { // Key1: start/stop scanning sub
      Keyboard.press(KEY_F11);
      delay(press_time);
      Keyboard.releaseAll();
      delay(debounce_time);
      }
    else if (digitalRead(KEY2) == LOW) { // Key2: single frame sub
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_F11);
      delay(press_time);
      Keyboard.releaseAll();
      delay(debounce_time);
      }
    else if (digitalRead(KEY3) == LOW) { // Key3: start/stop scanning main
      Keyboard.press(KEY_F12);
      delay(press_time);
      Keyboard.releaseAll();
      delay(debounce_time);
      }
    else if (digitalRead(KEY4) == LOW) { // Key4: single frame main
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_F12);
      delay(press_time);
      Keyboard.releaseAll();
      delay(debounce_time);
      }
    else if (digitalRead(KEY5) == LOW) { // Key5: reduce scan sub
      Keyboard.press(KEY_F7);
      delay(press_time);
      Keyboard.releaseAll();
      delay(debounce_time);
      }
    else if (digitalRead(KEY6) == LOW) { // Key6: ACB sub
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press('4');
      delay(press_time);
      Keyboard.releaseAll();
      delay(debounce_time);
      }
    else if (digitalRead(KEY7) == LOW) { // Key7: reduce scan main
      Keyboard.press(KEY_F8);
      delay(press_time);
      Keyboard.releaseAll();
      delay(debounce_time);
      }
    else if (digitalRead(KEY8) == LOW) { // Key8: ACB main
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('4');
      delay(press_time);
      Keyboard.releaseAll();
      delay(debounce_time);
      }
    else if (digitalRead(KEY9) == LOW) { // Key9: photo scan sub
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press(KEY_F11);
      delay(press_time);
      Keyboard.releaseAll();
      delay(debounce_time);
      }
    else if (digitalRead(KEY10) == LOW) { // Key10: photo scan main
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press(KEY_F12);
      delay(press_time);
      Keyboard.releaseAll();
      delay(debounce_time);
      }
      // NB: KEY 11 IS DISABLED, AS THE PIN IT CONNECTS TO DRIVES AN INTERNAL LED
      // THROUGH A 1K RESISTOR MAKING THE INTERNAL PULLUP USELESS.
//    else if (digitalRead(KEY11) == LOW) { // Key11: eucentric calc display
//      Keyboard.press(KEY_LEFT_CTRL);
//      Keyboard.press(KEY_LEFT_ALT);
//      Keyboard.press(KEY_F1);
//      delay(press_time);
//      Keyboard.releaseAll();
//      delay(debounce_time);
//      }
    else if (digitalRead(KEY12) == LOW) { // Key12: MPS controller display
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press('3');
      delay(press_time);
      Keyboard.releaseAll();
      delay(debounce_time);
      }
    else {
      delay(50);
    }
  }
}
