#pragma once
#include "sensor.h"

// Sensors
Adafruit_BMP280 bmp;
ArtronShop_BH1750 lightMeter(0x5C, &Wire);

void initSensors(bool useSerial) {
    Wire.begin(WIRE_PINS);

    // Init sensors
    if (!bmp.begin()) {
        if (useSerial) Serial.println("Error initialising BMP280!");
        while (1);
    }
    if (!lightMeter.begin()) {
        if (useSerial) Serial.println("Error initialising BH1750");
        while(1);
    }
    bmp.setSampling(Adafruit_BMP280::MODE_FORCED,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void getReadings(float &humidity, float &temperature, float &pressure, float &luminosity) {
    humidity = analogRead(HUM_PIN);
    if (bmp.takeForcedMeasurement()) {
        temperature = bmp.readTemperature();
        pressure = bmp.readPressure();
    }
    luminosity = lightMeter.light();
}
