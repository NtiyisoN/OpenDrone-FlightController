/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: TODO
 *
 * 	@author Tim Klecka
 * 	@version 0.0.1 07.01.2019
 */
#pragma once
#include "AbstractSensor/Barometer.h"
#include "../Filter/Filter.h"
class BMP280 :
	public virtual Barometer
{
public:
	BMP280();
	~BMP280();
	double *getBarometerValues();
	void runBarometer();
	void interruptBaromter();

private:
	int fd;
	bool run = false;
	double temperature = 0;
	int pressure = 0;

	unsigned short int cal_t1 = 27504;
	short int cal_t2 = 26435;
	short int cal_t3 = -1000;
	unsigned short int cal_p1 = 36477;
	short int cal_p2 = -10685;
	short int cal_p3 = 3024;
	short int cal_p4 = 2855;
	short int cal_p5 = 140;
	short int cal_p6 = -7;
	short int cal_p7 = 15500;
	short int cal_p8 = -14500;
	short int cal_p9 = 6000;

	void calcBaromter();
	int read_raw(int fd, int reg);
	int compensate_temp(int raw_temp);
	float read_temperature(int fd);
	int read_pressure(int fd);
	void load_calibration(int fd);

	Filter* filter;
};

