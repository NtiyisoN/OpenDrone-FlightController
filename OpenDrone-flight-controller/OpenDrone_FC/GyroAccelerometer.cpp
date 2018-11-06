#include "GyroAccelerometer.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <iostream>

using namespace std;

int addressAccel = 0x68;
int fdGyroAcc;

GyroAccelerometer::GyroAccelerometer()
{
	this->fdGyroAcc = wiringPiI2CSetup(addressAccel);
	if (fdGyroAcc < 1) {
		cout << "wiringPiI2CSetup(addressAccel)\n";
		exit(1);
	}
	wiringPiI2CWriteReg16(fdGyroAcc, 0x6b, 0);
}

double* GyroAccelerometer::getGyroValues()
{
	// °/s  ?? rad/s ??
	double ar[3];
	ar[0] = wiringPiI2CReadReg16(fdGyroAcc, 0x43) / 131; //Gyro X
	ar[1] = wiringPiI2CReadReg16(fdGyroAcc, 0x45) / 131; //Gyro Y
	ar[2] = wiringPiI2CReadReg16(fdGyroAcc, 0x47) / 131; //Gyro Z
	return ar;
}

double* GyroAccelerometer::getAccValues()
{
	//m/s^2 ??
	double ar[3];
	ar[0] = wiringPiI2CReadReg16(fdGyroAcc, 0x3b) / 16384.0; //Acc. X
    ar[1] = wiringPiI2CReadReg16(fdGyroAcc, 0x3d) / 16384.0; //Acc. Y
	ar[2] = wiringPiI2CReadReg16(fdGyroAcc, 0x3f) / 16384.0; //Acc. Z
	return ar;
}

GyroAccelerometer::~GyroAccelerometer()
{
}