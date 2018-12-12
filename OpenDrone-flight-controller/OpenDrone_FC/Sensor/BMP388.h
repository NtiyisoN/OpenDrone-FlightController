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

