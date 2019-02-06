/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: TODO
 *
 * 	@author Tim Klecka
 * 	@version 0.0.1 07.01.2019
 */
#include "BMP280.h"
#include "../Controller/Exit.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <iostream>
using namespace std;

#define DEVICE_ADDRESS 0x76
#define REG_TEMPERATURE 0xFC
#define REG_PRESSURE 0xF9
#define CTR_MEAS 0xF4
#define RESET 0xE0

BMP280::BMP280()
{
	this->fd = wiringPiI2CSetup(DEVICE_ADDRESS);
	if (this->fd < 1) {
		cout << "wiringPiI2CSetup(addressBarometer)\n";
		exit(1);
	}
	wiringPiI2CWriteReg8(fd, RESET, 0xB6);

	delay(2000);

	wiringPiI2CWriteReg8(fd, CTR_MEAS, 0xFF);
	
	delay(2000);
	//wiringPiI2CWriteReg8(fd, CTR_MEAS, 0x5F);

	int id = wiringPiI2CReadReg8(fd, 0xD0);
	cout  << "Id: " << id << "\n";
	/*if (id != 88) {
		Exit *exit = Exit::getInstance();
		exit->sendError(0x101, true);
	}*/
}

int BMP280::readRawData(int addr)
{
	int high_byte, middle_byte, low_byte, value;

	low_byte = wiringPiI2CReadReg8(fd, addr);
	cout << low_byte << "\t";
	low_byte = low_byte >> 4;
	middle_byte = wiringPiI2CReadReg8(fd, addr - 1);
	cout << middle_byte << "\t";
	value = (middle_byte << 4) | low_byte;
	high_byte = wiringPiI2CReadReg8(fd, addr - 2);
	cout << high_byte << "\n";

	value = (high_byte << 12) | value;
	return value;
}

void BMP280::calcBaromter()
{
	//this->temperature = readRawData(REG_TEMPERATURE);	//Temperature
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
