/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 */

#include "BMP180.h"
#include <bmp180.h>
#include <wiringPi.h>
#include <iostream>

BMP180::BMP180()
{
	bmp180Setup(200);
}

void BMP180::calcBaromter()
{
	this->temperature = analogRead(200 + 0) / 10.0; //Temperatur
	this->pressure = analogRead(200 + 1) / 10.0; //Pressure - hPa
}

double *BMP180::getBarometerValues()
{
	static double ar[2];
	if (this->run)
	{
		ar[0] = this->temperature;
		ar[1] = this->pressure;
	}
	else
	{
		ar[0] = NULL;
		ar[1] = NULL;
	}
	return ar;
}

void BMP180::runBarometer()
{
	this->run = true;
	while (this->run)
	{
		this->calcBaromter();
	}
}

void BMP180::interruptBaromter() 
{
	this->run = false;
}

BMP180::~BMP180()
{
}
