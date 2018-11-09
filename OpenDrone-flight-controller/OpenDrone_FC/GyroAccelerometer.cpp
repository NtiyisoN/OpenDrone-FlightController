#include "GyroAccelerometer.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <iostream>
using namespace std;


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

GyroAccelerometer::GyroAccelerometer()
{
	if (this->fd < 1) {
		cout << "wiringPiI2CSetup(addressAccel)\n";
		exit(1);
	}

	wiringPiI2CWriteReg8(this->fd, SMPLRT_DIV, 0x07);	/* Write to sample rate register */
	wiringPiI2CWriteReg8(this->fd, PWR_MGMT_1, 0x01);	/* Write to power management register */
	wiringPiI2CWriteReg8(this->fd, CONFIG, 0);			/* Write to Configuration register */
	wiringPiI2CWriteReg8(this->fd, GYRO_CONFIG, 24);	/* Write to Gyro Configuration register */
	wiringPiI2CWriteReg8(this->fd, INT_ENABLE, 0x01);	/* Write to interrupt enable register */
}


short GyroAccelerometer::readRawData(int addr)
{
	short high_byte, low_byte, value;

	high_byte = wiringPiI2CReadReg8(fd, addr);
	low_byte = wiringPiI2CReadReg8(fd, addr + 1);
	value = (high_byte << 8) | low_byte;
	return value;
}

float *GyroAccelerometer::getValues()
{
	static float ar[7];

	//Read new value and divide raw value by sensitivity scale factor
	ar[0] = millis();
	//g --> 1m/s^2 = 0.101972g
	//degree/seconds

	return ar;
}


GyroAccelerometer::~GyroAccelerometer()
{
}