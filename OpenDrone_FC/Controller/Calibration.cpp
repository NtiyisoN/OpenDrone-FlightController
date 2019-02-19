#include "Calibration.h"
#include "../Sensor/MPU6050.h"
#include "../Sensor/HMC5883L.h"
#include <iostream>
#include <wiringPi.h>
#include <math.h>
using namespace std;

Calibration::Calibration(Orientation *o)
{
	this->orientation = o;
}

double *Calibration::calibrate() {
	cout << "Starting Calibration\n";
	int length = 500;

	double pitch = 0.0, roll = 0.0, yaw = 0.0;
	for (int i = 0; i <= length; i++) {
		double* ar = orientation->getPitchRollReal();
		pitch += ar[0];
		roll += ar[1];
		yaw += ar[2];
		delay(3);
	}
	pitch = pitch / length;
	roll = roll / length;
	yaw = yaw / length;

	double ar[2];
	ar[0] = pitch;
	ar[1] = roll;
	ar[2] = yaw;
	return ar;
}

Calibration::~Calibration()
{
}
