/*
  FIBcontroller
  Version 1.1

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
// No point in defining key11, pin 17 is TX LED and can't be used with internal pullup
// const int KEY11 = 17;
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
const int press_time = 10;
// how long to wait after pressing keys to prevent debounce
const int debounce_time = 15;

int rotation;
int value;
bool submain;
char mode;
int encstate;
int oldstate;

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

  //setup serial for debugging
  // Serial.begin(9600);

  // the encoder does not use the internal pullup
  // NB: MAY HAVE TO CHANGE THIS TO MAKE RE WORK MORE RELIABLY
  pinMode(RE_A, INPUT_PULLUP);
  pinMode(RE_B, INPUT_PULLUP);

  // read initial state of encoder
  rotation = digitalRead(RE_A);
  encstate = digitalRead(RE_A) | (digitalRead(RE_B) << 1);
  oldstate = encstate;

  // set initial state of encoder action
  submain = false; // defined as false: sub controller, true: main controller

  // set up interrupts for rotary encoder
  attachInterrupt(digitalPinToInterrupt(RE_A), readencoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RE_B), readencoder, CHANGE);

  // start emulating keyboard
  Keyboard.begin();
}

// Utility function to press buttons until they are released, with a little debounce
void buttonpress(int keypin = 0, int usbkey1 = 0, int usbkey2 = 0, int usbkey3 = 0, int dbtime = 10) {
  if (keypin != 0)
    if (usbkey1 != 0)
      Keyboard.press(usbkey1);
  if (usbkey2 != 0)
    Keyboard.press(usbkey2);
  if (usbkey3 != 0)
    Keyboard.press(usbkey3);
  delay(dbtime);
  while (digitalRead(keypin) == LOW)
    delay(dbtime);
  Keyboard.releaseAll();
  delay(dbtime);
}

void loop() {

  // first, if the BRB is pressed send the stop stage command
  if (digitalRead(BRB) == LOW) {
    buttonpress(BRB, KEY_PAUSE);
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
    while (digitalRead(SW2) == LOW) {
      delay(25);
    }
    delay(debounce_time);
  }

  // 'Classical' mode
  if (mode == 'c') {
    if (digitalRead(KEY1) == LOW) { // Key1: start/stop scanning sub
      buttonpress(KEY1, KEY_LEFT_SHIFT, KEY_F12);
    }
    else if (digitalRead(KEY2) == LOW) { // Key2: single frame sub
      buttonpress(KEY2, KEY_LEFT_SHIFT, KEY_F11);
    }
    else if (digitalRead(KEY3) == LOW) { // Key3: start/stop scanning main
      buttonpress(KEY3, KEY_F12);
    }
    else if (digitalRead(KEY4) == LOW) { // Key4: single frame Main
      buttonpress(KEY4, KEY_F11);
    }
    else if (digitalRead(KEY5) == LOW) { // Key5: reduce scan sub
      buttonpress(KEY5, KEY_LEFT_SHIFT, KEY_F4);
    }
    else if (digitalRead(KEY6) == LOW) { // Key6: ACB sub
      buttonpress(KEY6, KEY_LEFT_SHIFT, KEY_F9);
    }
    else if (digitalRead(KEY7) == LOW) { // Key7: reduce scan main
      buttonpress(KEY7, KEY_F4);
    }
    else if (digitalRead(KEY8) == LOW) { // Key8: ACB main
      buttonpress(KEY8, KEY_F9);
    }
    else if (digitalRead(KEY9) == LOW) { // Key9: photo scan sub
      buttonpress(KEY9, KEY_LEFT_SHIFT, KEY_LEFT_CTRL, KEY_F11);
    }
    else if (digitalRead(KEY10) == LOW) { // Key10: photo scan main
      buttonpress(KEY10, KEY_LEFT_CTRL, KEY_F11);
    }
    // NB: KEY 11 IS DISABLED, AS THE PIN IT CONNECTS TO DRIVES AN INTERNAL LED
    // THROUGH A 1K RESISTOR MAKING THE INTERNAL PULLUP USELESS.
    // else if (digitalRead(KEY11) == LOW) { // Key11: eucentric calc display
    //   buttonpress(KEY11, KEY_F1);
    // }
    else if (digitalRead(KEY12) == LOW) { // Key12: MPS controller display
      buttonpress(KEY12, KEY_LEFT_CTRL, 51); // 51 is ASCII DEC code for '3'
    }
  }
  // If 'modern' mode
  else if (mode == 'm') {
    if (digitalRead(KEY1) == LOW) { // Key1: start/stop scanning sub
      buttonpress(KEY1, KEY_F11);
    }
    else if (digitalRead(KEY2) == LOW) { // Key2: single frame sub
      buttonpress(KEY2, KEY_LEFT_CTRL, KEY_F11);
    }
    else if (digitalRead(KEY3) == LOW) { // Key3: start/stop scanning main
      buttonpress(KEY3, KEY_F12);
    }
    else if (digitalRead(KEY4) == LOW) { // Key4: single frame main
      buttonpress(KEY4, KEY_LEFT_CTRL, KEY_F12);
    }
    else if (digitalRead(KEY5) == LOW) { // Key5: reduce scan sub
      buttonpress(KEY5, KEY_F7);
    }
    else if (digitalRead(KEY6) == LOW) { // Key6: ACB sub
      buttonpress(KEY6, KEY_LEFT_ALT, 52); // 52 is ASCII DEC code for '4'
    }
    else if (digitalRead(KEY7) == LOW) { // Key7: reduce scan main
      buttonpress(KEY7, KEY_F8);
    }
    else if (digitalRead(KEY8) == LOW) { // Key8: ACB main
      buttonpress(KEY8, KEY_LEFT_CTRL, 52); // 52 is ASCII DEC code for '4'
    }
    else if (digitalRead(KEY9) == LOW) { // Key9: photo scan sub
      buttonpress(KEY9, KEY_LEFT_SHIFT, KEY_F11);
    }
    else if (digitalRead(KEY10) == LOW) { // Key10: photo scan main
      buttonpress(KEY10, KEY_LEFT_SHIFT, KEY_F12);
    }
    // NB: KEY 11 IS DISABLED, AS THE PIN IT CONNECTS TO DRIVES AN INTERNAL LED
    // THROUGH A 1K RESISTOR MAKING THE INTERNAL PULLUP USELESS.
    // else if (digitalRead(KEY11) == LOW) { // Key11: eucentric calc display
    //   buttonpress(KEY11, KEY_LEFT_CTRL, KEY_LEFT_ALT, KEY_F1);
    // }
    else if (digitalRead(KEY12) == LOW) { // Key12: MPS controller display
      buttonpress(KEY12, KEY_LEFT_CTRL, KEY_LEFT_ALT, 51); // 51 is ASCII DEC code for '3'
    }
  }
}

// Serial printouts to put in interrupt for debugging
//    Serial.print(F("RE_A: "));
//    Serial.print(digitalRead(RE_A));
//    Serial.print(F("  RE_B: "));
//    Serial.print(digitalRead(RE_B));
//    Serial.print(F("  encstate: "));
//    Serial.print(encstate);
//    Serial.print(F("  oldstate: "));
//    Serial.println(oldstate);

void readencoder() {
  encstate = digitalRead(RE_A) | (digitalRead(RE_B) << 1);
  // at ratchet
  if (encstate == 3) {
    // Clockwise
    if (oldstate == 1) {
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
    // Counterclockwise
    else if (oldstate == 2) {
      if (mode == 'c') { // classical keymap
        if (submain == false) { // sub controller
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
    Keyboard.releaseAll();
  }
  oldstate = encstate;
}
