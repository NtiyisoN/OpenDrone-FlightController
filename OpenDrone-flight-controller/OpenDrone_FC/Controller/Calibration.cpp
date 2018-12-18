#include "Calibration.h"
#include "../Sensor/MPU6050.h"
#include "../Sensor/HMC5883L.h"
#include <wiringPi.h>
#include <iostream>
#include <list> 
#include <iterator>
#include <math.h>
using namespace std;

Calibration::Calibration(Gyroscope* g, Accelerometer* a, Magnetometer* m)
{
	this->gyro = g;
	this->acc = a;
	this->magnet = m;
}

bool Calibration::calibrate() {
	cout << "Starting Calibration\n";
	delay(5000);
	
	int length = 500;
	double avgGyroX = 0.0, avgGyroY = 0.0, avgGyroZ = 0.0;
	double avgAccX = 0.0, avgAccY = 0.0, avgAccZ = 0.0;
	double avgMagX = 0.0, avgMagY = 0.0, avgMagZ = 0.0;

	double *acc, *gyro, *magnet;
	for (int i = 0; i <= length; i++) {
		acc = this->acc->getAccelerometerValues();
		gyro = this->gyro->getGyroscopeValues();
		magnet = this->magnet->getMagnetometerValues();

		/*cout << acc[1] << acc[2] << acc[3] << "\n";
		cout << gyro[1] << gyro[2] << gyro[3] << "\n";
		cout << magnet[1] << magnet[2] << magnet[3] << "\n";*/
		avgGyroX += gyro[1];
		avgGyroY += gyro[2];
		avgGyroZ += gyro[3];
		avgAccX += acc[1];
		avgAccY += acc[2];
		avgAccZ += acc[3];
		avgMagX += magnet[1];
		avgMagY += magnet[2];
		avgMagZ += magnet[3];
	}

	double values[9] = {
		(avgGyroX / length),
		(avgGyroY / length),
		(avgGyroZ / length),
		(avgAccX / length),
		(avgAccY / length),
		(avgAccZ / length),
		(avgMagX / length),
		(avgMagY / length),
		(avgMagZ / length),
	};

	return writeToXML(values);
}

bool Calibration::writeToXML(double *values) {
	for (int i = 0; i < 9; i++) {
		cout << values[i] << "\n";
	}
	double pitch = 180 * atan(values[3] / sqrt(values[4] * values[4] + values[5] * values[5])) / M_PI;
	double roll = 180 * atan(values[4] / sqrt(values[3] * values[3] + values[5] * values[5])) / M_PI;
	double yaw = -1;
	cout << "pitch: " << pitch << " roll: " << roll << "yaw: " << yaw << "\n";
	return true;
}

Calibration::~Calibration()
{
}
