#include "BMF055.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <iostream>
using namespace std;

BMF055::BMF055()
{
}

double *BMF055::getAccelerometerValues() {
	return NULL;
}

double *BMF055::getGyroscopeValues() {
	return NULL;
}

double *BMF055::getMagnetometerValues() {
	return NULL;
}

BMF055::~BMF055()
{
}
