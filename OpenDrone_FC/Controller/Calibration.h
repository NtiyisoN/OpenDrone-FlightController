#include "Orientation.h"

#pragma once
class Calibration
{
public:
	Calibration(Orientation *o);
	~Calibration();
	void calibrate();

private:
	Orientation *orientation;
};

