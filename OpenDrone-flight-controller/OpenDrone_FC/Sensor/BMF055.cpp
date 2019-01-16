/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: TODO
 *
 * 	@author Tim Klecka
 * 	@version 0.0.1 07.01.2019
 */
#include "BMF055.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <iostream>
using namespace std;

BMF055::BMF055()
{
}

double *BMF055::getAccelerometerValues() {
	return NULL;
}

double *BMF055::getGyroscopeValues() {
	return NULL;
}

double *BMF055::getMagnetometerValues() {
	return NULL;
}

BMF055::~BMF055()
{
}
