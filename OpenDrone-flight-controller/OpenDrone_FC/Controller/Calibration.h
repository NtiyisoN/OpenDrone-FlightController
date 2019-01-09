#include "Orientation.h"

#pragma once
class Calibration
{
public:
	Calibration(Orientation *o);
	~Calibration();
	bool calibrate();
private:
	Orientation *orientation;
	bool writeToXML(double *values);
};

