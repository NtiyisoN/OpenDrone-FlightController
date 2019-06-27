/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: This class is used to calibrate the drone
 *
 * 	@author Thomas Brych, Tim Klecka
 * 	@version 0.0.2 27.06.2019
 */
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

