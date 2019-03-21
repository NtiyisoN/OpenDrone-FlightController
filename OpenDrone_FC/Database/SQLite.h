#include <string>
#include <sqlite3.h>
#include "../Controller/Orientation.h"

#pragma once
class SQLite
{
public:
	sqlite3 *db;

	~SQLite();
	SQLite();
	void startSQL(Orientation *o);
	bool initSQL(char* name);
	void interruptSQL();

private:
	bool run = false;
};
