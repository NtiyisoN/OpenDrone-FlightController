#pragma once
class GyroAccelerometer
{
public:
	int fd;

	float *getValues(void);
	short readRawData(int addr);
};

