/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 */

#include "FlightController.h"

#include "Sensor/AbstractSensor/Ultrasonic.h"
#include "Sensor/AbstractSensor/Barometer.h"
#include "Sensor/HCSR04.h"
#include "Sensor/BMP388.h"
#include "Sensor/BMP180.h"

#include "Controller/Calibration.h"
#include "Controller/Orientation.h"

#include <wiringPi.h>
#include <iostream>
#include <thread>
using namespace std;

Orientation *orientation = new Orientation();
Barometer *barometer = new BMP388();

FlightController::FlightController()
{
}

static void *runUltrasonic()
{
	//TODO: Ultrasonic not working properly right now
	/*//Initializing the wiringPi-Gpio's
	int rc = wiringPiSetupGpio();
	if (rc != 0)
	{
		cout << "Failed to wiringPiSetupGpio()\n";
		exit(1);
	}

	Ultrasonic* sensors[2];
	//Initialize the sensors
	sensors[0] = new HCSR04(17, 27, 1);
	sensors[1] = new HCSR04(17, 27, 1);

	//Infinite loop to keep measuring --> TODO: Need to be changed
	double curDistance;
	while (1)
	{
		for (int i = 0; i < 2; i++)
		{
			curDistance = sensors[i]->distance();
			cout << "Ultrasonic " << sensors[i]->getId() << ": Distance: " << curDistance << "\n";
			delay(2);
		}
	*/
}

static void runBarometer()
{
	barometer->runBarometer();
}

static void runOrientation()
{
	orientation->runOrientation();
}

int FlightController::run()
{
	thread pitchRollThread(runOrientation);
	thread barometerThread(runBarometer);
	thread ultrasonicThread(runUltrasonic);

	delay(1000);

	Calibration *calibration = new Calibration(orientation);
	calibration->calibrate();

	delay(250);

	int i = 0;
	while (i < 20) {
		double *valuesPitchRoll = orientation->getPitchRoll();
		double *valuesBarometer = barometer->getBarometerValues();
		cout << i << " Pitch: " << valuesPitchRoll[0] << " Roll: " << valuesPitchRoll[1] <<
			" Temperature: " << valuesBarometer[0] << " Pressure: " << valuesBarometer[1] << "\n";
		i++;
	}

	orientation->interruptOrientation();
	barometer->interruptBaromter();

	pitchRollThread.join();
	barometerThread.join();
	ultrasonicThread.join();

	return (0);
}

FlightController::~FlightController()
{
}
