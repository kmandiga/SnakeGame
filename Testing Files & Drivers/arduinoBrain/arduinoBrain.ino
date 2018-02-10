#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <ZumoMotors.h>
#include <SoftwareSerial.h>
;
SoftwareSerial mySerial(A1, A4); //2 is RX, A4 is TX, cross w/ RX and TX of other device
byte serialInput = 0;
ZumoMotors motors;
ZumoReflectanceSensorArray reflectanceSensors;

// Define an array for holding sensor values.
#define NUM_SENSORS 6
#define SPEED 100
unsigned int sensorValues[NUM_SENSORS];
int lastError = 0;
void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  reflectanceSensors.init();
  delay(500);
  //Begin Calibration of line sensors
  //All the sensors need to "see" the black and white
  //have the robot placed in the middle of a line
  //have the robot spin in place for 5 seconds slowly to auto calibrate

  unsigned long startTime = millis();
  motors.setLeftSpeed(100);
  motors.setRightSpeed(-100);
  while (millis() - startTime < 5000)  // make the calibration take 5 seconds
  {
    reflectanceSensors.calibrate();
  }
  motors.setLeftSpeed(0);
  motors.setRightSpeed(0);

  // print the calibration minimum values measured when emitters were on

  for (byte i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(reflectanceSensors.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();

  // print the calibration maximum values measured when emitters were on
  for (byte i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(reflectanceSensors.calibratedMaximumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(100);
  //At this point calibration is done, so robot is ready to recieve instructions from ESP
}
/*
   The robot will always continue moving forward (using a PID)
   If the user enters a or d, the robot needs to rotate left or right
   The robot has to follow a line until it hits an intersection on the board
   At the intersection, it will execute the rotation
   once the rotation is properly executed, the robot needs to send a message to ESP (to be published to status/robotNo)
   the message will say the turn was completed successfully
   It will go straight (PID) after finishing the turn
*/
void loop()
{
  if (mySerial.available() > 0) {
    serialInput = mySerial.read();
    Serial.println(serialInput);
    switch (serialInput) {
      case 119:
        //Move up
        
        motors.setLeftSpeed(100);
        motors.setRightSpeed(100);
        break;
      case 97:
        //Move left
        motors.setLeftSpeed(-100);
        motors.setRightSpeed(100);
        delay(750);
        motors.setSpeeds(0,0);
        
        break;
      case 115:
        //stop
        stopRobot();
        break;
      case 100:
        //Move right
        motors.setLeftSpeed(100);
        motors.setRightSpeed(-100);
        delay(750);
        motors.setSpeeds(0,0);
        
        break;
    }
    serialInput = 0;
  }
  pid();
  
}
void stopRobot()
{
  //motors.setLeftSpeed(0);
  //motors.setRightSpeed(0);
  motors.setSpeeds(0,0);
  while (1) {};
}
void pid()
{
  int position = reflectanceSensors.readLine(sensorValues);
  int error = position - 2500;
  int speedDifference = error / 4 + 6 * (error - lastError);

  lastError = error;
  int m1Speed = SPEED + speedDifference;
  int m2Speed = SPEED - speedDifference;

  if (m1Speed < 0)
    m1Speed = 0;
  if (m2Speed < 0)
    m2Speed = 0;
  if (m1Speed > SPEED)
    m1Speed = SPEED;
  if (m2Speed > SPEED)
    m2Speed = SPEED;
  
  motors.setSpeeds(m1Speed, m2Speed);



}
