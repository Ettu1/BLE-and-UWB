#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <math.h>
#include <Arduino.h>

// Constants
#define SCAN_TIME 1 // seconds
#define IDLE_TIMEOUT 10000
const int MIN_RSSI = -80;
const int numReadings = 5; // Number of RSSI samples to average

String beaconAddresses[] = {
    "34:85:18:a5:62:65",
    "34:85:18:a4:70:1d",
    "34:85:18:a4:b6:d1",
    "34:85:18:a4:d0:9d"
};

float rssi_avg[4] = {-100, -100, -100, -100};

// BLE scan object
BLEScan* pBLEScan;
unsigned long lastScanTime = 0;
bool isIdle = true;

// Smooth RSSI using exponential smoothing
float smoothRSSI(float rssi, float avgRSSI) {
    return 0.5 * rssi + 0.5 * avgRSSI;
}

// BLE callback to process discovered beacons
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        String address = advertisedDevice.getAddress().toString();
        int rssi = advertisedDevice.getRSSI();

        for (int i = 0; i < 4; i++) {
            if (address == beaconAddresses[i] && rssi > MIN_RSSI) {
                rssi_avg[i] = smoothRSSI(rssi, rssi_avg[i]);

                // Average multiple RSSI readings
                int rssi_sum = 0;
                for (int j = 0; j < numReadings; j++) {
                    rssi_sum += advertisedDevice.getRSSI(); // Accumulate RSSI values
                }
                int rssi_avg_val = rssi_sum / numReadings;
                rssi_avg[i] = rssi_avg_val; // Update averaged RSSI value
                
                isIdle = false;
                lastScanTime = millis();
            }
        }
    }
};

void printRSSI() {
    Serial.print("1: "); Serial.print(rssi_avg[0]); Serial.print(", ");
    Serial.print("2: "); Serial.print(rssi_avg[1]); Serial.print(", ");
    Serial.print("3: "); Serial.print(rssi_avg[2]); Serial.print(", ");
    Serial.print("4: "); Serial.println(rssi_avg[3]);
}

void setup() {
    Serial.begin(115200);
    BLEDevice::init("");
    pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true);
}

void loop() {
    pBLEScan->start(SCAN_TIME, false);

    if (millis() - lastScanTime > IDLE_TIMEOUT) isIdle = true;
    if (isIdle) {
        Serial.println("Entering deep sleep...");
        ESP.deepSleep(0);
    }

    // If RSSI values are valid, print them
    if (rssi_avg[0] != -100 || rssi_avg[1] != -100 || rssi_avg[2] != -100 || rssi_avg[3] != -100) {
        printRSSI();
    }

    delay(1000);
}
