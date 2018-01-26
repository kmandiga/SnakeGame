//This is a test file to see if the arduino can recieve UART messages from the nodeMCU
#include <SoftwareSerial.h>

SoftwareSerial mySerial(4,5);//4 is RX -> TX of other device, 5 is TX -> RX of other device

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  mySerial.begin(9600);  
}

void loop() {
    /*if (mySerial.available()) {
    Serial.write(mySerial.read());
  }*/
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
}
