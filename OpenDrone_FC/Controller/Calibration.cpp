/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: This class is used to calibrate the drone
 *
 * 	@author Thomas Brych, Tim Klecka
 * 	@version 0.0.2 27.06.2019
 */
#include "Calibration.h"
#include <iostream>
#include <wiringPi.h>
#include <math.h>
using namespace std;

Calibration::Calibration(Orientation *o)
{
	this->orientation = o;
}

Calibration::~Calibration()
{
}

/**
	Method to run the calibration
	@return void

	@params PID *pid
*/
void Calibration::calibrate() {
	cout << "Starting Calibration\n";
	int length = 500;

	double pitch = 0.0, roll = 0.0, yaw = 0.0;
	for (int i = 0; i <= length; i++) {
		double* ar = orientation->getPitchRollReal();
		pitch += ar[0];
		roll += ar[1];
		yaw += ar[2];
		delay(3);
	}
	pitch = pitch / length;
	roll = roll / length;
	yaw = yaw / length;

	double ar[2];
	ar[0] = pitch;
	ar[1] = roll;
	ar[2] = yaw;
	cout << "Calibrate: " << ar[0] << " " << ar[1] << " " << ar[2] << "\n";

	orientation->setCalibration(ar);
}
