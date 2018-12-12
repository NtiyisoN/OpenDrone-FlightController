#pragma once
#include "AbstractSensor/Magnetometer.h"
#include "AbstractSensor/Accelerometer.h"
#include "AbstractSensor/Gyroscope.h"

class BMF055 : 
	public virtual Magnetometer,
	public virtual Accelerometer,
	public virtual Gyroscope
{
public:
	BMF055();
	~BMF055();

	double *getAccelerometerValues();
	double *getGyroscopeValues();
	double *getMagnetometerValues();
};

