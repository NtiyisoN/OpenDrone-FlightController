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
#include <string.h>
#include "./Database/SQLite.h"
using namespace std;

/**
	Start our Flightcontroller
	@param int arg
	@return void
*/
void startFC(int arg) {
	FlightController *fc = new FlightController(arg);

	cout << "Starting the Flight Controller\n";
	fc->run();

	cout << "Stopping Flight Controller!\n";
}

/**
	Main Method of our Flightcontroller
	@param int argc, char * argv[]
	@return Errorcode
*/
int main(int argc, char * argv[])
{
	SQLite *sql = new SQLite();
	/*if (argc == 1) {
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
	}*/
}