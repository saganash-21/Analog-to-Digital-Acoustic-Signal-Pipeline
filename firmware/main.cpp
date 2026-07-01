#include <Arduino.h>

// --- Pin Definitions ---
const int CLAP_PIN = 34;   // Input from NPN Collector
const int FAN_PIN = 27;    // Output to MOSFET Gate
const int GREEN_LED = 33;    // Output to Red Gate
const int YELLOW_LED = 25;    // Output to yellow LED



volatile bool fanState = false;
volatile bool clapDetected = false; // Flag for the Serial monitor
volatile unsigned long lastClapTime = 0;
const unsigned long debounceDelay = 500;

void IRAM_ATTR handleClap() {
  unsigned long currentTime = millis();
  if ((currentTime - lastClapTime) > debounceDelay) {
    fanState = !fanState;
    clapDetected = true; // Tell the main loop to print a message
    lastClapTime = currentTime;
  }
}

void setup() {
  Serial.begin(115200); // Start serial communication
  Serial.println("ESP32 Clap Switch Ready. Waiting for sound...");
  
  pinMode(CLAP_PIN, INPUT); 
  pinMode(FAN_PIN, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  digitalWrite(FAN_PIN, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, HIGH);

  attachInterrupt(digitalPinToInterrupt(CLAP_PIN), handleClap, FALLING);
}

void loop() {
  if (clapDetected) {
    Serial.print("Clap heard! Fan State is now: ");
    Serial.println(fanState ? "ON" : "OFF");
    clapDetected = false; // Reset the flag
  }
  
  digitalWrite(FAN_PIN, fanState ? HIGH : LOW);
  digitalWrite(YELLOW_LED, fanState ? HIGH : LOW);
  digitalWrite(GREEN_LED, fanState ? LOW : HIGH);
}