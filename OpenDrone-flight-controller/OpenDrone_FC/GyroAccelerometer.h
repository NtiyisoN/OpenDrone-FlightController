#include "Filter.h"
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

