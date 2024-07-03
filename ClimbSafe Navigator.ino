#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <DHT.h>

// WiFi credentials
const char* ssid = "imein1212";
const char* password = "123456789";

// ThingSpeak credentials
const char* server = "api.thingspeak.com";
const long channelID = 123456; // replace with your channel ID
const char* apiKey = "API KEY"; // replace with your API key
WiFiClient client;

// Initialize the DHT sensor
#define DHTPIN D1
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Initialize the analog input pin for the heartbeat sensor
const int analogInPin = A0;

void setup() {
  // Connect to Wi-Fi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  // Initialize the DHT sensor
  dht.begin();

  // Initialize ThingSpeak
  ThingSpeak.begin(client);

  // Initialize serial communication
  Serial.begin(9600);
}

void loop() {
  // Read the data from the sensors
  int heartRate = analogRead(analogInPin);
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Print the data on the serial monitor
  Serial.print("Heart rate: ");
  Serial.print(heartRate/11.5);
  Serial.print(" | Temperature: ");
  Serial.print(temperature);
  Serial.print(" | Humidity: ");
  Serial.println(humidity);

  // Check if the data is valid
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Error: Invalid data");
    return;
  }
   

  // Update ThingSpeak
  ThingSpeak.setField(1, heartRate);
  ThingSpeak.setField(2, temperature);
  ThingSpeak.setField(3, humidity);
  int status = ThingSpeak.writeFields(channelID, apiKey);

  // Print the status on the serial monitor
  Serial.print("ThingSpeak Status: ");
  Serial.println(status);

  // Wait for a few seconds before sending the next data
  delay(10000);
}
