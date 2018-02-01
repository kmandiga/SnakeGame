/*
  Rough outline of the brain functionality. Right now if the arduino recieves a number between 0 and 4, it will print to the terminal a function.
  Eventually, this will change to control the motors based on the input.
*/
#include <ZumoMotors.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(4, 5); //Remember to cross RX and TX on other device, in this case, the ESP8266
byte serialInput = 0;
ZumoMotors motors;
int ledPins[2] = {10, 11};

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(ledPins[0], OUTPUT);
  pinMode(ledPins[1], OUTPUT);

  digitalWrite(ledPins[0], LOW);
  digitalWrite(ledPins[1], LOW);
}

void loop() {

  if (mySerial.available() > 0) {
    serialInput = mySerial.read();
    Serial.println(serialInput);
    switch (serialInput) {
      case 119:
        //Move up if not moving up or down
        Serial.println("UP");
        digitalWrite(ledPins[0], LOW);
        digitalWrite(ledPins[1], LOW);
        break;
      case 97:
        //Move left if not moving left or right
        Serial.println("LEFT");
        digitalWrite(ledPins[0], LOW);
        digitalWrite(ledPins[1], HIGH);
        break;
      case 115:
        //Move down if not moving up or down
        Serial.println("DOWN");
        digitalWrite(ledPins[0], HIGH);
        digitalWrite(ledPins[1], LOW);
        break;
      case 100:
        //Move right if not moving left or right
        Serial.println("RIGHT");
        digitalWrite(ledPins[0], HIGH);
        digitalWrite(ledPins[1], HIGH);
        break;
    }
    delay(1000);
    serialInput = 0;
    digitalWrite(ledPins[0], LOW);
    digitalWrite(ledPins[1], LOW);
  }
}
