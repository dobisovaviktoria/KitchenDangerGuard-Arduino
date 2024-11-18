#include <WiFiS3.h>
#include <ArduinoHttpClient.h>

// Before using this code follow these steps
// 1.set the ssid to what network you are using
// 2.set password to the password for that network
// 3.connect your laptop/computer to the same network
// 3.change server adress to ur local ip adress aka ipconfig
//Trouble? check if you have the right library which is the arduinohttpclient library and WifiS3 for the arduino R4 wifi

#define SENSOR_PIN  2 
int motion_state = LOW; 
int prev_motion_state = LOW; 

const char* ssid = "KdG-iDev";                     //network here
const char* password = "rpiAzeG4JNHEPnvN";          //password here

const char* serverAddress = "10.134.217.206";  //change this to the local ip adress aka check ipconfig
int port = 8080;  
const char* endpoint = "/motion";

WiFiClient wifiClient;
HttpClient client = HttpClient(wifiClient, serverAddress, port);

void setup() {
  Serial.begin(9600); 
  pinMode(SENSOR_PIN, INPUT); 
  
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to the WiFi network");
}

void loop() {
  prev_motion_state = motion_state; 
  motion_state = digitalRead(SENSOR_PIN); 

  if (prev_motion_state == LOW && motion_state == HIGH) {
    Serial.println("Motion detected!");
    sendMotionData(true);
  } else {
    Serial.println("No Motion detected!");
    sendMotionData(false);
  }

  delay(1000); 
}

void sendMotionData(bool motionStatus) {
  if (WiFi.status() == WL_CONNECTED) {
    String postData = "{\"motionStatus\": " + String(motionStatus ? "true" : "false") + "}"; 
    

    client.beginRequest();
    client.post(endpoint);
    client.sendHeader("Content-Type", "application/json");
    client.sendHeader("Content-Length", postData.length());
    client.beginBody();
    client.print(postData);
    client.endRequest();
    
    int statusCode = client.responseStatusCode();
    String response = client.responseBody();
    
    Serial.println("Response code: " + String(statusCode));
    Serial.println("Response: " + response);
  } else {
    Serial.println("WiFi Disconnected");
  }
}
