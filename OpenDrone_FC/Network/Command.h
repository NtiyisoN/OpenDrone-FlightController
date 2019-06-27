/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: This class is used, to save the commands received by the app
 *
 * 	@author Markus Kurzmann
 * 	@version 0.0.2 26.06.2019
 */
#pragma once
#include "iostream";
#include <string>;
#include <iostream>;

class Command
{
public:
	Command(long timestamp, int droneID, std::string cmd);
	~Command();

	long timestamp;
	int droneID = 1;
	std::string command;
};

