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

#include "XML/XMLParser.h"

#include "Motor/PWMMotorTest.h"

#include <wiringPi.h>
#include <iostream>
#include <thread>
#include <fstream>
using namespace std;

Orientation *orientation;
Barometer *barometer;
UltrasonicDistance *ultrasonic;
XMLParser *parser;
TCPServer *server;
Exit *error;
PID *pid;
PWMMotorTest *pwm;

FlightController::FlightController()
{
}

static void runUltrasonic()
{
	ultrasonic->runUltrasonic();
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
	pid = PID::getInstance(orientation, pwm);
	pid->calcValues();
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
	//barometer = new BMP280();
	pwm = new PWMMotorTest();

	//ultrasonic = new UltrasonicDistance();
	//parser = new XMLParser();
}

int FlightController::run()
{
	//Start server
	server = TCPServer::getInstance();
	thread serverThread(runServer);
	while (!server->connected) { delay(50); };
	cout << "Client connected!\n";

	//Initialize all important objects
	initObjects();

	delay(250);

	//Arm Motors
	/*cout << "Hallo";
	getchar();
	pwm->ExitMotor();
	pwm->ArmMotor();
	getchar();
	pwm->ExitMotor();*/

	//Calibrate
	/*cout << "Hallo";
	getchar();
	pwm->ExitMotor();
	pwm->CalMotor();
	cout << "Tim";
	getchar();
	pwm->ExitMotor();*/

	//Check Motors
	/*pwm->ExitMotor();
	pwm->ArmMotor();
	cout << "0";
	pwm->SetSpeed(0, 1500);
	getchar();
	cout << "1";
	pwm->SetSpeed(0, 0);
	pwm->SetSpeed(1, 1500);
	getchar();
	cout << "2";
	pwm->SetSpeed(1, 0);
	pwm->SetSpeed(2, 1500);
	getchar();
	cout << "3";
	pwm->SetSpeed(2, 0);
	pwm->SetSpeed(3, 1500);
	getchar();
	pwm->SetSpeed(3, 0);*/

	//Run Threads
	thread pitchRollYawThread(runOrientation);
	//thread barometerThread(runBarometer);
	thread pidController(runPid);
	cout << "Threads are running!\n";

	//Interrupt Threads
	/*orientation->interruptOrientation();
	barometer->interruptBaromter();
	pid->interruptPid();
	cout << "Interrupting Threads! \n";*/

	//Wait until threads stopped
	serverThread.join();
	pitchRollYawThread.join();
	//barometerThread.join();
	pidController.join();
	cout << "Stopped Threads!\n";

	return (0);
}

FlightController::~FlightController()
{
}


