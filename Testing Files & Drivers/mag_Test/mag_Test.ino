/*
Testing out the magnetometer to accurately know where the robots are on the board.
*/
#include <Wire.h>
#include <LSM303.h>
#include <ZumoMotors.h>

ZumoMotors motors;
LSM303 compass;
float heading;
float prevHeading;
int val;
void setup() {
  Serial.begin(9600);
  Wire.begin();
  compass.init();
  compass.enableDefault();

  compass.m_min = (LSM303::vector<int16_t>) {
    +7995,  +6476, +16090
  };
  compass.m_max = (LSM303::vector<int16_t>) {
    +8015,  +6494, +16109
  };

  compass.read();
  heading = takeReading();
  prevHeading = heading;
}

void loop() {
  compass.read();
  heading = takeReading();
  float newheading = heading;

  while (Serial.available())
  {
    val = Serial.read();
    if (val == 97) {
      if(heading + 90 > 360)
      {
        newheading = heading + 90 -360;
      }
      while (heading >= newheading + 15 || heading <= newheading - 15)
      {
        motors.setLeftSpeed(-200);
        motors.setRightSpeed(200);
        compass.read();
        heading = takeReading();
        Serial.print("heading: ");
        Serial.print(heading);
        Serial.print('\t');
        Serial.print("Target Heading: ");
        Serial.println(newheading);

      }
    }
    if (val == 100) {
      if(heading - 90 < 0)
      {
        newheading = 360 + heading - 90;
      }
      while (heading >= newheading + 15 || heading <= newheading - 15)
      {
        motors.setLeftSpeed(200);
        motors.setRightSpeed(-200);
        compass.read();
        heading = takeReading();
        Serial.print("heading: ");
        Serial.print(heading);
        Serial.print('\t');
        Serial.print("Target Heading: ");
        Serial.println(newheading);

      }
    }
    val = 0;
    motors.setRightSpeed(0);
    motors.setLeftSpeed(0);
  }

  /*if(heading - prevHeading <= 0)
    {
    Serial.print("Heading: ");
    Serial.print(heading);
    Serial.print(" ");
    Serial.print("Previous Heading: ");
    Serial.print(prevHeading);
    Serial.print(" ");
    Serial.println("Turning Counterclockwise.");
    }
    else
    {
    Serial.print("Heading: ");
    Serial.print(heading);
    Serial.print(" ");
    Serial.print("Previous Heading: ");
    Serial.print(prevHeading);
    Serial.print(" ");
    Serial.println("Turning Clockwise.");
    }*/
  prevHeading = heading;
}
float takeReading() {
  float acc = 0;
  for (int i = 0; i < 100; i++)
  {
    acc += compass.heading();
  }
  acc /= 100;
  return acc;
}

