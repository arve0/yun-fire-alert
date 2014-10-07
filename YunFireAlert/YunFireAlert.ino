/* TODO */

#include <Bridge.h>
#include <Console.h>
#include "MsTimer2.h"

// pins
const byte firePin = 2;
const byte failInPin = 3;
const byte failOutPin = 4;
const byte statusPin = 13;

void setup() {
  // external pullup
  pinMode(firePin, INPUT);
  // internal pullup
  pinMode(failInPin, INPUT_PULLUP);
  // relay normally open with external pullup
  pinMode(failOutPin, OUTPUT);
  // internal LED
  pinMode(statusPin, OUTPUT);

  Bridge.begin();
  Console.begin();
  Console.println("console started");
}

void toggleLed() {
  // change output of led
  static byte out = HIGH;
  digitalWrite(statusPin, out);
  out = !out;
}

// status codes
const byte ALARM = 0;
const byte FAILURE = 1;
const byte FAILURE_INTERNET = 2;
const byte FAILURE_TWILIO = 3;
const byte OK = 10;

byte getState(){
  // Get current state and return it.
  // FAILURE_INTERNET: failure internet - TODO
  // FAILURE_TWILIO: failure twilio   - TODO
  byte fire = digitalRead(firePin);
  byte failure = digitalRead(failInPin);
  delay(50); // debounce
  if (fire == 1 && digitalRead(firePin) == 1) return ALARM;
  else if (failure == 0 && digitalRead(failInPin) == 0) return FAILURE;
  else return OK;
}

void loop() {
  static byte currentState;
  static byte previousState = 10;
  // get current state
  currentState = getState();
  if (currentState != previousState) {
    switch (currentState) {
      case ALARM:
        Console.println("BRANN");
        MsTimer2::stop();
        MsTimer2::set(100, toggleLed);
        MsTimer2::start();
        break;
  
      case FAILURE:
        Console.println("FEIL");
        MsTimer2::stop();
        MsTimer2::set(1000, toggleLed);
        MsTimer2::start();
        break;
  
      case OK:
        Console.println("OK");
        MsTimer2::stop();
        digitalWrite(statusPin, LOW);
    } // end switch state
  } // end if state change
  previousState = currentState;
}
