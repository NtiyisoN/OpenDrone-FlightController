#pragma once
#include "AbstractSensor/Magnetometer.h"

class BMM150 : public virtual Magnetometer
{
public:
	BMM150();
	~BMM150();
	double *getMagnetometerValues();
	void selfTest();
	void calibrate();

private:
	int fd;
	short readRawData(int add, int bitshift);
};

