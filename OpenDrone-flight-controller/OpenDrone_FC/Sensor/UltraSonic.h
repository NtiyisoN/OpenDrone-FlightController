/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 */

#include "../Filter/Filter.h"
#pragma once
class UltraSonic
{
public:
	int pin_trigger;
	int pin_echo;
	int id;
	Filter *filter;

	UltraSonic(int pin_trigger, int pin_echo, int num);
	~UltraSonic();
	float distance();
	int getId();
};
