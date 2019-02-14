/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: TODO
 *
 * 	@author Thomas Brych, Tim Klecka
 * 	@version 0.0.1 14.02.2019
 */
#include "Sensor/AbstractSensor/Barometer.h"
#include "Sensor/AbstractSensor/Ultrasonic.h"
#include "Sensor/BMP388.h"
#include "Sensor/BMP280.h"
#include "Sensor/BMP180.h"
#include "Sensor/BNO080.h"

#include "Network/TCPServer.h"

#include "Controller/Calibration.h"
#include "Controller/Orientation.h"
#include "Controller/UltrasonicDistance.h"
#include "Controller/Exit.h"
#include "Controller/PID.h"

#include "Motor/PWMMotorTest.h"

#include "XML/XMLParser.h"

#include "Motor/PWMMotorTest.h"

#include <wiringPi.h>
#include <iostream>
#include <thread>
#include <fstream>

#pragma once
class FlightController
{
public:
	FlightController();
	~FlightController();
	int run();

private:
	void initObjects();

};

