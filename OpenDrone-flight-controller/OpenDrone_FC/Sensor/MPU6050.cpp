/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: TODO
 *
 * 	@author Tim Klecka
 * 	@version 0.0.1 07.01.2019
 */
#include "MPU6050.h"
#include "../Filter/Filter.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <iostream>
using namespace std;

#define DEVICE_ADDRESS 0x68	/*Device Address/Identifier for MPU6050*/

#define PWR_MGMT_1   0x6B
#define SMPLRT_DIV   0x19
#define CONFIG       0x1A
#define GYRO_CONFIG  0x1B
#define INT_ENABLE   0x38
#define ACCEL_XOUT_H 0x3B
#define ACCEL_YOUT_H 0x3D
#define ACCEL_ZOUT_H 0x3F
#define GYRO_XOUT_H  0x43
#define GYRO_YOUT_H  0x45
#define GYRO_ZOUT_H  0x47

MPU6050::MPU6050()
{
	this->fd = wiringPiI2CSetup(DEVICE_ADDRESS);
	if (this->fd < 1) {
		cout << "wiringPiI2CSetup(addressAccel)\n";
		exit(1);
	}

	wiringPiI2CWriteReg8(this->fd, SMPLRT_DIV, 0x07);	/* Write to sample rate register */
	wiringPiI2CWriteReg8(this->fd, PWR_MGMT_1, 0x01);	/* Write to power management register */
	wiringPiI2CWriteReg8(this->fd, CONFIG, 0);			/* Write to Configuration register */
	wiringPiI2CWriteReg8(this->fd, GYRO_CONFIG, 24);	/* Write to Gyro Configuration register */
	wiringPiI2CWriteReg8(this->fd, INT_ENABLE, 0x01);	/* Write to interrupt enable register */

	this->filterAccX = new Filter(-10.0, 10.0, 2.0);
	this->filterAccY = new Filter(-10.0, 10.0, 2.0);
	this->filterAccZ = new Filter(-10.0, 10.0, 2.0);

	this->filterGyroX = new Filter(-100.0, 100.0, 2.0);
	this->filterGyroY = new Filter(-100.0, 100.0, 2.0);
	this->filterGyroZ = new Filter(-100.0, 100.0, 2.0);
}


short MPU6050::readRawData(int addr)
{
	short high_byte, low_byte, value;

	high_byte = wiringPiI2CReadReg8(fd, addr);
	low_byte = wiringPiI2CReadReg8(fd, addr + 1);
	value = (high_byte << 8) | low_byte;
	if (value >= 0x8000)
	{
		return -((65535 - value) + 1);
	}
	else
	{
		return value;
	}
}

double *MPU6050::getAccelerometerValues()
{
	static double ar[4];

	//Read new value and divide raw value by sensitivity scale factor
	ar[0] = millis();
	//g --> 1m/s^2 = 0.101972g
	ar[1] = this->filterAccX->addValue(readRawData(ACCEL_XOUT_H) / 16384.0);
	ar[2] = this->filterAccY->addValue(readRawData(ACCEL_YOUT_H) / 16384.0);
	ar[3] = this->filterAccZ->addValue(readRawData(ACCEL_ZOUT_H) / 16384.0);
	return ar;
}

double *MPU6050::getGyroscopeValues()
{
	static double ar[4];

	//Read new value and divide raw value by sensitivity scale factor
	ar[0] = millis();
	//degree/seconds
	ar[1] = this->filterGyroX->addValue(readRawData(GYRO_XOUT_H) / 131.0);
	ar[2] = this->filterGyroY->addValue(readRawData(GYRO_YOUT_H) / 131.0);
	ar[3] = this->filterGyroZ->addValue(readRawData(GYRO_ZOUT_H) / 131.0);
	return ar;
}

MPU6050::~MPU6050()
{
}