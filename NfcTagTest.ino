#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClient.h>

const char* ssid = "ReaperX1";
const char* password =  "samsung12";
const char* mqttServer = "192.168.43.232" ;
const int mqttPort = 1883;
const char* mqttUser = "raakesh";
const char* mqttPassword = "raakesh30";
WiFiClient espClient;
PubSubClient client(espClient);
const int T = 2;//(d4)
const int E = 4;//(d2)
int hi = 0; //(d3)
// defines variables
long duration;
int distance;
// 5v FOR ULTRA SONIC SENSOR


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

    if (client.connect("ESP8266Client2", mqttUser, mqttPassword )) {

      Serial.println("connected");

    } else {

      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);

    }
  }



  pinMode(hi, OUTPUT);
  pinMode(T, OUTPUT); // Sets the T as an Output
  pinMode(E, INPUT); // Sets the E as an Input
  //pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(115200); // IMPORTANT******    <=== TO CHECK
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
  // Clears the T
  int sensorValue = analogRead(A0);
  digitalWrite(T, LOW);
  delayMicroseconds(2);

  // Sets the T on HIGH state for 10 micro seconds
  digitalWrite(T, HIGH);
  delayMicroseconds(10);
  digitalWrite(T, LOW);
  //pulseIn return time taken in microsec
  duration = pulseIn(E, HIGH);

  // Calculating the distance
  distance = duration * 0.034 / 2;


  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  delay(500);
  float voltage = sensorValue * (5.0 / 1023.0);
  if (int(voltage) < 2)
  {
    digitalWrite(hi, HIGH);
  }
  else
  {
    digitalWrite(hi, LOW);
  }
  delay(1000);
  Serial.println(voltage);
  if (int(voltage) < 1)
  {
    client.publish("LDR","ligth is OFF");
    client.publish("voltage",String(voltage).c_str());
    delay(5000);
  }
  else
  {
     client.publish("voltage", String(voltage).c_str());
     client.publish("LDR","Light is ON");
     delay(5000);
  }
  if (distance >22)
  {
  client.publish("distance", String(distance).c_str());
  client.publish("dismail","Empty");
  }
  else if (distance >14 && distance <17)
  {
      client.publish("distance", String(distance).c_str());
      client.publish("dismail","Half");
  }
  else if (distance < 14)
  {
      client.publish("distance", String(distance).c_str());
       client.publish("dismail","Almost Full") ; 
  }
  client.loop();
}
