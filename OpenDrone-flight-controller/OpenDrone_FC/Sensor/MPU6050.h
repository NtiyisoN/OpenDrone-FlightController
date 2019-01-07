/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: TODO
 *
 * 	@author Tim Klecka
 * 	@version 0.0.1 07.01.2019
 */
#include "../Filter/Filter.h"
#include "AbstractSensor/Accelerometer.h"
#include "AbstractSensor/Gyroscope.h"
#pragma once
class MPU6050 : public virtual Accelerometer, public virtual Gyroscope
{
public:
	MPU6050();
	~MPU6050();
	int fd;
	Filter *filterAccX;
	Filter *filterAccY;
	Filter *filterAccZ;
	Filter *filterGyroX;
	Filter *filterGyroY;
	Filter *filterGyroZ;

	short readRawData(int addr);

	double *getGyroscopeValues();
	double *getAccelerometerValues();
};

