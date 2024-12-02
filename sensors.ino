#include <WiFiS3.h>
#include <ArduinoHttpClient.h>
#include <Adafruit_MLX90614.h>

// WiFi network information
const char* ssid = "KdG-iDev";           // Replace with your WiFi SSID
const char* password = "rpiAzeG4JNHEPnvN";  // Replace with your WiFi password

// Server information
const char* serverAddress = "10.134.217.206";  // Replace with your server address or IP
int port = 8080;                              // Replace with your server's port (e.g., 80 for HTTP)

// Sensor and endpoint configuration
#define SENSOR_PIN 2
const char* motionEndpoint = "/motion";
const char* tempEndpoint = "/temperature";

// State tracking for sensors
int motion_state = LOW;
int prev_motion_state = LOW;
float old_temp = 0.0;

// WiFi and HTTP clients
WiFiClient wifiClient;
HttpClient client = HttpClient(wifiClient, serverAddress, port);

// MLX90614 object for temperature reading
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

// Function to connect to WiFi
void connectToWiFi() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
}

// Function to send data to the server
void sendDataToServer(const char* endpoint, String jsonData) {
  if (WiFi.status() == WL_CONNECTED) {
    client.beginRequest();
    client.post(endpoint);
    client.sendHeader("Content-Type", "application/json");
    client.sendHeader("Content-Length", jsonData.length());
    client.beginBody();
    client.print(jsonData);
    client.endRequest();

    int statusCode = client.responseStatusCode();
    String response = client.responseBody();

    Serial.print("Response code: ");
    Serial.println(statusCode);
    Serial.print("Response: ");
    Serial.println(response);
  } else {
    Serial.println("WiFi Disconnected");
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Starting setup...");

  pinMode(SENSOR_PIN, INPUT);

  Serial.println("Initializing MLX90614...");

  if (!mlx.begin()) {
    Serial.println("Failed to initialize MLX90614 sensor!");
  }

  Serial.println("MLX90614 initialized!");

  connectToWiFi();
}

void loop() {

  // Handle motion detection
  prev_motion_state = motion_state;
  motion_state = digitalRead(SENSOR_PIN);

  if (prev_motion_state == LOW && motion_state == HIGH) {
    Serial.println("Motion detected!");
    String motionData = "{\"motionStatus\": true}";
    sendDataToServer(motionEndpoint, motionData);
    Serial.println("Motion data sent to server.");
  } else if (prev_motion_state == HIGH && motion_state == LOW) {
    Serial.println("No Motion detected!");
    String motionData = "{\"motionStatus\": false}";
    sendDataToServer(motionEndpoint, motionData);
  }


    // Handle temperature reading
    float temperature = mlx.readObjectTempC();  // Read object temperature in Celsius
    if (abs(old_temp - temperature) >= 0.5) {
      old_temp = temperature;
      Serial.print("Temperature = ");
      Serial.print(temperature);
      Serial.println(" °C");

      String tempData = "{\"temperatureValue\": " + String(temperature) + "}";
      sendDataToServer(tempEndpoint, tempData);
      Serial.println("Temperature data sent to server.");
    }


  // Delay to avoid spamming the server
  delay(1000);
}