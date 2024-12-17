#include "BLEDevice.h"

// UUID for the BLE service (optional if not using any specific service for just distance)
static BLEUUID serviceUUID("12345678-1234-1234-1234-123456789abc");

static BLEAdvertisedDevice *myDevice = nullptr; // Holds reference to the discovered device

// Callback for device scanning
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    // If the desired device is found, stop the scan and store the device
    if (advertisedDevice.getServiceUUID().equals(serviceUUID)) {
      Serial.print("Advertised Device: ");
      Serial.println(advertisedDevice.toString().c_str());
      Serial.print("RSSI: ");
      Serial.println(advertisedDevice.getRSSI());
      Serial.print("Estimated distance: ");
      
      // Estimate distance using RSSI
      int rssi = advertisedDevice.getRSSI();
      float distance = pow(10.0, (27.55 - (20 * log10(2400)) + fabs(rssi)) / 20.0);
      Serial.println(distance);
      
      // Store the device reference and stop scanning
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      BLEDevice::getScan()->stop();  // Stop scanning after finding the target device
    }
  }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE Client...");

  BLEDevice::init("ESP32 Client");

  // Start scanning for BLE devices
  BLEScan *pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);  // 1.349s interval between scans
  pBLEScan->setWindow(449);     // 0.449s window for scanning
  pBLEScan->setActiveScan(true);  // Enable active scanning to get RSSI
  pBLEScan->start(5, false);      // Start scanning for 5 seconds
}

void loop() {
  if (myDevice != nullptr) {
    // If we've found the device, continuously measure the RSSI and distance
    BLEScan *pBLEScan = BLEDevice::getScan();  // Access BLE scan to get updated RSSI

    // Manually start scan for a small period and get RSSI update
    pBLEScan->start(1, false);  // Scan for 1 second

    // If the device is still found, measure the RSSI again
    int rssi = myDevice->getRSSI();
    if (rssi > -100) {  // Check if RSSI is valid (avoid extremely low RSSI values)
      Serial.print("RSSI: ");
      Serial.println(rssi);

      // Estimate distance using updated RSSI
      float distance = pow(10.0, (27.55 - (20 * log10(2400)) + fabs(rssi)) / 20.0);
      Serial.print("Estimated distance: ");
      Serial.print(distance);
      Serial.println(" meters");
    }

    delay(1000);  // Measure every 1 second
  }
}
