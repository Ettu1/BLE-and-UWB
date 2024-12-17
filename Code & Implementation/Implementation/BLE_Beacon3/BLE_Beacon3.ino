#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// Define unique Service and Characteristic UUIDs for Beacon 3
#define SERVICE_UUID        "32345678-3234-3234-3234-323456789abc"
#define CHARACTERISTIC_UUID "32345678-3234-3234-3234-323456789abd"

void setup() {
    // Start Serial for debugging
    Serial.begin(115200);
    Serial.println("Starting BLE Beacon 3...");

    // Initialize BLE device
    BLEDevice::init("BLE Beacon 3");

    // Create BLE Server
    BLEServer *pServer = BLEDevice::createServer();

    // Create BLE Service
    BLEService *pService = pServer->createService(BLEUUID(SERVICE_UUID));

    // Create BLE Characteristic
    BLECharacteristic *pCharacteristic = pService->createCharacteristic(
        BLEUUID(CHARACTERISTIC_UUID),
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
    );

    // Set a unique initial value for the characteristic
    pCharacteristic->setValue("Hello from Beacon 3!");

    // Start the service
    pService->start();

    // Start advertising with unique settings
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);  // Add scan response data
    pAdvertising->setMinInterval(100);  // Minimum advertising interval (in ms)
    pAdvertising->setMaxInterval(200);  // Maximum advertising interval (in ms)
    pAdvertising->start();

    Serial.println("BLE Beacon 3 is now advertising...");
}

void loop() {
    // Beacon 3 is just continuously advertising, no actions required in loop
    delay(1000);
}
