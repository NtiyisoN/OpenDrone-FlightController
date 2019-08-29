/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: TODO
 *
 * 	@author Tim Klecka
 * 	@version 0.0.1 07.01.2019
 */
#pragma once
class Ultrasonic
{
public:
	virtual double getDistance() = 0;
	virtual void runUltrasonic() = 0;
	virtual void interruptUltrasonic() = 0;
};
