#include "DHT.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <TimeLib.h>

#define DHTPIN D4         // Define the pin where the DHT sensor is connected
#define DHTTYPE DHT11     // Define the type of DHT sensor (DHT11)

// Ultrasonic Sensor pins
#define TRIG_PIN D2       // Define the pin for the ultrasonic sensor's trigger
#define ECHO_PIN D3       // Define the pin for the ultrasonic sensor's echo

DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor

float humidity;    // Variable to store humidity value
float temperature; // Variable to store temperature value
float distance;    // Variable to store distance value

const char* ssid = "AbdulMuiz-2.4G";          // WiFi SSID
const char* password = "MuizZailah9116";      // WiFi password
char server[] = "192.168.100.106";            // Server IP address

WiFiClient client; // Initialize WiFi client

// NTP configuration
const char* ntpServer = "pool.ntp.org";       // NTP server address
const long gmtOffset_sec = 0;                 // GMT offset in seconds
const int daylightOffset_sec = 0;             // Daylight offset in seconds

void setup() {
  Serial.begin(115200); // Initialize serial communication at 115200 baud rate
  delay(10);
  dht.begin();          // Initialize DHT sensor

  // Setup Ultrasonic Sensor pins
  pinMode(TRIG_PIN, OUTPUT); // Set trigger pin as output
  pinMode(ECHO_PIN, INPUT);  // Set echo pin as input

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password); // Start WiFi connection

  // Wait until connected to WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Initialize time library and connect to NTP server
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("Waiting for time");
  while (!time(nullptr)) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Time synchronized");

  Serial.println("Server started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); // Print local IP address
  delay(1000);
}

void loop() {
  humidity = dht.readHumidity();    // Read humidity from DHT sensor
  temperature = dht.readTemperature(); // Read temperature from DHT sensor
  distance = readUltrasonicDistance(); // Read distance from ultrasonic sensor

  Sending_To_phpmyadmindatabase(); // Send data to the server

  delay(30000); // Wait for 30 seconds before sending the next data
}

float readUltrasonicDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH); // Measure the duration of the echo pulse
  float distance = duration * 0.034 / 2;   // Calculate the distance in cm
  return distance;
}

void Sending_To_phpmyadmindatabase() {
  time_t now = time(nullptr); // Get current time in seconds since 1970-01-01 00:00:00 UTC

  if (client.connect(server, 80)) {
    Serial.println("Connected to server");

    // Formulate the HTTP GET request:
    String url = "/env/data.php";
    url += "?humidity=";
    url += humidity;
    url += "&temperature=";
    url += temperature;
    url += "&distance=";
    url += distance;
    url += "&timestamp=";
    url += now; // Append current Unix timestamp

    Serial.print("Requesting URL: ");
    Serial.println(url);

    // Send HTTP GET request to the server:
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + server + "\r\n" +
                 "Connection: close\r\n\r\n");

  } else {
    Serial.println("Connection failed");
  }
}
