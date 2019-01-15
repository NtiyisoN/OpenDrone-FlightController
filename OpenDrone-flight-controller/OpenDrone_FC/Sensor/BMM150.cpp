#include "BMM150.h"
#include <iostream>;
#include <wiringPi.h>
#include <wiringPiI2C.h>
using namespace std;

#define DEVICE_ADDRESS 0x10

BMM150::BMM150()
{
	this->fd = wiringPiI2CSetup(DEVICE_ADDRESS);
	if (this->fd < 1) {
		cout << "wiringPiI2CSetup(addressMagnetometer)\n";
		exit(1);
	}
	wiringPiI2CWriteReg8(this->fd, 0x4B, 0x01);
	wiringPiI2CWriteReg8(this->fd, 0x4C, 0x00);
}

short BMM150::readRawData(int addr, int bitshift)
{
	short high_byte, low_byte, value;

	high_byte = wiringPiI2CReadReg8(fd, addr);
	low_byte = wiringPiI2CReadReg8(fd, addr + 1);
	value = (high_byte << 8) | low_byte;
	value = value << bitshift;
	return value;
}

double *BMM150::getMagnetometerValues() {
	static double ar[4];  /* Declared locally, but saved in the data-segment (and not on the stack) */
	cout << wiringPiI2CReadReg8(this->fd, 0x40) << "    ";
	ar[0] = millis();
	ar[1] = readRawData(0x42,3); //Magnet X
	ar[2] = readRawData(0x44,3); //Magnet Y
	ar[3] = readRawData(0x46,1); //Magnet Z
	return ar;
}

void BMM150::selfTest() {
	wiringPiI2CWriteReg8(this->fd, 0x4C, 0x06);
	wiringPiI2CWriteReg8(this->fd, 0x4C, 0x07);
	bool one = true;
	while (one)
	{
		short s = wiringPiI2CReadReg8(this->fd, 0x4C);
		if (s == 0x06)
		{
			one = false;
		}
		delay(250);
	}
	cout << (wiringPiI2CReadReg8(this->fd, 0x42) >> 7) << "\n";
	cout << (wiringPiI2CReadReg8(this->fd, 0x44) >> 7) << "\n";
	cout << (wiringPiI2CReadReg8(this->fd, 0x46) >> 7) << "\n";
}

void BMM150::calibrate() {
	int16_t value_x_min = 0;
	int16_t value_x_max = 0;
	int16_t value_y_min = 0;
	int16_t value_y_max = 0;
	int16_t value_z_min = 0;
	int16_t value_z_max = 0;
	uint32_t timeStart = 0;

	short dataX = readRawData(0x42, 3);
	short dataY = readRawData(0x44, 3);
	short dataZ = readRawData(0x46, 1);
	value_x_min = dataX;
	value_x_max = dataX;
	value_y_min = dataY;
	value_y_max = dataY;
	value_z_min = dataZ;
	value_z_max = dataZ;
	delay(100);

	timeStart = millis();
	int timeout = 10000;
	while ((millis() - timeStart) < timeout)
	{
		short dataX = readRawData(0x42, 3);
		short dataY = readRawData(0x44, 3);
		short dataZ = readRawData(0x46, 1);

		/* Update x-Axis max/min value */
		if (value_x_min > dataX)
		{
			value_x_min = dataX;
			// Serial.print("Update value_x_min: ");
			// Serial.println(value_x_min);
		}
		else if (value_x_max < dataX)
		{
			value_x_max = dataX;
			// Serial.print("update value_x_max: ");
			// Serial.println(value_x_max);
		}

		/* Update y-Axis max/min value */
		if (value_y_min > dataY)
		{
			value_y_min = dataY;
			// Serial.print("Update value_y_min: ");
			// Serial.println(value_y_min);
		}
		else if (value_y_max < dataY)
		{
			value_y_max = dataY;
			// Serial.print("update value_y_max: ");
			// Serial.println(value_y_max);
		}

		/* Update z-Axis max/min value */
		if (value_z_min > dataZ)
		{
			value_z_min = dataZ;
			// Serial.print("Update value_z_min: ");
			// Serial.println(value_z_min);
		}
		else if (value_z_max < dataZ)
		{
			value_z_max = dataZ;
			// Serial.print("update value_z_max: ");
			// Serial.println(value_z_max);
		}

		cout << ".";
		delay(100);
	}

	int16_t value_offsetX = value_x_min + (value_x_max - value_x_min) / 2;
	int16_t value_offsetY = value_y_min + (value_y_max - value_y_min) / 2;
	int16_t value_offsetZ = value_z_min + (value_z_max - value_z_min) / 2;

	cout << "x: " << value_offsetX << " y: " << value_offsetY << " z: " << value_offsetZ << "\n";
}

BMM150::~BMM150()
{
}
