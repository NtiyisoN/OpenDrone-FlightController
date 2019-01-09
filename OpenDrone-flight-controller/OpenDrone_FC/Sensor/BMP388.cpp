/*
* Copyright (c) OpenDrone, 2018.  All rights reserved.
* The entire project (including this file) is licensed under the GNU GPL v3.0
*/

#include "BMP388.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <iostream>
using namespace std;

#define DEVICE_ADDRESS 0x76
#define PWR_CTRL 0x1B
#define OSR 0x1C
#define ODR 0x1D
#define CONFIG 0x1F

BMP388::BMP388()
{
	this->fd = wiringPiI2CSetup(DEVICE_ADDRESS);
	if (this->fd < 1) {
		cout << "wiringPiI2CSetup(addressBarometer)\n";
		exit(1);
	}
	wiringPiI2CWriteReg8(this->fd, PWR_CTRL, 0x33); //Power-mode normal
	wiringPiI2CWriteReg8(this->fd, OSR, 0x03);		//Pressure = high resolution, temperature = 1x
	wiringPiI2CWriteReg8(this->fd, CONFIG, 0x00);	//Filter coefficient = 3
	wiringPiI2CWriteReg8(this->fd, ODR, 0x00); //Sampling-Rate
	delay(2000);

	cout << wiringPiI2CReadReg8(this->fd, 0x02);
}

int BMP388::readRawData(int addr)
{
	int high_byte, middle_byte, low_byte, value;

	low_byte = wiringPiI2CReadReg8(fd, addr);
	middle_byte = wiringPiI2CReadReg8(fd, addr + 1);
	value = (middle_byte << 8) | low_byte;
	high_byte = wiringPiI2CReadReg8(fd, addr + 2);
	value = (high_byte << 16) | value;

	return value;
}

void BMP388::calcBaromter()
{
	this->temperature = readRawData(0x07);	//Temperature
	this->pressure = readRawData(0x04);		//Pressure
}

double *BMP388::getBarometerValues()
{
	static double ar[2];
	if (this->run)
	{
		ar[0] = this->temperature;
		ar[1] = this->pressure;
	}
	else
	{
		ar[0] = NULL;
		ar[1] = NULL;
	}
	return ar;
}

void BMP388::runBarometer()
{
	this->run = true;
	while (this->run)
	{
		this->calcBaromter();
	}
}

void BMP388::interruptBaromter()
{
	this->run = false;
}

BMP388::~BMP388()
{
}
