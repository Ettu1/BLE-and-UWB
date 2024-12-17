# BLE-and-UWB
Analysis of Indoor Localization Methods

This project focuses on the implementation and analysis of Bluetooth Low Energy (BLE) and Ultra-Wideband (UWB) technologies to develop reliable indoor localization systems for smart home automation.

The methodology for BLE localization involves the use of Received Signal Strength Indicator (RSSI) values to estimate distances. BLE beacons were deployed at fixed positions within the environment, and their RSSI values were measured by a mobile device or receiver.

For UWB localization, the methodology utilized the Time of Flight (ToF) technique to determine distances with high precision. UWB anchors were installed within the test environment, and UWB tags collected ranging data with sub-meter accuracy. Similar to BLE, the Multilateration algorithm was employed to compute the device’s position. UWB required more specialized hardware and a precise calibration process, making its implementation more complex.

The results of the study revealed significant differences between the two methods. BLE demonstrated lower accuracy, with positioning errors ranging from 1 to 3 meters. On the other hand, UWB achieved accuracy, offering far superior precision compared to BLE.

In summary, this project provides a comparative analysis of BLE and UWB indoor localization technologies, implementing and testing both methods in a controlled environment.

The codes and results are all here.
