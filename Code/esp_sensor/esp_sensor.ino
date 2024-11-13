#include "sensor.h"

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// BLE Service and Characteristics UUIDs
#define SERVICE_UUID "2346ee0a-5a80-48e1-8fa5-6e8c23ea4d77"
#define HUMIDITY_CHARACTERISTIC_UUID "2346ee0a-5a80-48e1-8fa5-6e8c23ea4d78"
#define TEMPERATURE_CHARACTERISTIC_UUID "2346ee0a-5a80-48e1-8fa5-6e8c23ea4d79"
#define PRESSURE_CHARACTERISTIC_UUID "2346ee0a-5a80-48e1-8fa5-6e8c23ea4d7A"
#define LUMINOSITY_CHARACTERISTIC_UUID "2346ee0a-5a80-48e1-8fa5-6e8c23ea4d7B"

BLECharacteristic *humidityCharacteristic;
BLECharacteristic *temperatureCharacteristic;
BLECharacteristic *pressureCharacteristic;
BLECharacteristic *luminosityCharacteristic;

bool deviceConnected = false;

class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      Serial.println("Client connected");
    }

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      Serial.println("Client disconnected, restarting advertising");
      BLEDevice::startAdvertising();  // Restart advertising when the client disconnects
    }
};

void setup() {
  Serial.begin(19200);
  Serial.println("Starting BLE setup...");

  initSensors(true);

  // Initialize BLE configurations
  BLEDevice::init("Sensor");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());  // Set the callback for connection events

  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create read-only characteristics with notification enabled
  humidityCharacteristic = pService->createCharacteristic(
      HUMIDITY_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
  temperatureCharacteristic = pService->createCharacteristic(
      TEMPERATURE_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
  pressureCharacteristic = pService->createCharacteristic(
      PRESSURE_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
  luminosityCharacteristic = pService->createCharacteristic(
      LUMINOSITY_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);

  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // Improve compatibility with iOS
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  Serial.println("Waiting for a client to connect...");
}

float hum = 5, temp = 0, press = 0, lum = 0;

void loop() {
  if (deviceConnected) {
    getReadings(hum, temp, press, lum);

    // Update characteristics with new sensor values
    humidityCharacteristic->setValue(hum);
    temperatureCharacteristic->setValue(temp);
    pressureCharacteristic->setValue(press);
    luminosityCharacteristic->setValue(lum);

    // Notify clients about updated values
    humidityCharacteristic->notify();
    temperatureCharacteristic->notify();
    pressureCharacteristic->notify();
    luminosityCharacteristic->notify();

    Serial.println("Updated characteristics and sent notifications:");
    Serial.print("Humidity: "); Serial.println(hum);
    Serial.print("Temperature: "); Serial.println(temp);
    Serial.print("Pressure: "); Serial.println(press);
    Serial.print("Luminosity: "); Serial.println(lum);

    sleep(10);  // Delay between updates
  } else {
    // Device is not connected;
    sleep(30);
  }
}
