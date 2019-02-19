#include "Orientation.h"

#pragma once
class Calibration
{
public:
	Calibration(Orientation *o);
	~Calibration();
	double *calibrate();

private:
	Orientation *orientation;
};

