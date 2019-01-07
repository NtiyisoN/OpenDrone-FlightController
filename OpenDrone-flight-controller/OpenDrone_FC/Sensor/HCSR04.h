/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: TODO
 *
 * 	@author Tim Klecka
 * 	@version 0.0.1 07.01.2019
 */
#pragma once
#include "./AbstractSensor/Ultrasonic.h"
#include "../Filter/Filter.h"

class HCSR04 : public virtual Ultrasonic
{
public:
	int pin_trigger;
	int pin_echo;
	int id;
	Filter *filter;

	HCSR04(int pin_trigger, int pin_echo, int num);
	~HCSR04();
	double distance();
	int getId();
};
