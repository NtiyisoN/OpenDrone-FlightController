#pragma once
#include "AbstractSensor/Magnetometer.h"

class BMM150 : public virtual Magnetometer
{
public:
	int fd;

	BMM150();
	~BMM150();
	double *getMagnetometerValues();
	short readRawData(int add, int bitshift);
	void selfTest();
	void calibrate();
};

