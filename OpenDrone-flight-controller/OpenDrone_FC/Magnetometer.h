#pragma once
class Magnetometer
{
public:
	int fd;

	Magnetometer();
	float *getMagnetometerValues();
	short readRawData(int addr);
	~Magnetometer();
};

