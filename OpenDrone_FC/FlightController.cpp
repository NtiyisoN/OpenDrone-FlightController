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
#include "Sensor/BMP180.h"
#include "Sensor/BNO080.h"

#include "Network/TCPServer.h"

#include "Controller/Calibration.h"
#include "Controller/Orientation.h"
#include "Controller/UltrasonicDistance.h"

#include "Network/TCPServer.h"

#include "XML/XMLParser.h"

#include <wiringPi.h>
#include <iostream>
#include <thread>
using namespace std;

Orientation *orientation;
Barometer *barometer;
UltrasonicDistance *ultrasonic;
XMLParser *parser;
//TCPServer *server;

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
	//server->startUp();
}

int FlightController::initObjects() 
{
	int rc = wiringPiSetupGpio();
	if (rc != 0)
	{
		//The GPIO-Setup did not work
		return 0x01;
	}
  
	orientation = new Orientation();
	barometer = new BMP180();
	ultrasonic = new UltrasonicDistance();
	parser = new XMLParser();
	//server = new TCPServer();

	return 0x00;
}

int FlightController::run()
{
	int code = initObjects();
	if (code != 0x00) {
		return (code);
  }

	delay(250);

	//thread server(runServer);
	thread pitchRollThread(runOrientation);
	thread barometerThread(runBarometer);
	//thread ultrasonicThread(runUltrasonic);

	delay(1000);

	Calibration *calibration = new Calibration(orientation);
	calibration->calibrate();

	delay(250);

	int i = 0;
	while (i < 15) {
		double *valuesPitchRoll = orientation->getPitchRoll();
		double *valuesBarometer = barometer->getBarometerValues();
		list<double> valuesUltrasonic = ultrasonic->getDistance();

		cout << i << " Pitch: " << valuesPitchRoll[0] << " Roll: " << valuesPitchRoll[1] <<
			" Temperature: " << valuesBarometer[0] << " Pressure: " << valuesBarometer[1] << "\n";
		i++;
		delay(10);
	}

	orientation->interruptOrientation();
	barometer->interruptBaromter();

	//server.join();
	pitchRollThread.join();
	barometerThread.join();
	//ultrasonicThread.join();
  
	return (0);
}

FlightController::~FlightController()
{
}
