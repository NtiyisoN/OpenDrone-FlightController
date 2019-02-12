/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: TODO
 *
 * 	@author Tim Klecka
 * 	@version 0.0.1 07.01.2019
 */
#include "FlightController.h"

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

#include "XML/XMLParser.h"

#include "Motor/PWMMotorTest.h"

#include <wiringPi.h>
#include <iostream>
#include <thread>
#include <fstream>
using namespace std;

PWMMotorTest *pw = new PWMMotorTest();

Orientation *orientation;
Barometer *barometer;
UltrasonicDistance *ultrasonic;
XMLParser *parser;
TCPServer *server;
Exit *error;

FlightController::FlightController()
{
}

static void runUltrasonic()
{
	ultrasonic->runUltrasonic();
}

static void runBarometer()
{
	barometer->runBarometer();
}

static void runOrientation()
{
	orientation->runOrientation();
}

static void runServer()
{
	server->startUp();
}

void FlightController::initObjects() 
{
	error = Exit::getInstance();

	int rc = wiringPiSetupGpio();
	if (rc != 0)
	{
		//The GPIO-Setup did not work
		error->sendError(0x01, true);
		return;
	}
  
	orientation = new Orientation();
	barometer = new BMP180();
	//ultrasonic = new UltrasonicDistance();
	//parser = new XMLParser();
}

int FlightController::run()
{
	server = TCPServer::getInstance();
	thread serverThread(runServer);
	//while (!server->connected) { delay(50); };
	
	initObjects();

	delay(250);

	thread pitchRollYawThread(runOrientation);
	thread barometerThread(runBarometer);

	delay(1000);

	int i = 0;
	while (i < 100000) {
		double *valuesPitchRollYaw = orientation->getPitchRoll();
		double *valuesBarometer = barometer->getBarometerValues();
		cout << i << " Pitch: " << valuesPitchRollYaw[0] << " Roll: " << valuesPitchRollYaw[1] << " Yaw: " << valuesPitchRollYaw[2] <<
			" Temperature: " << valuesBarometer[0] << " Pressure: " << valuesBarometer[1] << "\n";
		i++;
		delay(100);
	}

	//orientation->interruptOrientation();
	//barometer->interruptBaromter();

	serverThread.join();
	pitchRollYawThread.join();
	barometerThread.join();
  
	return (0);
}

FlightController::~FlightController()
{
}
