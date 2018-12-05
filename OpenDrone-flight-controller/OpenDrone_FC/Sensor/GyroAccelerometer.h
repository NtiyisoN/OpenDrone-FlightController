/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 */

#include "../Filter/Filter.h"
#pragma once
class GyroAccelerometer
{
public:
	GyroAccelerometer();
	~GyroAccelerometer();
	int fd;
	Filter *filterAccX;
	Filter *filterAccY;
	Filter *filterAccZ;
	Filter *filterGyroX;
	Filter *filterGyroY;
	Filter *filterGyroZ;

	float *getValues(void);
	short readRawData(int addr);
};

