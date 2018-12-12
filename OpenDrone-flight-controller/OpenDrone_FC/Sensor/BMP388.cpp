/*
* Copyright (c) OpenDrone, 2018.  All rights reserved.
* The entire project (including this file) is licensed under the GNU GPL v3.0
*/

#include "BMP388.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <iostream>
using namespace std;

#define DEVICE_ADDRESS 0x77
#define OSR 0x1C
#define PWR_CTRL 0x1B
#define CONFIG 0x1F

BMP388::BMP388()
{
	this->fd = wiringPiI2CSetup(DEVICE_ADDRESS);
	if (this->fd < 1) {
		cout << "wiringPiI2CSetup(addressBarometer)\n";
		exit(1);
	}

	wiringPiI2CWriteReg8(this->fd, OSR, 0x03);		//Pressure = high resolution, temperature = 1x
	wiringPiI2CWriteReg8(this->fd, PWR_CTRL, 0x30); //Power-mode normal
	wiringPiI2CWriteReg8(this->fd, CONFIG, 0x04);	//Filter coefficient = 3
}

int BMP388::readRawData(int addr)
{
	int high_byte, middle_byte, low_byte, value;

	low_byte = wiringPiI2CReadReg8(fd, addr);
	middle_byte = wiringPiI2CReadReg8(fd, addr + 1);
	high_byte = wiringPiI2CReadReg8(fd, addr + 2);
	value = (high_byte << 16) | (middle_byte << 8) | low_byte;

	return value;
}

int *BMP388::getBarometerValues()
{
	static int ar[2];
	ar[0] = readRawData(0x07);	//Temperature
	ar[1] = readRawData(0x04);	//Pressure
	return ar;
}

BMP388::~BMP388()
{
}
