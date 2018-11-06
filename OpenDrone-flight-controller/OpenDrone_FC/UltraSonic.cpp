#include "UltraSonic.h"
#include "Filter.h"
#include <wiringPi.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
using namespace std;

int pin_trigger;
int pin_echo;
int id;
Filter *filter;

//TODO: output 1 -> no sensor

UltraSonic::UltraSonic()
{
	//Empty Constructor -- Necessary
}

UltraSonic::UltraSonic(int pin_trigger, int pin_echo, int id)
{
	this->pin_trigger = pin_trigger;
	this->pin_echo = pin_echo;
	this->id = id;

	cout << "Starting HC-SR04 with the id " << id << "\n";

	//Initalize the 
	filter = new Filter(400.0,3.0,16.0);
	//Defines the pins
	pinMode(pin_trigger, OUTPUT);
	pinMode(pin_echo, INPUT);
	digitalWrite(pin_trigger, LOW);
	delay(50); 
} 

UltraSonic::~UltraSonic()
{
}

double UltraSonic::distance()
{
	long ping = 0;
	long pong = 0;
	double distance = 0;
	long timeout = 25000;

	digitalWrite(pin_trigger, LOW);
	delayMicroseconds(2);
	//Trigger the ping.
	digitalWrite(pin_trigger, HIGH);
	delayMicroseconds(10);
	digitalWrite(pin_trigger, LOW);
	long start = micros();

	while(digitalRead(pin_echo) == LOW && micros()-start < timeout) {}
	ping = micros();
	while (digitalRead(pin_echo) == HIGH && micros()-start < timeout) {}
	pong = micros();

	// Convert ping duration to distance.
	distance = ((pong - ping)/2000000.0)*341.29*100;

	return filter->addValue(distance);
}


int UltraSonic::getId() {
	return this->id;
}