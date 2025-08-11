// Required Libraries
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Ultrasonic Sensor Pins
#define TRIG_PIN D4  // GPIO0
#define ECHO_PIN D3  // GPIO2

// Wi-Fi credentials
char ssid[] = "YOUR_WIFI_SSID";         // Replace with your Wi-Fi SSID
char pass[] = "YOUR_WIFI_PASSWORD";     // Replace with your Wi-Fi Password

// Blynk Auth Token
char auth[] = "YOUR_BLYNK_AUTH_TOKEN";  // Replace with your Blynk Auth Token

// Tank Dimensions (in cm)
const int tankHeight = 30;  // Change according to your tank height (example: 30 cm)

// Variables
long duration;
float distance;
int waterLevelPercent;

BlynkTimer timer;

// Function to calculate distance
float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  duration = pulseIn(ECHO_PIN, HIGH);
  
  // Distance in cm
  distance = (duration * 0.0343) / 2;
  
  return distance;
}

// Function to measure water level
void sendWaterLevel() {
  float distanceCm = getDistance();
  
  // Calculate water level
  float waterLevel = tankHeight - distanceCm;
  
  // Avoid negative values
  if (waterLevel < 0) waterLevel = 0;
  if (waterLevel > tankHeight) waterLevel = tankHeight;
  
  // Convert to percentage
  waterLevelPercent = (waterLevel / tankHeight) * 100;
  
  Serial.print("Distance: ");
  Serial.print(distanceCm);
  Serial.print(" cm  | Water Level: ");
  Serial.print(waterLevelPercent);
  Serial.println(" %");
  
  // Send to Blynk
  Blynk.virtualWrite(V0, waterLevelPercent);
}

void setup() {
  Serial.begin(115200);
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  Blynk.begin(auth, ssid, pass);

  // Run sendWaterLevel() every 2 seconds
  timer.setInterval(2000L, sendWaterLevel);
}

void loop() {
  Blynk.run();
  timer.run();
}
