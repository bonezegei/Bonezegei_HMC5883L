#include <Wire.h>
#include <Bonezegei_HMC5883L.h>

Bonezegei_HMC5883L compass;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("Initializing HMC5883L...");

  if (!compass.begin()) {
    Serial.println("Failed to find HMC5883L sensor! Check wiring.");
    while (1);
  }

  Serial.println("HMC5883L Connected Successfully!");
}

void loop() {
  if (compass.read()) {
    Serial.print("Raw X: "); Serial.print(compass.getRawX());
    Serial.print(" | Y: "); Serial.print(compass.getRawY());
    Serial.print(" | Z: "); Serial.println(compass.getRawZ());

    Serial.print("Gauss X: "); Serial.print(compass.getX());
    Serial.print(" mG | Y: "); Serial.print(compass.getY());
    Serial.print(" mG | Z: "); Serial.print(compass.getZ());
    Serial.println(" mG");

    // Pass magnetic declination angle in radians (e.g. +0.04 radians for ~2.3° declination)
    float heading = compass.getHeading(0.0);
    Serial.print("Heading: ");
    Serial.print(heading);
    Serial.println("°");
    Serial.println("---------------------------------");
  }

  delay(500);
}