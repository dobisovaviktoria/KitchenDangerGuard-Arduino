# Kitchen Danger Guard Project

## Team 7

## Team Members
- **Khaled Asfour**
- **Viktória Dobišová**
- **Nang Cherry Naw**
- **Deren Ozen**
- **Alec Tuffin**

## Responsibilities
- **Alec Tuffin**: Responsible for motion sensor integration and Wifi connection.
- **Deren Ozen**: Responsible for temperature sensor integration and server communication, and data transmission.

## Reviewer
- **Viktória Dobišová**: Responsible for testing and reviewing the functionality of project.

## Project Overview

- The Kitchen Danger Guard project combines a motion sensor and an infrared temperature sensor to monitor kitchen conditions. Data from these sensors is sent to a remote server via WiFi for analysis and potential alerts.

## Installation and Configuration Instructions

1. **Install the Arduino IDE**: 
   - Download and install the Arduino IDE from [Arduino Official Website](https://www.arduino.cc/en/software).

2. **Set up the Arduino Board**:
   - Open the Arduino IDE and select your Arduino board model under **Tools > Board**.

3. **Install Required Libraries**:
   The following libraries must be installed via the Arduino Library Manager:
   
   - **WiFiS3** 

   - **ArduinoHttpClient** 

   - **Adafruit_MLX90614** 

4. **Configure WiFi Credentials**:
   - Open the `.ino` file in the Arduino IDE.
   - Modify the following lines with your own WiFi credentials:
     ```cpp
     const char* ssid = "Your_WiFi_SSID";
     const char* password = "Your_WiFi_Password";
     ```

5. **Upload the Code**:
   - Once the libraries are installed and WiFi credentials are set, connect your Arduino board to your computer and upload the code via **Sketch > Upload**.

6. **Monitor the Output**:
   - Open the **Serial Monitor** (under **Tools > Serial Monitor**) to view the output and see if the system connects to WiFi and sends data to the server correctly.

## Hardware(Sensor) Specifications

- **Adafruit MLX90614** (Infrared Temperature Sensor):
  - Measures ambient and object temperature.
  - Interface: I2C
  - Operating Voltage: 3.3V to 5V
  - Measurement Range: -70°C to +380°C
  - Accuracy: ±0.5°C (for object temperatures between -20°C to 85°C)

- **Motion Sensor**:
  - Type: Passive Infrared (PIR) sensor
  - Interface: Digital Output
  - Operating Voltage: 5V
  - Detects motion in the surrounding environment and outputs a HIGH signal when motion is detected.

## Software Functionality

- Motion Detection: Monitors for movement using a PIR motion sensor.

- Temperature Monitoring: Reads object temperature using the MLX90614 sensor.

- WiFi Communication: Sends JSON-formatted sensor data to a server over HTTP.

- **Data Format**
   - The data sent to the server is formatted as a JSON object:
   {
  "motionStatus": true,
  "temperatureValue": 25.6,
  "deviceId": 1
   }

## Libraries Used

1. **WiFiS3 Library**:
   - Used to connect the Arduino board to a WiFi network.

2. **ArduinoHttpClient Library**:
   - Provides HTTP request handling functionality to send sensor data to the server.

3. **Adafruit MLX90614 Library**:
   - Provides an easy interface to communicate with the MLX90614 temperature sensor.

## Troubleshooting

1. **WiFi Connection Issues**:
   - Make sure your WiFi credentials are entered correctly in the code.
   - Check that your Arduino board is within range of the WiFi network.

2. **Sensor Initialization**:
   - If the MLX90614 sensor is not initialized correctly, check the wiring and ensure that the sensor is properly connected to the I2C pins of your Arduino.

3. **Server Communication**:
   - Verify the server's IP address and port in the code. Ensure that the server is accessible from the network your Arduino is connected to.

## Notes
- This project is designed to send sensor data (motion and temperature) over HTTP to a server. Ensure the server is set up to handle the incoming requests at the specified endpoint (`/sensor-data`).
- The system includes visual feedback with an LED to indicate motion detection.