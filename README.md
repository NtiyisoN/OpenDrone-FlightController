[![CodeFactor](https://www.codefactor.io/Content/badges/A.svg)](https://www.codefactor.io/repository/github/opendroneat/opendrone-flightcontroller/overview/develop)
[![Gitter chat](https://badges.gitter.im/USER/REPO.png)](https://gitter.im/OpenDroneAT/Lobby "Gitter chat")
  
# !! Work in Progress !!  
## Infomation
  
Open Drone Open-source Project  
LICENSE GNU GPLv3  
  
WEB http://opendrone.at/  
WIKI  https://github.com/OpenDroneAT/OpenDrone-FlightController/wiki#our-fly-opendrone-wiki

We here at OpenDrone love Open Source and Drones. So why not combine them?  
So this is an open source project for a self-flying and navigating drone.  
We also got an Open-source Frame Created with ❤️ by our 3D Printer  

Used technologies:  
* Android - Our App runs on Android  
* C++ - Our flight controller runs on C++  
* Raspberry Pi - The brain of the drone  
---  
## How to contribute  
### FC  
* Visual Studio  
* Visual C++ for Linux Development  
### Android App  
* Android Studio 3.3  
### [Read This](https://github.com/OpenDroneAT/OpenDrone-FlightController/blob/develop/CODE_OF_CONDUCT.md#contributor-covenant-code-of-conduct "code-of-conduct").
---  
## Features  
### App
* Configure Drone ✔  
* Create/Modify Flightplans ✔   
* GUI for Manual Flight ✔  
* Communtication ✔  
### FC  
* Rotors rotating round and round ✔  
* Reading Sensors ✔  
* Filtering sensor values ✔ 
* PID Controller ⏳  
  * Ptch / Roll  ✔ 
  * Yaw ⏳
  * Hight ⏳
  * Position ⏳
* Communtication ✔ works but we might change the protocoll
* Fly it to the moon and back ⏳
---  
## Supported Sensors  
### Gyroscope / Accelerometer  
* BMI088 ⏳  
* BNO080/85 ✔  
* MPU6050 ✔  
* BMF055 ⏳  
### Magnetometer  
* HMC5883L ⏳  
* BMF055 ⏳  
* BNO080/85 ✔  
* BMM150 ⏳  
### Barometer  
* BMP180 ✔  
* BMP280 ✔  
* BMP388 ⏳  
### GPS
* BN-880 ⏳ 
### Other
* HC-SR04 ✔  
---  
## Requirements  
### App  
* Android 6.0  
* WiFi  
* We are also working on a Cross Platform App, stay tuned
### Flight Controller  
* Raspberry Pi 3B or better
* Gyro, Accelerometer
* Magnetometer
* Barometer
* GPS only Auto. Flight
* ESC
* Motors  
  
---
