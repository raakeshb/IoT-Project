//***** Raakesh*****======> LDR and  ULTRASONIC
//https://www.arduino.cc/reference/en/#functions ====== for all fn

//https://www.arduino.cc/reference/en/language/functions/advanced-io/pulsein -FOR PULSE IN FUNCTION USED BELOW
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClient.h>

const char* ssid = "SSID";
const char* password =  "PASSWORD";
const char* mqttServer = "IPADDRESS OR LOCALHOST" ;
const int mqttPort = 1883;
const char* mqttUser = "MQTTUSER";
const char* mqttPassword = "MQTTPASS";
WiFiClient espClient;
PubSubClient client(espClient);
int outputpin= A0;


void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {

      Serial.println("connected");

    } else {

      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);

    }
  }

}


void callback(char* topic, byte* payload, unsigned int length) {
 

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  Serial.println();
  Serial.println("-----------------------");

}


void loop() {
  int analogValue = analogRead(outputpin);
float millivolts = (analogValue/1024.0) * 3300; //3300 is the voltage provided by NodeMCU
float celsius = millivolts/10;
Serial.print("in DegreeC=   ");
Serial.println(celsius);
float fahrenheit = ((celsius * 9)/5 + 32);
Serial.print(" in Farenheit=   ");
Serial.println(fahrenheit);
delay(1000);
  client.publish("esp/temp", String(celsius).c_str());
  client.publish("esp/temp", String(fahrenheit).c_str());
  client.loop();
}
