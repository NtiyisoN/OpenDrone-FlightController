/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: This class logs the important data into our SQLite3-Database
 *
 * 	@author Markus Kurzmann, Thomas Brych
 * 	@version 0.0.2 26.06.2019
 */
#pragma once
#include <string>
#include <sqlite3.h>
#include "../Controller/Orientation.h"
#include "../Sensor/AbstractSensor/Ultrasonic.h"

class SQLite
{
public:
	~SQLite();
	SQLite();
	void startSQL(Orientation *o, Ultrasonic *ultrasonic);
	bool initSQL(char* name);
	void interruptSQL();

private:
	sqlite3 *db;

	bool run = false;
};
