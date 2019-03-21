/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: TODO
 *
 * 	@author Tim Klecka
 * 	@version 0.0.1 07.01.2019
 */
#include "PWMMotorTest.h"
#include <iostream>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include "PCA9685.h"

using namespace std;

#define PIN_BASE 300
#define MAX_PWM 2000
#define MIN_PWM 1000
#define HERTZ 50

/**
	PWMMotorTest
	@param void
	@return void
*/
PWMMotorTest::PWMMotorTest()
{
	this->fd = PCA9685Setup(PIN_BASE, 0x40, HERTZ);
	if (this->fd < 0)
	{
		printf("Error in setup\n");
	}

	PCA9685PWMReset(this->fd);
}

/**
	Sets speed for a Pin
	@param short pin ,int speed
	@return void

	@info
	* Use 16 for all Motors
	* Use 0-15 for single Motors
*/
void PWMMotorTest::SetSpeed(short pin ,int speed) {
	pwmWrite(PIN_BASE + pin, speed*0.208);
}

/**
	Stops all Motors
	@param void
	@return void
*/
void PWMMotorTest::ExitMotor() {
	pwmWrite(PIN_BASE + 16, 0 * 0.208);
}

/**
	Arms the ESC
	@param void
	@return void
*/
void PWMMotorTest::ArmMotor() {
	pwmWrite(PIN_BASE + 16, 0 * 0.208);
	delay(2000);
	cout << "MIN\n";
	pwmWrite(PIN_BASE + 16, MIN_PWM*0.208);
	delay(5000);
}

/**
	Calibrate the ESC
	@param short pin ,int speed
	@return void
*/
void PWMMotorTest::CalMotor() {
	cout << "MAX\n";
	pwmWrite(PIN_BASE + 16, MAX_PWM*0.208);
	getchar();
	cout << "MIN\n";
	pwmWrite(PIN_BASE + 16, MIN_PWM*0.208);
	delay(5000);
	cout << "0\n";
	pwmWrite(PIN_BASE + 16, 0 * 0.208);
	delay(5000);
	cout << "MIN\n";
	pwmWrite(PIN_BASE + 16, MIN_PWM*0.208);
	delay(5000);
	//getchar();
}

/**
	Methode to test all Motors
	@param void
	@return void

	@info
	* sets speed from min to max in 1x steps
*/
void PWMMotorTest::Test()
{
	cout << "MAX\n";
	pwmWrite(PIN_BASE + 16, MAX_PWM*0.208);
	delay(2000);
	cout << "MIN\n";
	pwmWrite(PIN_BASE + 16, MIN_PWM*0.208);
	delay(5000);

	while (true) {
		for (size_t i = MIN_PWM; i < MAX_PWM; i++)
		{
			pwmWrite(PIN_BASE + 16, i*0.208);
			cout << i << "\n";
			cout.flush();
			delay(10);
		}

		for (size_t i = MAX_PWM; i > MIN_PWM; i--)
		{
			pwmWrite(PIN_BASE + 16, i*0.208);
			cout << i << "\n";
			cout.flush();
			delay(10);
		}
	}
}

/**
	Destructor
*/
PWMMotorTest::~PWMMotorTest()
{
}
