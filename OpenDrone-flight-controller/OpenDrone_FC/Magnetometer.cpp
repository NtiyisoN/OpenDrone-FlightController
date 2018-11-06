#include "Magnetometer.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <iostream>

using namespace std;

int addressMag = 0x1E;
int fdMagnet;

Magnetometer::Magnetometer()
{
	this->fdMagnet = wiringPiI2CSetup(addressMag);
	if (fdMagnet < 1) {
		cout << "wiringPiI2CSetup(addressMagnetometer)\n";
		exit(1);
	}
	wiringPiI2CWriteReg8(fdMagnet, 0x02, 0x00);
	wiringPiI2CWrite(fdMagnet, 0x03);
}

double *Magnetometer::getMagnetometerValues()
{
	double ar[3];
	ar[0] = wiringPiI2CReadReg16(fdMagnet, 6 + 8); //Magnet X
	ar[1] = wiringPiI2CReadReg16(fdMagnet, 6 + 8 + 8) / 131; //Magnet Y
	ar[2] = wiringPiI2CReadReg16(fdMagnet, 6 + 8 + 8 + 8) / 131; //Magnet Z
	return ar;
}


Magnetometer::~Magnetometer()
{
}
