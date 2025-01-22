#include <ArtronShop_BH1750.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>

// DEFINE PINS
#define HUM_PIN 1
#define WIRE_PINS 8,9

void initSensors(bool useSerial = false);
void getReadings(float &humidity, float &temperature, float &pressure, float &luminosity);