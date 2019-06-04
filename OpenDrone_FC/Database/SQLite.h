#include <string>
#include <sqlite3.h>
#include "../Controller/Orientation.h"
#include "../Sensor/HCSR04.h"

#pragma once
class SQLite
{
public:
	sqlite3 *db;

	~SQLite();
	SQLite();
	void startSQL(Orientation *o, HCSR04 *ultrasonic);
	bool initSQL(char* name);
	void interruptSQL();

private:
	bool run = false;
};
