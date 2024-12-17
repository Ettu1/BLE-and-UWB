#include "BLEDevice.h"

// UUIDs for the BLE service and characteristic
static BLEUUID serviceUUID("12345678-1234-1234-1234-123456789abc");
static BLEUUID charUUID("12345678-1234-1234-1234-123456789abd");

static boolean doConnect = false;
static boolean connected = false;
static BLERemoteCharacteristic *pRemoteCharacteristic;
static BLEAdvertisedDevice *myDevice;

// Callback for notifications
static void notifyCallback(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify) {
  Serial.print("Notification from characteristic ");
  Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
  Serial.print(": ");
  Serial.write(pData, length);
  Serial.println();
}

// Client callback class to handle events
class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient *pclient) {
    Serial.println("Connected to server!");
  }

  void onDisconnect(BLEClient *pclient) {
    connected = false;
    Serial.println("Disconnected from server.");
  }
};

// Connect to the server and retrieve the desired service and characteristic
bool connectToServer() {
  Serial.print("Connecting to device at ");
  Serial.println(myDevice->getAddress().toString().c_str());

  BLEClient *pClient = BLEDevice::createClient();
  Serial.println(" - Created client");

  pClient->setClientCallbacks(new MyClientCallback());

  if (!pClient->connect(myDevice)) {
    Serial.println("Failed to connect to the server.");
    return false;
  }
  Serial.println(" - Connected to server");

  // Get the service
  BLERemoteService *pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr) {
    Serial.println("Failed to find service UUID.");
    pClient->disconnect();
    return false;
  }
  Serial.println(" - Found service");

  // Get the characteristic
  pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
  if (pRemoteCharacteristic == nullptr) {
    Serial.println("Failed to find characteristic UUID.");
    pClient->disconnect();
    return false;
  }
  Serial.println(" - Found characteristic");

  // Read the value of the characteristic
  if (pRemoteCharacteristic->canRead()) {
    String value = pRemoteCharacteristic->readValue().c_str();  // Use Arduino String for compatibility
    Serial.print("Characteristic value: ");
    Serial.println(value);
  }

  // Register for notifications if supported
  if (pRemoteCharacteristic->canNotify()) {
    pRemoteCharacteristic->registerForNotify(notifyCallback);
  }

  connected = true;
  return true;
}

// Callback class for device scanning
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("Advertised Device: ");
    Serial.println(advertisedDevice.toString().c_str());

    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {
      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
    }
  }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE Client...");

  BLEDevice::init("ESP32 Client");

  BLEScan *pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);
}

void loop() {
  if (doConnect) {
    if (connectToServer()) {
      Serial.println("Connected to server.");
    } else {
      Serial.println("Failed to connect to server.");
    }
    doConnect = false;
  }

  if (connected) {
    // Example of writing a new value to the characteristic
    String newValue = "Uptime: " + String(millis() / 1000) + "s";
    Serial.println("Writing new characteristic value: " + newValue);

    pRemoteCharacteristic->writeValue(newValue.c_str(), newValue.length());
  }

  delay(1000);
}
