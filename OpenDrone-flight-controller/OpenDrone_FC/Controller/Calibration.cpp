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

bool Calibration::calibrate() {
	cout << "Starting Calibration\n";
	int length = 500;

	double pitch = 0.0, roll = 0.0;
	for (int i = 0; i <= length; i++) {
		double* ar = orientation->getPitchRoll();
		pitch += ar[0];
		roll += ar[0];
	}
	pitch = pitch / length;
	roll = roll / length;

	double ar[2];
	ar[0] = pitch;
	ar[1] = roll;
	return writeToXML(ar);
}

bool Calibration::writeToXML(double *values)
{	
	//TODO: Write to XML
	cout << "Calibration: Pitch: " << values[0] << " Roll: " << values[1] << "\n\n";
	return true;
}

Calibration::~Calibration()
{
}
