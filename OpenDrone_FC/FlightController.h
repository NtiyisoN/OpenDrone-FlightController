/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: TODO
 *
 * 	@author Thomas Brych, Tim Klecka
 * 	@version 0.0.1 14.02.2019
 */


#pragma once
class FlightController
{
public:
	FlightController(int arg);
	~FlightController();
	int run();

private:
	void initObjects();

};

