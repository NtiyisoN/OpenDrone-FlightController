/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: TODO
 *
 * 	@author Tim Klecka
 * 	@version 0.0.1 07.01.2019
 */
#pragma once
#include "AbstractSensor/Barometer.h"
class BMP280 :
	public virtual Barometer
{
public:
	BMP280();
	~BMP280();
	double *getBarometerValues();
	void runBarometer();
	void interruptBaromter();

private:
	int fd;
	bool run = false;
	double temperature = 0, pressure = 0;
	int readRawData(int addr);
	void calcBaromter();
};

