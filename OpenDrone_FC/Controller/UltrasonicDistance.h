#pragma once
#include "../Sensor/AbstractSensor/Ultrasonic.h"
#include "../Sensor/HCSR04.h"
#include <wiringPi.h>
#include <list> 
#include <iostream>
using namespace std;

class UltrasonicDistance
{
public:
	UltrasonicDistance();
	~UltrasonicDistance();
	list<double> getDistance();
	void runUltrasonic();
	void interruptUltrasonic();

private:
	//list<Ultrasonic> ultrasonic;
	bool run = false;
	void calcDistance();
	void createObjects();
};

