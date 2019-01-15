#include "UltrasonicDistance.h"

UltrasonicDistance::UltrasonicDistance()
{
	createObjects();
}

void UltrasonicDistance::createObjects()
{
	//TD Read the data from the XML and fill the list
}

void UltrasonicDistance::calcDistance()
{
	//TD Iterate through list<ultrasonic> and calculate the values for the ultrasonic
}

void UltrasonicDistance::runUltrasonic()
{
	this->run = true;
	while (this->run)
	{
		this->calcDistance();
	}
}

void UltrasonicDistance::interruptUltrasonic()
{
	this->run = false;
}

list<double> UltrasonicDistance::getDistance()
{
	list<double> values;

	//TD iterate through list<ultrasonic>, get the values and return them

	return values;
}

UltrasonicDistance::~UltrasonicDistance()
{
}
