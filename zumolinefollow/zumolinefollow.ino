/*
 * Line-following code for the Pololu Zumo Robot and also stop code for when robt detects something ahead of it
 * The zumo bot used a sharp infrared sensor for proximity sensing 
 */

//incuding all dependences
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <ZumoMotors.h>
#include <ZumoBuzzer.h>
#include <Pushbutton.h>

ZumoBuzzer buzzer;
ZumoReflectanceSensorArray reflectanceSensors;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);
int lastError = 0;
int i = 0;
// This is the maximum speed the motors will be allowed to turn.
// (400 lets the motors go at top speed; decrease to impose a speed limit)
const int MAX_SPEED = 250; //initial 400


void setup()
{
  // Play a little welcome song (this part doesn't work for my zumo bot for some reason)
  buzzer.play(">g32>>c32"); 

  Serial.begin(9600);
  
  // Initialize the reflectance sensors module
  reflectanceSensors.init();

  // Wait for the user button to be pressed and released
  button.waitForButton();

  // Turn on LED to indicate we are in calibration mode
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  // Wait 1 second and then begin automatic sensor calibration
  // by rotating in place to sweep the sensors over the line
  delay(1000);
  
  for(i = 0; i < 80; i++)
  {
    if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
      motors.setSpeeds(-200, 200);
    else
      motors.setSpeeds(200, -200);
    reflectanceSensors.calibrate();
    delay(20);
  }
  motors.setSpeeds(0,0);

  // Turn off LED to indicate we are through with calibration
  digitalWrite(13, LOW);
  //buzzer.play(">g32>>c32");

  // Wait for the user button to be pressed and released
  button.waitForButton();

  delay(500);
}

void loop()
{
  unsigned int sensors[6];
  int obstacle = analogRead(A2);
  
  // Get the position of the line
  int position = reflectanceSensors.readLine(sensors);
  
  // Our "error" is how far we are away from the center of the line, which
  // corresponds to position 2500.
  int error = position - 2500;

  // Get motor speed difference using proportional and derivative PID terms
  // (the integral term is generally not very useful for line following).
  // Here we are using a proportional constant of 1/4 and a derivative
  // constant of 6, which should work decently for many Zumo motor choices.
  // You probably want to use trial and error to tune these constants for
  // your particular Zumo and line course.
  
  //int speedDifference = error / 4 + 6 * (error - lastError); 
  int speedDifference = error / 7 + 6 * (error - lastError); 
  lastError = error;

  // Get individual motor speeds.  The sign of speedDifference
  // determines if the robot turns left or right.
  int m1Speed = MAX_SPEED + speedDifference;
  int m2Speed = MAX_SPEED - speedDifference;

 //restricting speed values so they are neither to big or small
  if (m1Speed < 0)
    m1Speed = 0;
  if (m2Speed < 0)
    m2Speed = 0;
  if (m1Speed > MAX_SPEED)
    m1Speed = MAX_SPEED;
  if (m2Speed > MAX_SPEED)
    m2Speed = MAX_SPEED;

  if(obstacle > 500)  {
    motors.setSpeeds(0,0);
    while(1); //once robot is senses an obstacle trap robot in an infinite loop
  }
  else
    motors.setSpeeds(m1Speed, m2Speed);

  //for debugging purposes you can view the sensor values on the serial monitor
  Serial.print("Line sensor: ");
  Serial.print(position);
  Serial.print(", ");
  Serial.print("Diistance sensor: ");
  Serial.println(obstacle); 
}

