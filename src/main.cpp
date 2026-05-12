#include <Arduino.h>
#include "HX711.h"

// Pin configuration
#define HX711_DOUT 34
#define HX711_SCK  2

HX711 scale;

// Calibration factor (must be adjusted with a known weight)
float calibration_factor = -7050.0;

// Number of samples for averaging
const int sample_count = 10;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("HX711 Load Cell Test with ESP32");

  // Initialize HX711
  scale.begin(HX711_DOUT, HX711_SCK);

  if (!scale.is_ready()) {
    Serial.println("HX711 not found. Check wiring.");
    while (1);
  }

  Serial.println("HX711 initialized.");

  // Tare (set zero weight)
  Serial.println("Taring... Remove all weight.");
  delay(2000);
  scale.tare();
  Serial.println("Tare complete.");

  scale.set_scale(calibration_factor);
}

void loop() {

  if (scale.is_ready()) {

    // Raw ADC value
    long raw = scale.read_average(sample_count);

    // Converted weight
    float weight = scale.get_units(sample_count);

    Serial.print("Raw: ");
    Serial.print(raw);

    Serial.print(" | Weight: ");
    Serial.print(weight, 2);
    Serial.println(" g");

  } 
  else {
    Serial.println("HX711 not ready.");
  }

  // Serial commands
  if (Serial.available()) {
    char cmd = Serial.read();

    if (cmd == 't') {
      Serial.println("Taring...");
      scale.tare();
      Serial.println("Tare done.");
    }

    if (cmd == '+') {
      calibration_factor += 10;
      scale.set_scale(calibration_factor);
      Serial.print("Calibration factor: ");
      Serial.println(calibration_factor);
    }

    if (cmd == '-') {
      calibration_factor -= 10;
      scale.set_scale(calibration_factor);
      Serial.print("Calibration factor: ");
      Serial.println(calibration_factor);
    }
  }

  delay(500);
}
