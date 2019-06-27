/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: This class calls the Orientation-Sensor and returns the current Pitch/Roll/Yaws
 *
 * 	@author Thomas Brych, Tim Klecka
 * 	@version 0.0.2 27.06.2019
 */
#pragma once
#include "../Sensor/BNO080.h"

class Orientation
{
public:
	Orientation();
	~Orientation();
	double *getPitchRoll();
	double *getPitchRollReal();
	void runOrientation();
	void interruptOrientation();
	void setCalibration(double *ar);

private:
	BNO080 *bno;
	bool run = false;

	double calPitch = 0.0, calRoll = 0.0, calYaw = 0.0;
};

