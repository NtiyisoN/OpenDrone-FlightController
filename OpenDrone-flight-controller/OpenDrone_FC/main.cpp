/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: TODO
 *
 * 	@author Tim Klecka
 * 	@version 0.0.1 07.01.2019
 */
#include <iostream>
#include "FlightController.h"
using namespace std;

/**
	Main Method of our Flightcontroller
	@param void
	@return Errorcode
*/
int main(void)
{
	FlightController *fc = new FlightController();

	cout << "Starting the Flight Controller\n";
	int status = fc->run();
	if (status == 0)
	{
		cout << "Stopping Flight Controller!";
		return (0);
	}
	else
	{
		cout << "Stopping Flight Controller!";
		return (status);
	}
}