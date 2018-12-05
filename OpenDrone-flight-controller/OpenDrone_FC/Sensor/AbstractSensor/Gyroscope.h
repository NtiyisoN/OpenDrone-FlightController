/*
* Copyright (c) OpenDrone, 2018.  All rights reserved.
* The entire project (including this file) is licensed under the GNU GPL v3.0
*/

#pragma once
class Gyroscope
{
public:
	virtual double *getGyroscopeValues() = 0;
};