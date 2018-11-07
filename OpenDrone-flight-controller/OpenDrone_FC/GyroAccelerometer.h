#pragma once
class GyroAccelerometer
{
public:
	int fd;

	GyroAccelerometer();
	float *getValues(void);
	short readRawData(int addr);
	~GyroAccelerometer();
};

