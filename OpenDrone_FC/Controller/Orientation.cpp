/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: This class calls the Orientation-Sensor and returns the current Pitch/Roll/Yaws
 *
 * 	@author Thomas Brych, Tim Klecka
 * 	@version 0.0.2 27.06.2019
 */
#include "Orientation.h"
#include <iostream>
#include <wiringPi.h>
#include <math.h>
using namespace std;

Orientation::Orientation()
{
	this->bno = new BNO080();
}

Orientation::~Orientation()
{
}

/**
	Method to start the Orientation object and start the BNO080 (called by the thread in the main.cpp)
	@return void
*/
void Orientation::runOrientation()
{
	this->run = true;
	this->bno->runBNO();
}

/**
	Method to interrupt the Orientation thread
	@return void
*/
void Orientation::interruptOrientation()
{
	this->run = false;
}

/**
	This method is called after the calibration and is used to storage the calibrated 
	@return void

	@params double *ar
*/
void Orientation::setCalibration(double *ar) {
	calPitch = ar[0];
	calRoll = ar[1];
	calYaw = ar[2];
}

/**
	This method is used to get the real Pitch/Roll/Yaw values (without calibration)
	This method should only be used for calibrating the drone!
	@return double *
*/
double *Orientation::getPitchRollReal()
{
	static double ar[4];
	if (this->run)
	{
		ar[0] = this->bno->gyroIntegratedRotationVectorData.lastPitch;
		ar[1] = this->bno->gyroIntegratedRotationVectorData.lastRoll;
		ar[2] = this->bno->gyroIntegratedRotationVectorData.lastYaw;
		ar[3] = this->bno->gyroIntegratedRotationVectorData.time;
	}
	else
	{
		ar[0] = NULL;
		ar[1] = NULL;
		ar[2] = NULL;
		ar[3] = NULL;
	}

	return ar;
}

/**
	This method is used to get the calibrated Pitch/Roll/Yaw values
	This method should not be used for calibrating the drone!
	@return double *
*/
double *Orientation::getPitchRoll()
{
	static double ar[4];
	if (this->run)
	{
		ar[0] = this->bno->gyroIntegratedRotationVectorData.lastPitch - calPitch;
		ar[1] = this->bno->gyroIntegratedRotationVectorData.lastRoll - calRoll;
		ar[2] = this->bno->gyroIntegratedRotationVectorData.lastYaw - calYaw;
		ar[3] = this->bno->gyroIntegratedRotationVectorData.time;
	}
	else
	{
		ar[0] = NULL;
		ar[1] = NULL;
		ar[2] = NULL;
		ar[3] = NULL;
	}

	return ar;
}

//Deprecated code (might be used again later)
/*void Orientation::calcPitchRoll()
{
	double K = 0.98;
	double K1 = 1 - K;
	double *valuesGyro = this->gyro->getGyroscopeValues();
	double *valuesAcc = this->acc->getAccelerometerValues();
	double combined[6];
	combined[0] = valuesGyro[1];
	combined[1] = valuesGyro[2];
	combined[2] = valuesGyro[3];
	combined[3] = valuesAcc[1];
	combined[4] = valuesAcc[2];
	combined[5] = valuesAcc[3];

	//TODO: read from XML
	//values[0] -= gyro_offset_x;
	//values[1] -= gyro_offset_y;

	double gyro_x_delta = (combined[0] * 0.01);
	double gyro_y_delta = (combined[1] * 0.01);

	double rotation_x = getXRotation(combined[3], combined[4], combined[5]);
	double rotation_y = getYRotation(combined[3], combined[4], combined[5]);

	this->roll = (K * (gyro_x_delta)+(K1 * rotation_x)) * 50;
	this->pitch = (K * (gyro_y_delta)+(K1 * rotation_y)) * 50;
}

double Orientation::getXRotation(double accX, double accY, double accZ)
{
	return (atan2(accX, sqrt((accY * accY) + (accZ * accZ))))*(180 / M_PI);
}

double Orientation::getYRotation(double accX, double accY, double accZ)
{
	return (atan2(accY, sqrt((accX * accX) + (accZ * accZ))))*(180 / M_PI);
}*/
