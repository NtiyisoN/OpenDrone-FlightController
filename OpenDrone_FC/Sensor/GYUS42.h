#include "./AbstractSensor/Ultrasonic.h"

#pragma once
class GYUS42 : 
	public virtual Ultrasonic
{
public:
	GYUS42();
	int distance;
	int fd;
	bool run;

	void runUltrasonic();
	double getDistance();
	void interruptUltrasonic();
	void readVal();
};

