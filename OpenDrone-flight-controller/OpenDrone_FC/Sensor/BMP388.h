#pragma once
#include "AbstractSensor/Barometer.h"
class BMP388 :
	public virtual Barometer
{
public:
	BMP388();
	~BMP388();
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

