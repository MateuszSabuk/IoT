#include <BLEDevice.h>

// UUIDs of the service and characteristics from the sensor device
static BLEUUID serviceUUID("2346ee0a-5a80-48e1-8fa5-6e8c23ea4d77");
static BLEUUID humidityCharacteristicUUID("2346ee0a-5a80-48e1-8fa5-6e8c23ea4d78");
static BLEUUID temperatureCharacteristicUUID("2346ee0a-5a80-48e1-8fa5-6e8c23ea4d79");
static BLEUUID pressureCharacteristicUUID("2346ee0a-5a80-48e1-8fa5-6e8c23ea4d7A");
static BLEUUID luminosityCharacteristicUUID("2346ee0a-5a80-48e1-8fa5-6e8c23ea4d7B");

// Variables for BLE client and connection status
BLEClient* pClient;
static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic *pRemoteCharacteristic;
static BLEAdvertisedDevice *myDevice;

// Notification callback function
void notifyCallback(
  BLERemoteCharacteristic* pCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify
)
{
  if (pCharacteristic->getUUID().toString() == humidityCharacteristicUUID.toString()) {
    Serial.printf("Humidity: %.2f\n", *((float*) pData));
  } else if (pCharacteristic->getUUID().toString() == temperatureCharacteristicUUID.toString()) {
    Serial.printf("Temperature: %.2f deg C\n", *((float*) pData));
  } else if (pCharacteristic->getUUID().toString() == pressureCharacteristicUUID.toString()) {
    Serial.printf("Pressure: %.2f Pa\n", *((float*) pData));
  } else if (pCharacteristic->getUUID().toString() == luminosityCharacteristicUUID.toString()) {
    Serial.printf("Luminosity: %.2f\n", *((float*) pData));
  }
}

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient *pclient) {}

  void onDisconnect(BLEClient *pclient) {
    connected = false;
    Serial.println("onDisconnect");
  }
};

// Function to connect to a BLE server and subscribe to notifications
bool connectToServer() {
  BLEClient *pClient = BLEDevice::createClient();
  pClient->setClientCallbacks(new MyClientCallback());

  pClient->connect(myDevice);

  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService *pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr) {
    // Failed to find our service UUID
    pClient->disconnect();
    return false;
  }

  // Obtain a reference to the characteristic in the service of the remote BLE server.
  for (const auto& charUUID : {humidityCharacteristicUUID, temperatureCharacteristicUUID, pressureCharacteristicUUID, luminosityCharacteristicUUID}) {
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic == nullptr) {
      pClient->disconnect();
      return false;
    }

    if (pRemoteCharacteristic->canNotify()) {
      pRemoteCharacteristic->registerForNotify(notifyCallback);
    }
  }

  connected = true;
  return true;
}

// Callback for device scanning
class AdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) override {
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {
      Serial.println("Found sensor device");
      advertisedDevice.getScan()->stop();  // Stop scanning
      
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;
    }
  }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE Client");

  // Initialize BLE and start scanning for the sensor device
  BLEDevice::init("");
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new AdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(30);  // Scan for 30 seconds
}

void loop() {
  // If the flag "doConnect" is true then we have scanned for and found the desired
  // BLE Server with which we wish to connect.  Now we connect to it.  Once we are
  // connected we set the connected flag to be true.
  if (doConnect == true) {
    if (connectToServer()) {
      Serial.println("We are now connected to the BLE Server.");
    } else {
      Serial.println("We have failed to connect to the server; there is nothing more we will do.");
    }
    doConnect = false;
  }
  // If we are connected to a peer BLE Server, update the characteristic each time we are reached
  // with the current time since boot.
  if (connected) {
    
  } else if (doScan) {
    BLEDevice::getScan()->start(0);  // this is just example to start scan after disconnect, most likely there is better way to do it in arduino
  }
  delay(1000);
}
