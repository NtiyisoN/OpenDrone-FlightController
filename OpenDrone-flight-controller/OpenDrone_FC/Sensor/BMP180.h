/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 */

#pragma once
class BMP180
{
public:
	BMP180();
	float *getBarometerValues();
	~BMP180();
};

