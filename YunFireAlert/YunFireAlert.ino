/* 
yun-fire-alert
Author: Arve Seljebu
Code: https://github.com/arve0/yun-fire-alert

Check inputs and alert upon fire alarm or failure.
*/
#include <Bridge.h>
#include <Console.h>
#include <Process.h>
#include <String.h>
#include <MsTimer2.h>

/*
 * Variables
 */
// pins
const byte firePin = 2;
const byte failInPin = 3;
const byte failOutPin = 4;
const byte statusPin = 13;
// status codes
const byte FIRE = 0;
const byte FAILURE = 1;
const byte FAILURE_YUN = 2;
const byte OK = 10;


/*
 * Functions
 */
void toggleStatus() {
  // toggle statusPin
  static byte out = HIGH;
  digitalWrite(statusPin, out);
  out = !out;
}

void blink(unsigned int period){
  /* blink with MsTimer2
   * 
   * :param period: duty cycle of blinking in milliseconds
   */
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

byte getState(){
  /* Get current state and return it.
   *
   * :globals: firePin, failInPin
   * :return: constant FIRE, FAILURE or OK
   */
  byte fire = digitalRead(firePin);
  byte failure = digitalRead(failInPin);
  delay(50); // debounce
  if (fire == 1 && digitalRead(firePin) == 1)
    return FIRE;
  else if (failure == 0 && digitalRead(failInPin) == 0)
    return FAILURE;
//  else if (Bridge.isOk() != true) {
//    Console.println("bridge not ok");
//    return FAILURE_YUN;
//  }
  else
    return OK;
}

void smsAlert(char type[]){
  /* send SMS through twilio with sms_alert.py
   * 
   * :param type: "fire", "failure", "ok" or "test"
   */
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

byte checkTwilio(){
  /* check connection to twilio with python script
   *
   * :return: exit code python script
   */
//  if (Bridge.isOk() == false)
//    return -1;
  Process p;
  p.begin("/root/check_twilio.py");
  p.run();
  if (p.available()>0) {
    Console.println("No Twilio connection. Error:");
  }
  while (p.available()>0){
    char c = p.read();
    Console.write(c);
  }
  Console.flush();
  return p.exitValue();
}


/*
 * Runtime
 */
void setup() {
  // relay normally open with external pullup
  pinMode(failOutPin, OUTPUT);
  // set failure upon upstart
  digitalWrite(failOutPin, LOW);
  blink(1000);
  // external pullup
  pinMode(firePin, INPUT);
  // internal pullup
  pinMode(failInPin, INPUT_PULLUP);
  // internal LED
  pinMode(statusPin, OUTPUT);

  Bridge.begin();
  Console.begin();
  while (checkTwilio() != 0) {
    Console.println("Waiting for internet connection to Twilio.");
    delay(10000);
  }
}

void loop() {
  // initial state (previous FAILURE -> send OK alert upon start)
  static byte currentState = OK;
  static byte previousState = FAILURE_YUN;

  // get current state
  currentState = getState();
  if (currentState != previousState) {
    switch (currentState) {
      case FIRE:
        blink(100);
        digitalWrite(failOutPin, HIGH);
        Console.println("FIRE ALARM!");
        smsAlert("fire");
        break;
  
      case FAILURE:
        blink(1000);
        digitalWrite(failOutPin, HIGH);
        Console.println("FIRE ALARM FAILURE");
        smsAlert("failure");
        break;
  
      case FAILURE_YUN:
        blink(500);
        digitalWrite(failOutPin, LOW);
        Console.println("YUN FAILURE");
        break;

      case OK:
        blink(0);
        digitalWrite(failOutPin, HIGH);
        Console.println("OK");
        smsAlert("ok");
    } // end switch state
  } // end if state change
  previousState = currentState;
}
