/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: TODO
 *
 * 	@author Tim Klecka, Thomas Brych
 * 	@version 0.0.1 14.02.2019
 */
#include <iostream>
#include "FlightController.h"
using namespace std;

/**
	Main Method of our Flightcontroller
	@param int argc, char * argv[]
	@return Errorcode
*/
int main(int argc, char * argv[])
{
	cout << "args\t" << argc << "\t" << argv;
	FlightController *fc = new FlightController();

	cout << "Starting the Flight Controller\n";
	fc->run();

	cout << "Stopping Flight Controller!\n";
	return (0);
}