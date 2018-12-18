#include "../Sensor/AbstractSensor/Gyroscope.h"
#include "../Sensor/AbstractSensor/Accelerometer.h"
#include "../Sensor/AbstractSensor/Magnetometer.h"

#pragma once
class Calibration
{
public:
	Calibration(Gyroscope* g, Accelerometer* a, Magnetometer* m);
	~Calibration();
	bool calibrate();
private:
	Gyroscope* gyro;
	Accelerometer* acc;
	Magnetometer* magnet;
	bool writeToXML(double *values);
};

