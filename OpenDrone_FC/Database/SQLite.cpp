/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: This class logs the important data into our SQLite3-Database
 *
 * 	@author Markus Kurzmann, Thomas Brych
 * 	@version 0.0.2 26.06.2019
 */
#include "SQLite.h"
#include <stdio.h>
#include <sqlite3.h>
#include "../Controller/Exit.h"
#include "../Controller/Orientation.h"
#include "../Sensor/AbstractSensor/Ultrasonic.h"
#include "../Controller/PID.h"
#include "wiringPi.h"
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>    
using namespace std;

SQLite::SQLite()
{	
}

SQLite::~SQLite()
{
}

/**
	The Callback-Function used for the database
	@return int

	@params void *data, int argc, char **argv, char **azColName
*/
/*static int callback(void *data, int argc, char **argv, char **azColName) {
	int i;
	fprintf(stderr, "%s: ", (const char*)data);

	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}

	printf("\n");
	return 0;
}*/

/**
	This function logs important data into the database
	@return void

	@params Orientation *o, Ultrasonic *ultrasonic
*/
void SQLite::startSQL(Orientation *o, Ultrasonic *ultrasonic) {
	/*char *zErrMsg = 0;
	int rc;
	char *sql;
	const char* data = "Callback function called";*/

	PID *pid = PID::getInstanceCreated();
	TCPServer *tcp = TCPServer::getInstance();
	ofstream altLoggingFile;
	altLoggingFile.open("Logger.csv");
	altLoggingFile << "Time;CurBaroVal;UltrasonicDistance;ESC1;ESC2;ESC3;ESC4;Throttle;Pitch;Roll;Yaw;OuputHeightPID;OutputPitchPID;OutputRollPID;OutputYawPID;ErrorPitch;ErrorRoll;ErrorYaw" << std::endl;
	altLoggingFile.flush();
	int64_t startTime = std::chrono::system_clock::now().time_since_epoch().count();

	run = true;
	while (run == true) {
		int64_t curTime = std::chrono::system_clock::now().time_since_epoch().count() - startTime;
		double* arAlt = pid->getAltVals();
		if (arAlt != NULL) {
			altLoggingFile << to_string(curTime) << ";" << arAlt[0] << ";" << arAlt[1] << ";" << arAlt[2] << ";" << arAlt[3] << ";" << arAlt[4] << ";" << arAlt[5] << ";" << arAlt[6] << ";" << arAlt[7] << ";" << arAlt[8] << ";" << arAlt[9] << ";" << arAlt[10] << ";" << arAlt[11] << ";" << arAlt[12] << ";" << arAlt[13] << ";" << arAlt[14] << ";" << arAlt[15] << ";" << arAlt[16] << std::endl;
			altLoggingFile.flush();
		}
		/*double *ar = o->getPitchRollReal();
		string sql;
		if (ar[0] != NULL) {
			int m = millis();

			//Create SQL statement
			sql = "INSERT INTO Orientation (timestamp,droneid,pitch,roll,yaw, heightUS) VALUES (" + to_string(m) + ",1," + to_string(ar[0]) + "," + to_string(ar[1]) + "," + to_string(ar[2]) + "," + to_string(ultrasonic->getDistance()) + ");";

			//Execute SQL statement
			rc = sqlite3_exec(db, sql.c_str(), callback, (void*)data, &zErrMsg);

			if (rc != SQLITE_OK) {
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
			}

			int *arThrottle = pid->getThrottles();
			//Create SQL statement
			sql = "INSERT INTO Motors (timestamp,droneid,MotorSpeed1, MotorSpeed2, MotorSpeed3, Motorspeed4, throttle) VALUES (" + to_string(m) + ",1," + to_string(arThrottle[0]) + "," + to_string(arThrottle[1]) + "," + to_string(arThrottle[2]) + "," + to_string(arThrottle[3]) + "," + to_string(arThrottle[4]) + ");";

			// Execute SQL statement
			rc = sqlite3_exec(db, sql.c_str(), callback, (void*)data, &zErrMsg);

			if (rc != SQLITE_OK) {				fprintf(stderr, "SQL error: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
			}

			float *arPID = pid->getPIDVals();
			//Create SQL statement
			sql = "INSERT INTO PID_Values (timestamp, droneid, PIDPitch, PIDRoll, PIDYaw) VALUES (" + to_string(m) + ",1,'" + to_string(arPID[0]) + "','" + to_string(arPID[1]) + "','" + to_string(arPID[2]) + "');";

			//Execute SQL statement
			rc = sqlite3_exec(db, sql.c_str(), callback, (void*)data, &zErrMsg);

			if (rc != SQLITE_OK) {
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
			}

			if (tcp->list1.size() > 0)
			{
				Command *c = tcp->list1.front();
				
				sql = "INSERT INTO Commands (timestamp, droneid, command) VALUES (" + to_string(c->timestamp) + ",1,'" + c->command + "');";

				// Execute SQL statement
				rc = sqlite3_exec(db, sql.c_str(), callback, (void*)data, &zErrMsg);
				if (rc != SQLITE_OK) {
					fprintf(stderr, "SQL error: %s\n", zErrMsg);
					sqlite3_free(zErrMsg);
				}

				tcp->list1.pop_front();
			}
		}*/
		delay(5);
	}
	//sqlite3_close(db);
}

/**
	This function opens the SQLite3-Database 
	@return bool

	@params char* name
*/
bool SQLite::initSQL(char* name)
{
	/*char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open("/home/pi/projects/opendrone", &db);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	}
	else {
		fprintf(stderr, "Opened database successfully\n");
	}*/

	return true;
}

/**
	This function interrupts the logging
	@return bool
*/
void SQLite::interruptSQL() {
	run = false;
}
