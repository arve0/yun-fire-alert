/* 
yun-fire-alert
Author: Arve Seljebu
Code: https://github.com/arve0/yun-fire-alert

Check inputs and alert upon fire alarm or failure.
*/
#include <Bridge.h>
#include <Console.h>
#include <Process.h>
#include "MsTimer2.h"

// pins
const byte firePin = 2;
const byte failInPin = 3;
const byte failOutPin = 4;
const byte statusPin = 13;

void setup() {
  // relay normally open with external pullup
  pinMode(failOutPin, OUTPUT);
  // set failure upon upstart
  digitalWrite(failOutPin, HIGH);
  blink(1000);
  // external pullup
  pinMode(firePin, INPUT);
  // internal pullup
  pinMode(failInPin, INPUT_PULLUP);
  // internal LED
  pinMode(statusPin, OUTPUT);

  Bridge.begin();
  Console.begin();
}

void toggleStatus() {
  // toggle statusPin
  static byte out = HIGH;
  digitalWrite(statusPin, out);
  out = !out;
}

void blink(unsigned int period){
  if (period == 0){
    MsTimer2::stop();
    digitalWrite(statusPin, LOW);
  }
  else {
    MsTimer2::stop();
    MsTimer2::set(period, toggleStatus);
    MsTimer2::start();
  }
}

// status codes
const byte FIRE = 0;
const byte FAILURE = 1;
const byte OK = 10;

byte getState(){
  // Get current state and return it.
  byte fire = digitalRead(firePin);
  byte failure = digitalRead(failInPin);
  delay(50); // debounce
  if (fire == 1 && digitalRead(firePin) == 1)
    return FIRE;
  else if (failure == 0 && digitalRead(failInPin) == 0)
    return FAILURE;
  else
    return OK;
}

void smsAlert(string type){
  // send SMS through twilio with sms_alert.py
  // type: "fire", "failure", "ok" or "test"
  Process p;
  p.begin("/root/sms_alert.py");
  p.addParameter(type);
  p.run();
  while (p.available()>0){
    char c = p.read();
    Console.write(c);
  }
  Console.flush();    
}

void loop() {
  // initial states
  static byte currentState = OK;
  static byte previousState = OK;

  // get current state
  currentState = getState();
  if (currentState != previousState) {
    switch (currentState) {
      case FIRE:
        blink(100);
        Console.println("FIRE ALARM!");
        smsAlert("fire");
        break;
  
      case FAILURE:
        blink(1000);
        Console.println("FIRE ALARM FAILURE");
        smsAlert("failure");
        break;
  
      case OK:
        blink(0);
        Console.println("OK");
        smsAlert("ok");
    } // end switch state
  } // end if state change
  previousState = currentState;
}
