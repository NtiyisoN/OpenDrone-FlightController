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

//#define REG_TEMPERATURE 0xFB
//#define REG_PRESSURE 0xF8*/
//#define DEVICE_ADDRESS 0x77

#define BMP280_PRESSUREDATA 0xF7
#define BMP280_TEMPDATA     0xFA

/* BMP280 default address */
#define BMP280_I2CADDR 0x76
#define BMP280_CHIPID  0xD0

/* BMP280 Registers */
#define BMP280_DIG_T1 0x88  /* R   Unsigned Calibration data (16 bits) */
#define BMP280_DIG_T2 0x8A  /* R   Signed Calibration data (16 bits) */
#define BMP280_DIG_T3 0x8C  /* R   Signed Calibration data (16 bits) */
#define BMP280_DIG_P1 0x8E  /* R   Unsigned Calibration data (16 bits) */
#define BMP280_DIG_P2 0x90  /* R   Signed Calibration data (16 bits) */
#define BMP280_DIG_P3 0x92  /* R   Signed Calibration data (16 bits) */
#define BMP280_DIG_P4 0x94  /* R   Signed Calibration data (16 bits) */
#define BMP280_DIG_P5 0x96  /* R   Signed Calibration data (16 bits) */
#define BMP280_DIG_P6 0x98  /* R   Signed Calibration data (16 bits) */
#define BMP280_DIG_P7 0x9A  /* R   Signed Calibration data (16 bits) */
#define BMP280_DIG_P8 0x9C  /* R   Signed Calibration data (16 bits) */
#define BMP280_DIG_P9 0x9E  /* R   Signed Calibration data (16 bits) */

#define BMP280_CONTROL      0xF4
#define BMP280_RESET        0xE0
#define BMP280_CONFIG       0xF5

#define SWAP_2BYTES(x) (((x & 0xFFFF) >> 8) | ((x & 0xFF) << 8))

BMP280::BMP280()
{
	this->fd = wiringPiI2CSetup(BMP280_I2CADDR);
	if (this->fd < 1) {
		cout << "wiringPiI2CSetup(addressBarometer)\n";
		exit(1);
	}
	//wiringPiI2CWriteReg8(fd, RESET, 0xB6);
	//delay(2000);
	//wiringPiI2CWriteReg8(fd, CTR_MEAS, 0x11);
	delay(1000);
	//wiringPiI2CWriteReg8(fd, BMP280_CONFIG, 0x38);
	wiringPiI2CWriteReg8(fd, BMP280_CONFIG, 0x00);
	wiringPiI2CWriteReg8(fd, BMP280_CONTROL, 0x3F);

	int id = wiringPiI2CReadReg8(fd, 0xD0);
	/*if (id != 88) {
		Exit *exit = Exit::getInstance();
		exit->sendError(0x101, true);
	}*/
	filter = new Filter(0, 500000, 50);
}

void BMP280::load_calibration(int fd)
{
	cal_t1 = wiringPiI2CReadReg16(fd, BMP280_DIG_T1);
	cal_t2 = wiringPiI2CReadReg16(fd, BMP280_DIG_T2);
	// TO DO: double check the value for t3
	//cal_t3 = wiringPiI2CReadReg16(fd, BMP280_DIG_T3);
	cal_p1 = wiringPiI2CReadReg16(fd, BMP280_DIG_P1);
	cal_p2 = wiringPiI2CReadReg16(fd, BMP280_DIG_P2);
	cal_p3 = wiringPiI2CReadReg16(fd, BMP280_DIG_P3);
	cal_p4 = wiringPiI2CReadReg16(fd, BMP280_DIG_P4);
	cal_p5 = wiringPiI2CReadReg16(fd, BMP280_DIG_P5);
	cal_p6 = wiringPiI2CReadReg16(fd, BMP280_DIG_P6);
	cal_p7 = wiringPiI2CReadReg16(fd, BMP280_DIG_P7);
	cal_p8 = wiringPiI2CReadReg16(fd, BMP280_DIG_P8);
	cal_p9 = wiringPiI2CReadReg16(fd, BMP280_DIG_P9);
}

int BMP280::read_raw(int fd, int reg)
{
	int raw = SWAP_2BYTES(wiringPiI2CReadReg16(fd, reg));
	raw <<= 8;
	raw = raw | wiringPiI2CReadReg8(fd, reg + 2);
	raw >>= 4;
	return raw;
}

int BMP280::compensate_temp(int raw_temp)
{
	int t1 = (((raw_temp >> 3) - (cal_t1 << 1)) * (cal_t2)) >> 11;
	int t2 = (((((raw_temp >> 4) - (cal_t1)) *
		((raw_temp >> 4) - (cal_t1))) >> 12) *
		(cal_t3)) >> 14;
	return t1 + t2;
}

float BMP280::read_temperature(int fd)
{
	int raw_temp = read_raw(fd, BMP280_TEMPDATA);
	int compensated_temp = compensate_temp(raw_temp);
	return (float)((compensated_temp * 5 + 128) >> 8) / 100;
}

int BMP280::read_pressure(int fd)
{
	int raw_temp = read_raw(fd, BMP280_TEMPDATA);
	int32_t compensated_temp = compensate_temp(raw_temp);
	int raw_pressure = read_raw(fd, BMP280_PRESSUREDATA);

	int64_t p1 = compensated_temp / 2 - 64000;
	int64_t p2 = p1 * p1 * (int64_t)cal_p6 / 32768;
	int64_t buf = (p1 * (int64_t)cal_p5 * 2);
	p2 += buf << 17;
	p2 += (int64_t)cal_p4 << 35;
	p1 = ((p1 * p1 * cal_p3) >> 8) + ((p1 * cal_p2) << 12);
	p1 = (((int64_t)1 << 47) + p1) * ((int64_t)cal_p1) >> 33;

	// Avoid exception caused by division by zero
	if (0 == p1)
	{
		return 0;
	}

	int64_t p = 1048576 - raw_pressure;
	p = (((p << 31) - p2) * 3125) / p1;
	p1 = ((int64_t)cal_p9 * (p >> 13) * (p >> 13)) >> 25;
	p2 = ((int64_t)cal_p8 * p) >> 19;
	p = ((p + p1 + p2) >> 8) + (((int64_t)cal_p7) << 4);

	return (int)(p / 256);
}

void BMP280::calcBaromter()
{
	int result = read_pressure(fd);
	double curP = (double)result;
	double curT = read_temperature(fd);
	
	pressure = filter->addValue(curP);
 	temperature = curT;
	if (pressure < 90000 || pressure > 105000) {
		cout << pressure << " " << temperature << "\n";
		cout.flush();
	}
	//cout << temperature << endl;
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
	load_calibration(fd);
	while (this->run)
	{
		this->calcBaromter();
		delayMicroseconds(1500);
	}
}

void BMP280::interruptBaromter()
{
	this->run = false;
}

BMP280::~BMP280()
{
}
