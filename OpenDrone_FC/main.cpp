/**********************************************************************
* !! WORK IN PROGRESS !!
* 
* Some classes/methods are not
* described or undone!
* The Sensor classes are depricated not described yet, because
* many of them are currently not in use!
*    --> HCSR04, BNO080, BMP280 are currently in use on the drone!
*
* !! WORK IN PROGRESS !!
***********************************************************************/

/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: The main class of the OpenDrone-Project
 *
 * 	@author Tim Klecka, Thomas Brych
 * 	@version 0.0.2 26.06.2019
 */
#include "FlightController.h"
#include <iostream>
#include <string.h>
using namespace std;

/**
	Start our Flightcontroller
	@param int arg

	@return void
*/
void startFC(int arg) {
	FlightController* fc = new FlightController(arg);

	cout << "Starting the Flight Controller\n";
	fc->run();
	cout << "Stopping Flight Controller!\n";
}

/**
	Main Method of our Flightcontroller
	@param int argc, char * argv[]
	@return Errorcode
*/
int main(int argc, char* argv[])
{
	//Arguments should be used to start the FC in a specific mode (-d Debug, -l Log) --> not implemented yet
	if (argc == 1) {
		startFC(0);
	}
	else if (argc == 2) {
		if (strcmp(argv[1], "-d") == 0) {
			startFC(1);
		}
		else if (strcmp(argv[1], "-l") == 0) {
			startFC(2);
		}
		else {
			return 1;
		}
	}
	else {
		return 1;
	}
}