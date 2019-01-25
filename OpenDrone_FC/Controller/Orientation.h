#pragma once
#include "../Sensor/AbstractSensor/Gyroscope.h"
#include "../Sensor/AbstractSensor/Accelerometer.h"
#include "../Sensor/MPU6050.h"

class Orientation
{
public:
	Orientation();
	~Orientation();
	double *getPitchRoll();
	void runOrientation();
	void interruptOrientation();

private:
	Gyroscope *gyro;
	Accelerometer *acc;
	double pitch = 0.0, roll = 0.0;
	bool run = false;

	void calcPitchRoll();
	double getXRotation(double accX, double accY, double accZ);
	double getYRotation(double accX, double accY, double accZ);
};

