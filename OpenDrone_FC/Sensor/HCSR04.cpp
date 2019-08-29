/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: TODO
 *
 * 	@author Tim Klecka
 * 	@version 0.0.1 07.01.2019
 */
#include "HCSR04.h"
#include "../Filter/Filter.h"
#include "./AbstractSensor/Ultrasonic.h"
#include <wiringPi.h>
#include "stdio.h"
#include <iostream>
using namespace std;

HCSR04::HCSR04(int pin_trigger, int pin_echo, int id)
{
	this->pin_trigger = pin_trigger;
	this->pin_echo = pin_echo;
	this->id = id;

	//Initalize the filter
	this->filter = new Filter(3.0,400.0,10.0);

	//Defines the pins
	pinMode(pin_trigger, OUTPUT);
	pinMode(pin_echo, INPUT);
	digitalWrite(pin_trigger, LOW);
} 

void HCSR04::calcDistance()
{
	long ping = 0;
	long pong = 0;
	double distance = 0;
	unsigned int timeout = 25000;

	digitalWrite(pin_trigger, LOW);
	delayMicroseconds(2);
	//Trigger the ping.
	digitalWrite(pin_trigger, HIGH);
	delayMicroseconds(10);
	digitalWrite(pin_trigger, LOW);
	unsigned int start = micros();

	while(digitalRead(pin_echo) == LOW && micros()-start < timeout) {}
	ping = micros();
	while (digitalRead(pin_echo) == HIGH && micros()-start < timeout) {}
	pong = micros();

	// Convert ping duration to distance.
	distance = ((pong - ping)/2000000.0)*341.29*100;

	this->curDistance = this->filter->addValue(distance);
	//cout << distance << endl;
}

double HCSR04::getDistance() 
{
	return this->curDistance;
}

void HCSR04::runUltrasonic()
{
	this->run = true;
	while (this->run)
	{
		this->calcDistance();
		delay(15);
	}
}

void HCSR04::interruptUltrasonic()
{
	this->run = false;
}

HCSR04::~HCSR04()
{
}
