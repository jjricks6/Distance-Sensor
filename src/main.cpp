#include <ESP8266WiFi.h>
#include <string>

// Network Details
// Change these to your network settings
const char* wifiSSID = "BYU-WiFi";
const char* wifiPSK  = "";

// Tell the program which pins the ultrasonic distance sensor uses
int echo = D7;
int trig = D8;

long duration;
long distance;

int    HTTP_PORT   = 80;
String HTTP_METHOD = "GET";
char   HOST_NAME[] = "192.168.1.226"; // hostname of web server:

WiFiClient client;

void setup() {
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  Serial.begin(9600);
  delay(5000);
  Serial.println("HC-SR04 Distance Sensor");

  Serial.print("Connecting to ");
  Serial.println(wifiSSID);

  // Connect to the WiFi
  WiFi.begin(wifiSSID, wifiPSK);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Send out pulse
  digitalWrite(trig, LOW);
  delay(20);
  digitalWrite(trig, HIGH);
  delayMicroseconds(15);
  digitalWrite(trig, LOW);
  // Receive pulse
  duration = pulseIn(echo, HIGH);
  // Calculate distance in cm
  distance = (duration*0.0343/2);
  
  Serial.print("Distance: "); // debug out
  Serial.println(distance);

  // Send http request to stoplight based on distance
  if (distance >= 50){
    // send HTTP request header
    client.connect(HOST_NAME, HTTP_PORT);
    client.println(HTTP_METHOD + " " + "/off" + " HTTP/1.1");
  }
  else if (distance >= 20){
    client.connect(HOST_NAME, HTTP_PORT);
    client.println(HTTP_METHOD + " " + "/green" + " HTTP/1.1");
  }
  else if (distance >= 10){
    client.connect(HOST_NAME, HTTP_PORT);
    client.println(HTTP_METHOD + " " + "/yellow" + " HTTP/1.1");
  }
  else if (distance >= 5){
    client.connect(HOST_NAME, HTTP_PORT);
    client.println(HTTP_METHOD + " " + "/red" + " HTTP/1.1");
  }
  else{
    client.connect(HOST_NAME, HTTP_PORT);
    client.println(HTTP_METHOD + " " + "/flash" + " HTTP/1.1");
  }
  
  delay(1000);                // Loop every 1 second
}