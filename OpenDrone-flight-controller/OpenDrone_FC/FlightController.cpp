/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 */

#include "FlightController.h"

#include "Sensor/AbstractSensor/Barometer.h"
#include "Sensor/AbstractSensor/Ultrasonic.h"
#include "Sensor/BMP388.h"
#include "Sensor/BMP180.h"

#include "Controller/Calibration.h"
#include "Controller/Orientation.h"
#include "Controller/UltrasonicDistance.h"

#include <wiringPi.h>
#include <iostream>
#include <thread>
using namespace std;

Orientation *orientation;
Barometer *barometer;
UltrasonicDistance *ultrasonic;

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

bool FlightController::initObjects() 
{
	int rc = wiringPiSetupGpio();
	if (rc != 0)
	{
		cout << "Failed to wiringPiSetupGpio()\n";
		return false;
	}

	orientation = new Orientation();
	barometer = new BMP180();
	ultrasonic = new UltrasonicDistance();

	return true;
}

int FlightController::run()
{	
	bool worked = initObjects();
	if (!worked) {
		return (1);
	}

	delay(250);

	thread pitchRollThread(runOrientation);
	thread barometerThread(runBarometer);
	//thread ultrasonicThread(runUltrasonic);

	delay(1000);

	Calibration *calibration = new Calibration(orientation);
	calibration->calibrate();

	delay(250);

	int i = 0;
	while (i < 20) {
		double *valuesPitchRoll = orientation->getPitchRoll();
		double *valuesBarometer = barometer->getBarometerValues();
		list<double> valuesUltrasonic = ultrasonic->getDistance();

		cout << i << " Pitch: " << valuesPitchRoll[0] << " Roll: " << valuesPitchRoll[1] <<
			" Temperature: " << valuesBarometer[0] << " Pressure: " << valuesBarometer[1] << "\n";
		i++;
	}

	orientation->interruptOrientation();
	barometer->interruptBaromter();

	pitchRollThread.join();
	barometerThread.join();
	//ultrasonicThread.join();

	return (0);
}

FlightController::~FlightController()
{
}
