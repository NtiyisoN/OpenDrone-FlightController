#pragma once
class GyroAccelerometer
{
public:
	GyroAccelerometer();
	void getGyroValues(double* ar);
	void getAccValues(double* ar);
	~GyroAccelerometer();
private:
	int fdGyroAcc;
	int addressAccel;
};

