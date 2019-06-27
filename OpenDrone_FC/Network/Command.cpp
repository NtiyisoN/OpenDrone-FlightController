/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: This class is used, to save the commands received by the app
 *
 * 	@author Markus Kurzmann
 * 	@version 0.0.2 26.06.2019
 */
#include "Command.h"
#include <string>

Command::Command(long timestamp, int droneID, std::string cmd) {
	this->timestamp = timestamp;
	this->droneID = 1;
	this->command = cmd;
}

Command::~Command()
{
}
