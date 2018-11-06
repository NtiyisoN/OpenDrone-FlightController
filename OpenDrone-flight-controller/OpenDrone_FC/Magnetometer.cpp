#include "Magnetometer.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <iostream>

using namespace std;

int addressMag = 0x1E;
int fd;

Magnetometer::Magnetometer()
{
	fd = wiringPiI2CSetup(addressMag);
	if (fd < 1) {
		cout << "wiringPiI2CSetup(addressMagnetometer)\n";
		exit(1);
	}
	wiringPiI2CWriteReg8(fd, 0x02, 0x00);
	wiringPiI2CWrite(fd, 0x03);
}

void Magnetometer::getMagnetometerValues(double *ar)
{
	ar[0] = wiringPiI2CReadReg16(fd, 6 + 8); //Gyro X
	ar[1] = wiringPiI2CReadReg16(fd, 6 + 8 + 8) / 131; //Gyro Y
	ar[2] = wiringPiI2CReadReg16(fd, 6 + 8 + 8 + 8) / 131; //Gyro Z
}


Magnetometer::~Magnetometer()
{
}
