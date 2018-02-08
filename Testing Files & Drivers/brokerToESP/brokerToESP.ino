/*
 * This is able to recieve a message from the MQTT broker. It takes the message and passes it on to the Arduino microcontroller.
 * Still needs to: Be able to recieve messages from the Arduino, and push those messages to the MQTT broker.
 */

#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <PubSubClient.h>

#define MQTT_SERVER "172.25.107.154"
const char* ssid = "UHWireless";
const char* password = "";
char* topic = "controls";

void callback(char* topic, byte* payload, unsigned int length);

byte serialInput = 0;

WiFiClient wifiClient;

PubSubClient client(MQTT_SERVER, 1883, callback, wifiClient);

SoftwareSerial mySerial(12, 13); //corresponds to D6 and D7 on board
int wifiConnectedPin = 14;
int mqttConnectedPin = 5;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(9600);

  /*pinMode(wifiConnectedPin, OUTPUT);
  pinMode(mqttConnectedPin, OUTPUT);
  digitalWrite(wifiConnectedPin, LOW);
  digitalWrite(mqttConnectedPin, LOW);*/

  WiFi.begin(ssid, password);
  reconnect();

  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  if ( (WiFi.status() == WL_CONNECT_FAILED) || (WiFi.status() == WL_CONNECTION_LOST) || (WiFi.status() == WL_DISCONNECTED) || !client.connected())
  {
    /*digitalWrite(wifiConnectedPin, LOW);
    digitalWrite(mqttConnectedPin, LOW);*/
    reconnect();
  }
  client.loop();
  delay(10);
}
void callback(char* topic, byte* payload, unsigned int length) {

  //convert topic to string to make it easier to work with
  String topicStr = topic;

  serialInput = payload[0];
  mySerial.write(serialInput);
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
  //make sure we are connected to wifi before connecting to mqtt
  if (WiFi.status() == WL_CONNECTED)
  {
    while (!client.connected())
    {
      Serial.print("Attempting MQTT connection...");

      String clientName;
      clientName += "esp8266-";
      uint8_t mac[6];
      WiFi.macAddress(mac);
      clientName += macToStr(mac);

      if (client.connect((char*) clientName.c_str()))
      {
        Serial.print("\tMQTT connected");
        //digitalWrite(mqttConnectedPin, HIGH);
        client.subscribe(topic);
      }
      else
      {
        Serial.println("\tFailed");
        abort();
      }
    }
  }
}
String macToStr(const uint8_t* mac){

  String result;

  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);

    if (i < 5){
      result += ':';
    }
  }

  return result;
}
