/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: TODO
 *
 * 	@author Tim Klecka
 * 	@version 0.0.1 07.01.2019
 */
#pragma once
#include "AbstractSensor/Magnetometer.h"
#include "AbstractSensor/Accelerometer.h"
#include "AbstractSensor/Gyroscope.h"

class BMF055 : 
	public virtual Magnetometer,
	public virtual Accelerometer,
	public virtual Gyroscope
{
public:
	BMF055();
	~BMF055();

	double *getAccelerometerValues();
	double *getGyroscopeValues();
	double *getMagnetometerValues();
};

