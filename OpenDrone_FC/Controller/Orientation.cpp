#include "Orientation.h"
#include <iostream>
#include <wiringPi.h>
#include <math.h>
using namespace std;

Orientation::Orientation()
{
	//this->gyro = new MPU6050();
	//this->acc = new MPU6050();
	this->bno = new BNO080();
}

/*void Orientation::calcPitchRoll()
{
	double K = 0.98;
	double K1 = 1 - K;
	double *valuesGyro = this->gyro->getGyroscopeValues();
	double *valuesAcc = this->acc->getAccelerometerValues();
	double combined[6];
	combined[0] = valuesGyro[1];
	combined[1] = valuesGyro[2];
	combined[2] = valuesGyro[3];
	combined[3] = valuesAcc[1];
	combined[4] = valuesAcc[2];
	combined[5] = valuesAcc[3];

	//TODO: read from XML
	//values[0] -= gyro_offset_x;
	//values[1] -= gyro_offset_y;

	double gyro_x_delta = (combined[0] * 0.01);
	double gyro_y_delta = (combined[1] * 0.01);

	double rotation_x = getXRotation(combined[3], combined[4], combined[5]);
	double rotation_y = getYRotation(combined[3], combined[4], combined[5]);

	this->roll = (K * (gyro_x_delta)+(K1 * rotation_x)) * 50;
	this->pitch = (K * (gyro_y_delta)+(K1 * rotation_y)) * 50;
}

double Orientation::getXRotation(double accX, double accY, double accZ)
{
	return (atan2(accX, sqrt((accY * accY) + (accZ * accZ))))*(180 / M_PI);
}

double Orientation::getYRotation(double accX, double accY, double accZ)
{
	return (atan2(accY, sqrt((accX * accX) + (accZ * accZ))))*(180 / M_PI);
}*/

void Orientation::runOrientation()
{
	this->run = true;
	this->bno->runBNO();
}

void Orientation::interruptOrientation()
{
	this->run = false;
}

double *Orientation::getPitchRoll()
{
	static double ar[3];
	if (this->run)
	{
		ar[0] = this->bno->gyroIntegratedRotationVectorData.lastPitch;
		ar[1] = this->bno->gyroIntegratedRotationVectorData.lastRoll;
		ar[2] = this->bno->gyroIntegratedRotationVectorData.lastYaw;
	}
	else
	{
		ar[0] = NULL;
		ar[1] = NULL;
		ar[2] = NULL;
	}

	return ar;
}

Orientation::~Orientation()
{
}
