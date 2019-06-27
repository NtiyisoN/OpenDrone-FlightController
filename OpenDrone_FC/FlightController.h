/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: The class, that initializes all objects and starts all the necessary threads
 *
 * 	@author Thomas Brych, Tim Klecka
 * 	@version 0.0.2 26.06.2019
 */
#pragma once
#include "Sensor/AbstractSensor/Barometer.h"
#include "Sensor/AbstractSensor/Ultrasonic.h"
#include "Network/TCPServer.h"
#include "Controller/Orientation.h"
#include "Controller/Exit.h"
#include "Controller/PID.h"
#include "Database/SQLite.h"
#include "Motor/PWMMotorTest.h"

class FlightController
{
public:
	FlightController(int arg);
	~FlightController();
	int run();

private:
	int arg;
	Orientation *orientation;
	Barometer *barometer;
	Ultrasonic *ultrasonic;
	TCPServer *server;
	Exit *error;
	PID *pid;
	PWMMotorTest *pwm;
	SQLite *sql;

	void initObjects();
};
