#include "SQLite.h"
#include <stdio.h>
#include <sqlite3.h>
#include "../Controller/Exit.h"
#include "../Controller/Orientation.h"
#include "../Controller/PID.h"
#include "wiringPi.h"
#include <string.h>
#include <sstream>
using namespace std;

SQLite::SQLite()
{	
}

static int callback(void *data, int argc, char **argv, char **azColName) {
	int i;
	fprintf(stderr, "%s: ", (const char*)data);

	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}

	printf("\n");
	return 0;
}

void SQLite::startSQL(Orientation *o) {
	char *zErrMsg = 0;
	int rc;
	char *sql;
	const char* data = "Callback function called";

	PID *pid = PID::getInstance(NULL, NULL);
	run = true;
	while (run == true) {
		double *ar = o->getPitchRollReal();
		string sql;
		if (ar[0] != NULL) {
			int m = millis();

			//Create SQL statement
			sql = "INSERT INTO Orientation (timestamp,droneid,pitch,roll,yaw) VALUES (" + to_string(m) + ",1," + to_string(ar[0]) + "," + to_string(ar[1]) + "," + to_string(ar[2]) + ");";

			//Execute SQL statement
			rc = sqlite3_exec(db, sql.c_str(), callback, (void*)data, &zErrMsg);

			if (rc != SQLITE_OK) {
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
			}

			int *arThrottle = pid->getThrottles();
			//Create SQL statement
			sql = "INSERT INTO Motors (timestamp,droneid,MotorSpeed1, MotorSpeed2, MotorSpeed3, Motorspeed4) VALUES (" + to_string(m) + ",1," + to_string(arThrottle[0]) + "," + to_string(arThrottle[1]) + "," + to_string(arThrottle[2]) + "," + to_string(arThrottle[3]) + ");";

			// Execute SQL statement
			rc = sqlite3_exec(db, sql.c_str(), callback, (void*)data, &zErrMsg);

			if (rc != SQLITE_OK) {
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
			}

			float *arPID = pid->getPIDVals();
			/* Create SQL statement */
			sql = "INSERT INTO PID_Values (timestamp, droneid, PIDPitch, PIDRoll, PIDYaw) VALUES (" + to_string(m) + ",1,'" + to_string(arPID[0]) + "','" + to_string(arPID[1]) + "','" + to_string(arPID[2]) + "');";

			/* Execute SQL statement */
			rc = sqlite3_exec(db, sql.c_str(), callback, (void*)data, &zErrMsg);

			if (rc != SQLITE_OK) {
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
			}
		}
		delay(1);
	}
	sqlite3_close(db);
}

bool SQLite::initSQL(char* name)
{
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open("/home/pi/projects/opendrone", &db);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	}
	else {
		fprintf(stderr, "Opened database successfully\n");
	}

	return true;
}

void SQLite::interruptSQL() {
	run = false;
}

SQLite::~SQLite()
{
}
