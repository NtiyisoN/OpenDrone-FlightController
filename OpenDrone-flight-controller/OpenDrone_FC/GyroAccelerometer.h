#pragma once
class GyroAccelerometer
{
public:
	GyroAccelerometer();
	double* getGyroValues();
	double* getAccValues();
	~GyroAccelerometer();
private:
	int fdGyroAcc;
	int addressAccel;
};

