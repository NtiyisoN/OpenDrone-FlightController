#pragma once
#include "../Sensor/AbstractSensor/Gyroscope.h"
#include "../Sensor/AbstractSensor/Accelerometer.h"
#include "../Sensor/MPU6050.h"
#include "../Sensor/BNO080.h"

class Orientation
{
public:
	Orientation();
	~Orientation();
	double *getPitchRoll();
	double *getPitchRollReal();
	void runOrientation();
	void interruptOrientation();
	void setCalibration(double *ar);

private:
	//Gyroscope *gyro; 
	//Accelerometer *acc;
	BNO080 *bno;
	//double pitch = 0.0, roll = 0.0;
	bool run = false;

	double calPitch = 0.0, calRoll = 0.0, calYaw = 0.0;
	//void calcPitchRoll();
	//double getXRotation(double accX, double accY, double accZ);
	//double getYRotation(double accX, double accY, double accZ);
};

