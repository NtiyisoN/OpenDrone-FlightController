/*
* Copyright (c) OpenDrone, 2018.  All rights reserved.
* The entire project (including this file) is licensed under the GNU GPL v3.0
*/

#pragma once
class Barometer
{
public:
	//ar[0] = Temperature, ar[1] = Pressure
	virtual int *getBarometerValues() = 0;
};