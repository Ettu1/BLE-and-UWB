#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// Define unique Service and Characteristic UUIDs
#define SERVICE_UUID        "22345678-2234-2234-2234-223456789abc"
#define CHARACTERISTIC_UUID "22345678-2234-2234-2234-223456789abd"

void setup() {
    // Start Serial for debugging
    Serial.begin(115200);
    Serial.println("Starting BLE Beacon 2...");

    // Initialize BLE device with a unique name
    BLEDevice::init("BLE Beacon 2");

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
    pCharacteristic->setValue("Hello from Beacon 2!");

    // Start the service
    pService->start();

    // Start advertising with unique settings
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true); 
    pAdvertising->setMinInterval(100); // Minimum advertising interval
    pAdvertising->setMaxInterval(200); // Maximum advertising interval
    pAdvertising->start();

    Serial.println("BLE Beacon 2 is now advertising...");
}

void loop() {
    // No loop actions required for a simple beacon
    delay(1000);
}
