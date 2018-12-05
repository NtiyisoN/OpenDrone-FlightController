/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 */

#include "HCSR04.h"
#include "../Filter/Filter.h"
#include "./AbstractSensor/Ultrasonic.h"
#include <wiringPi.h>

HCSR04::HCSR04(int pin_trigger, int pin_echo, int id)
{
	this->pin_trigger = pin_trigger;
	this->pin_echo = pin_echo;
	this->id = id;
	//Initalize the 
	this->filter = new Filter(3.0,400.0,16.0);
	//Defines the pins
	pinMode(pin_trigger, OUTPUT);
	pinMode(pin_echo, INPUT);
	digitalWrite(pin_trigger, LOW);
	delay(50); 
} 

double HCSR04::distance()
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

	return this->filter->addValue(distance);
}


int HCSR04::getId() {
	return this->id;
}

HCSR04::~HCSR04()
{
}