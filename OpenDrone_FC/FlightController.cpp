/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: TODO
 *
 * 	@author Thomas Brych, Tim Klecka
 * 	@version 0.0.1 14.02.2019
 */
#include "FlightController.h"
#include "Sensor/AbstractSensor/Barometer.h"
#include "Sensor/AbstractSensor/Ultrasonic.h"
#include "Sensor/BMP388.h"
#include "Sensor/BMP280.h"
#include "Sensor/BMP180.h"
#include "Sensor/BNO080.h"

#include "Network/TCPServer.h"

#include "Controller/Calibration.h"
#include "Controller/Orientation.h"
#include "Controller/UltrasonicDistance.h"
#include "Controller/Exit.h"
#include "Controller/PID.h"

#include "Motor/PWMMotorTest.h"

#include "Database/SQLite.h"

#include "Motor/PWMMotorTest.h"

#include <wiringPi.h>
#include <iostream>
#include <thread>
#include <fstream>
#include <signal.h>

using namespace std;

Orientation *orientation;
Barometer *barometer;
//UltrasonicDistance *ultrasonic;
HCSR04 *ultrasonic;
TCPServer *server;
Exit *error;
PID *pid;
PWMMotorTest *pwm;
SQLite *sql;
int arg;

FlightController::FlightController(int argIn)
{
	arg = argIn;
}


static void runUltrasonic()
{
	while (true) {
		ultrasonic->calcDistance();
	}
}

static void runBarometer()
{
	barometer->runBarometer();
}

static void runOrientation()
{
	orientation->runOrientation();
}

static void runServer()
{
	server->startUp();
}

static void runPid() {
	pid->calcValues();
}

static void runSQL()
{
	sql->initSQL("opendrone");
	sql->startSQL(orientation, ultrasonic);
}

void sighandler(int sig)
{
	cout << "Signal " << sig << " caught..." << endl;

	//Interrupt Threads
	orientation->interruptOrientation();
	pid->interruptPid();
	cout << "Interrupting Threads! \n";

	pwm->ExitMotor();
	cout << "Exitmotors called";
	getchar();
	pwm->ExitMotor();

	system("sudo /home/pi/projects/getValues.sh");

	exit(1);
}

void FlightController::initObjects() 
{
	error = Exit::getInstance();

	int rc = wiringPiSetupGpio();
	if (rc != 0)
	{
		//The GPIO-Setup did not work
		error->sendError(0x01, true);
		return;
	}
  
	orientation = new Orientation();
	barometer = new BMP280();
	pwm = new PWMMotorTest();
	//ultrasonic = new UltrasonicDistance();
	//parser = new XMLParser();
	ultrasonic = new HCSR04(5,6,0);
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
	thread serverThread(runServer);
	while (!server->connected) { delay(50); };
	cout << "Client connected!\n";

	//Initialize all important objects
	initObjects();

	signal(SIGINT, &sighandler);

	delay(250);

	//Arm Motors
	/*cout << "Hallo";
	getchar();
	pwm->ExitMotor();
	pwm->ArmMotor();
	getchar();
	pwm->ExitMotor();

	//Calibrate
	/*cout << "Hallo";
	getchar();
	pwm->ExitMotor();
	pwm->CalMotor();
	cout << "Tim";
	getchar();
	pwm->ExitMotor();*/

	//Check Motors
	/*int rc = wiringPiSetupGpio();
	pwm = new PWMMotorTest();
	pwm->ExitMotor();
	pwm->ArmMotor();
	cout << "0";
	pwm->SetSpeed(0, 1200);
	getchar();
	cout << "1";
	pwm->SetSpeed(0, 0);
	pwm->SetSpeed(1, 1200);
	getchar();
	cout << "2";
	pwm->SetSpeed(1, 0);
	pwm->SetSpeed(2, 1200);
	getchar();
	cout << "3";
	pwm->SetSpeed(2, 0);
	pwm->SetSpeed(3, 1200);
	getchar();
	pwm->SetSpeed(3, 0);*/

	//Run Threads
	thread pitchRollYawThread(runOrientation);
	thread barometerThread(runBarometer);
	thread pidController(runPid);
	thread sqlThread(runSQL);
	thread ultrasonicThread(runUltrasonic);
	cout << "Threads are running!\n";

	//Interrupt Threads
	//orientation->interruptOrientation();
	//barometer->interruptBaromter();
	//pid->interruptPid();
	//sql->interruptSQL();
	//cout << "Interrupting Threads! \n";

	//Wait until threads stopped
	serverThread.join();
	pitchRollYawThread.join();
	barometerThread.join();
	pidController.join();
	sqlThread.join();
	cout << "Stopped Threads!\n";

	return (0);
}

FlightController::~FlightController()
{
}
