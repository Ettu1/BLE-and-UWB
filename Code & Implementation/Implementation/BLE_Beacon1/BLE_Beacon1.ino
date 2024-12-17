#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// Define Service and Characteristic UUIDs
#define SERVICE_UUID        "12345678-1234-1234-1234-123456789abc"
#define CHARACTERISTIC_UUID "12345678-1234-1234-1234-123456789abd"

void setup() {
    // Start Serial for debugging
    Serial.begin(115200);
    Serial.println("Starting BLE Beacon 1...");

    // Initialize BLE device
    BLEDevice::init("BLE Beacon 1");

    // Create BLE Server
    BLEServer *pServer = BLEDevice::createServer();

    // Create BLE Service
    BLEService *pService = pServer->createService(BLEUUID(SERVICE_UUID));

    // Create BLE Characteristic
    BLECharacteristic *pCharacteristic = pService->createCharacteristic(
        BLEUUID(CHARACTERISTIC_UUID),
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
    );

    // Set initial value for the characteristic
    pCharacteristic->setValue("Hello from Beacon 1!");

    // Start the service
    pService->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true); // Optional: add scan response data
    pAdvertising->start();

    Serial.println("BLE Beacon 1 is now advertising...");
}

void loop() {
    // Nothing to do in the loop for the beacon, it continues advertising
    delay(1000); // Optional delay for debugging consistency
}
