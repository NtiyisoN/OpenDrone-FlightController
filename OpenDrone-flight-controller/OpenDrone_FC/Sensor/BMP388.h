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
class BMP388 :
	public virtual Barometer
{
public:
	int fd;

	BMP388();
	int *getBarometerValues();
	int readRawData(int addr);
	~BMP388();
};

