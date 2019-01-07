/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: TODO
 *
 * 	@author Tim Klecka
 * 	@version 0.0.1 07.01.2019
 */
#include "BMP180.h"
#include <bmp180.h>
#include <wiringPi.h>

BMP180::BMP180()
{
	bmp180Setup(200);
}

int *BMP180::getBarometerValues()
{
	static int ar[2];
	ar[0] = analogRead(200 + 0); //Temperatur
	ar[1] = analogRead(200 + 1); //Pressure - hPa
	return ar;
}

BMP180::~BMP180()
{
}
