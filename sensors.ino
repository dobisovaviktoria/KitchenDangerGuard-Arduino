#include <WiFiS3.h>
#include <ArduinoHttpClient.h>
#include <Adafruit_MLX90614.h>

// WiFi network information
const char* ssid = "KdG-iDev";              // Replace with your WiFi SSID
const char* password = "rpiAzeG4JNHEPnvN";  // Replace with your WiFi password

// Server information
const char* serverAddress = "10.134.217.205";  // Replace with your server address or IP
int port = 8080;                               // Replace with your server's port (e.g., 80 for HTTP)

// Sensor and endpoint configuration
#define SENSOR_PIN 2
#define LED_PIN 13

const char* endpoint = "/sensor-data";
int deviceId = 1;

// State tracking for sensors
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
    Serial.println("WiFi Disconnected!");
  }
}

// Function to handle motion detection
int checkMotion() {
  return digitalRead(SENSOR_PIN);
}

// Function to read temperature from the MLX90614 sensor
float readTemperature() {
  return mlx.readObjectTempC();
}

// Function to create the JSON data
String createJsonData(int motionDetected, float temperature) {
  String jsonData = "{";
  jsonData += "\"motionStatus\": " + String(motionDetected == HIGH ? "true" : "false") + ",";
  jsonData += "\"temperatureValue\": " + String(temperature) + ",";
  jsonData += "\"deviceId\": " + String(deviceId);
  jsonData += "}";
  return jsonData;
}

void setup() {
  Serial.begin(9600);
  Serial.println("Starting setup...");
  while (!Serial);

  pinMode(SENSOR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  Serial.println("Initializing MLX90614...");

  if (!mlx.begin()) {
    Serial.println("Error connecting to MLX sensor. Check wiring.");
    while (1);
  };

  Serial.println("MLX90614 initialized!");

  connectToWiFi();
}

void loop() {

  int motionDetected = checkMotion();
  float temperature = readTemperature();

  if (motionDetected == HIGH) {
    digitalWrite(LED_PIN, HIGH);  // Turn LED ON
    Serial.println("Motion Detected: True - LED ON");
  } else {
    digitalWrite(LED_PIN, LOW);  // Turn LED OFF
    Serial.println("Motion Detected: False - LED OFF");
  }

  Serial.println("Temperature is: " + String(temperature));
  old_temp = temperature;

  if (isnan(temperature)) {
    Serial.println("Connot read the temperature correctly");
  } else {
    String jsonData = createJsonData(motionDetected, temperature);
    Serial.println("Sending data to server...");
    sendDataToServer(endpoint, jsonData);
    Serial.println("Combined data sent to server: " + jsonData);
  }

  delay(1000);
}