#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

const char* ssid = "UHWireless";
const char* password = "";

byte serialInput = 0;

WiFiClient wifiClient;

SoftwareSerial mySerial(12, 13); //corresponds to D6 and D7 on board
int wifiConnectedPin = 14;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(9600);

  pinMode(wifiConnectedPin, OUTPUT);
  digitalWrite(wifiConnectedPin, LOW);

  WiFi.begin(ssid, password);
  reconnect();

  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  if ( (WiFi.status() == WL_CONNECT_FAILED) || (WiFi.status() == WL_CONNECTION_LOST) || (WiFi.status() == WL_DISCONNECTED))
  {
    digitalWrite(wifiConnectedPin, LOW);
    reconnect();
  }
  if(Serial.available()>0)
  {
    serialInput = Serial.read();
    //Serial.println(serialInput);
    mySerial.write(serialInput);
  }
  serialInput = 0;
}
void reconnect() {

  //attempt to connect to the wifi if connection is lost
  if (WiFi.status() != WL_CONNECTED) {
    //debug printing
    //Serial.print("Connecting to ");
    Serial.println(ssid);

    //loop while we wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      //Serial.print(".");
    }
    digitalWrite(wifiConnectedPin, HIGH);
    //print out some more debug once connected
    /*Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());*/
  }
}
