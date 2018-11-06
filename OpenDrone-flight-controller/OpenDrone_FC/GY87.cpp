#include "GY87.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <iostream>
#include "UltraSonicSensor.h"

using namespace std;

int addressAccel = 0x68;
int addressBarometer = 0x77;

int fd;

GY87::GY87()
{
	fd = wiringPiI2CSetup(addressAccel);
	if (fd < 1) {
		cout << "wiringPiI2CSetup(addressAccel)\n";
		exit(1);
	}
	wiringPiI2CWriteReg16(fd, 0x6b, 0);
}

void GY87::getGyroValues(double *ar) 
{
	// °/s  ?? rad/s ??
	ar[0] = wiringPiI2CReadReg16(fd, 0x43) / 131; //Gyro X
	ar[1] = wiringPiI2CReadReg16(fd, 0x45) / 131; //Gyro Y
	ar[2] = wiringPiI2CReadReg16(fd, 0x47) / 131; //Gyro Z
}

void GY87::getAccValues(double *ar) 
{
	//m/s^2 ??
	ar[0] = wiringPiI2CReadReg16(fd, 0x3b) / 16384.0; //Acc. X
    ar[1] = wiringPiI2CReadReg16(fd, 0x3d) / 16384.0; //Acc. Y
	ar[2] = wiringPiI2CReadReg16(fd, 0x3f) / 16384.0; //Acc. Z
}

GY87::~GY87()
{
}