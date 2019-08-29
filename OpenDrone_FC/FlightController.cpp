/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: The class, that initializes all objects and starts all the necessary threads
 *
 * 	@author Thomas Brych, Tim Klecka
 * 	@version 0.0.2 26.06.2019
 */
#include "FlightController.h"
#include "Sensor/AbstractSensor/Barometer.h"
#include "Sensor/AbstractSensor/Ultrasonic.h"
#include "Sensor/BMP280.h"
#include "Sensor/BNO080.h"
#include "Sensor/HCSR04.h"
#include "Sensor/GYUS42.h"

#include "Network/TCPServer.h"
#include "Motor/PWMMotorTest.h"
#include "Database/SQLite.h"

#include "Controller/Calibration.h"
#include "Controller/Orientation.h"
#include "Controller/Exit.h"
#include "Controller/PID.h"

#include <wiringPi.h>
#include <iostream>
#include <thread>
#include <fstream>
#include <signal.h>
using namespace std;

FlightController::FlightController(int argIn)
{
	arg = argIn;
}

FlightController::~FlightController()
{
}

/**
	Method to run the Ultrasonic-Thread
	@return void

	@params Ultrasonic *ultrasonic
*/
static void runUltrasonic(Ultrasonic *ultrasonic)
{
	ultrasonic->runUltrasonic();
}

/**
	Method to run the Barometer-Thread
	@return void

	@params Barometer *barometer
*/
static void runBarometer(Barometer *barometer)
{
	barometer->runBarometer();
}

/**
	Method to run the Orientation-Thread
	@return void

	@params Orientation *orientation
*/
static void runOrientation(Orientation *orientation)
{
	orientation->runOrientation();
}

/**
	Method to run the Server-Thread
	@return void

	@params TCPServer *server
*/
static void runServer(TCPServer *server)
{
	server->startUp();
}

/**
	Method to run the PID-Thread
	@return void

	@params PID *pid
*/
static void runPid(PID *pid) {
	pid->calcValues();
}

/**
	Method to run the SQL-Thread
	@return void

	@params SQLite *sql, Orientation *orientation, Ultrasonic *ultrasonic
*/
static void runSQL(SQLite *sql, Orientation *orientation, Ultrasonic *ultrasonic)
{
	sql->initSQL("opendrone");
	sql->startSQL(orientation, ultrasonic);
}

/**
	Method that gets called after the user presses Ctrl+C
	@return void

	@params int sig
*/
void sighandler(int sig)
{
	cout << "Signal " << sig << " caught..." << endl;
	
	//Interrupt PID-Thread
	PID *pid = PID::getInstanceCreated();
	pid->interruptPid();
	
	//Exits the motors (stop the motors immediately)
	PWMMotorTest *pwm = pid->getPwmMotorTest();
	cout << "Exitmotors called";
	pwm->ExitMotor();
	delay(250);
	pwm->ExitMotor();

	exit(0);
}

/**
	Method thats called to initialize all our objects
	@return void
*/
void FlightController::initObjects() 
{
	error = Exit::getInstance();

	//WiringPi GPIO-Setup
	int rc = wiringPiSetupGpio();
	if (rc != 0)
	{
		//The GPIO-Setup did not work
		cout << "Error initializing the GPIO-Setup!\n";
		error->sendError(0x01, true);
		return;
	}
  
	//Init the important objects
	orientation = new Orientation();
	barometer = new BMP280();
	pwm = new PWMMotorTest();
	ultrasonic = new GYUS42();
	sql = new SQLite();
	pid = PID::getInstance(orientation, pwm, barometer, ultrasonic);
}

/**
	Default method that's called to start the Flight 
	@return int

	@info
	* Starts all the threads
*/
int FlightController::run()
{
	//Start server
	server = TCPServer::getInstance();
	thread serverThread(runServer, server);
	while (!server->connected) { delay(50); };
	cout << "Client connected!\n";

	//Initialize all important objects
	initObjects();

	//Check if user pressed Ctrl+C to interrupt the pid and stop the motors
	signal(SIGINT, &sighandler);

	delay(250);

	//Run Threads
	thread pitchRollYawThread(runOrientation, orientation);
	thread barometerThread(runBarometer, barometer);
	thread pidController(runPid, pid);
	thread sqlThread(runSQL, sql, orientation, ultrasonic);
	thread ultrasonicThread(runUltrasonic, ultrasonic);
	cout << "Threads are running!\n";

	//Interrupt the Threads
	/*orientation->interruptOrientation();
	barometer->interruptBaromter();
	pid->interruptPid();
	sql->interruptSQL();
	cout << "Interrupting Threads! \n";*/

	//Wait until threads stopped
	serverThread.join();
	pitchRollYawThread.join();
	barometerThread.join();
	pidController.join();
	sqlThread.join();
	cout << "Stopped Threads!\n";

	return (0);
}
