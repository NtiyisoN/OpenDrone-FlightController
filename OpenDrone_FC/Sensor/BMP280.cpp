/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: TODO
 *
 * 	@author Tim Klecka
 * 	@version 0.0.1 07.01.2019
 */
#include "BMP280.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <iostream>
using namespace std;

#define DEVICE_ADDRESS 0x76
#define REG_TEMPERATURE 0xFA
#define REG_PRESSURE 0xF7

BMP280::BMP280()
{
	this->fd = wiringPiI2CSetup(DEVICE_ADDRESS);
	if (this->fd < 1) {
		cout << "wiringPiI2CSetup(addressBarometer)\n";
		exit(1);
	}
}

int BMP280::readRawData(int addr)
{
	int high_byte, middle_byte, low_byte, value;

	low_byte = wiringPiI2CReadReg8(fd, addr);
	middle_byte = wiringPiI2CReadReg8(fd, addr + 1);
	value = (middle_byte << 8) | low_byte;
	high_byte = wiringPiI2CReadReg8(fd, addr + 2);
	value = (high_byte << 16) | value;
	value = value << 4;
	return value;
}

void BMP280::calcBaromter()
{
	this->temperature = readRawData(REG_TEMPERATURE);	//Temperature
	this->pressure = readRawData(REG_PRESSURE);		//Pressure
}

double *BMP280::getBarometerValues()
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

void BMP280::runBarometer()
{
	this->run = true;
	while (this->run)
	{
		this->calcBaromter();
	}
}

void BMP280::interruptBaromter()
{
	this->run = false;
}

BMP280::~BMP280()
{
}
