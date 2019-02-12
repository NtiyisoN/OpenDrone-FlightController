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
#define MAX_PWM 400
#define MIN_PWM 200
#define HERTZ 45

PWMMotorTest::PWMMotorTest()
{
	this->fd = PCA9685Setup(PIN_BASE, 0x40, HERTZ);
	if (this->fd < 0)
	{
		printf("Error in setup\n");
	}

	PCA9685PWMReset(this->fd);
}

void PWMMotorTest::SetSpeed(short pin ,int speed) {
	pwmWrite(PIN_BASE + pin, speed);
}

void PWMMotorTest::ExitMotor() {
	pwmWrite(PIN_BASE + 16, 0);
}

void PWMMotorTest::ArmMotor() {
	pwmWrite(PIN_BASE + 16, 0);
	delay(2000);
	cout << "MIN\n";
	pwmWrite(PIN_BASE + 16, MIN_PWM);
	delay(5000);
}

void PWMMotorTest::CalMotor() {
	cout << "MAX\n";
	pwmWrite(PIN_BASE + 16, MAX_PWM);
	getchar();
	cout << "MIN\n";
	pwmWrite(PIN_BASE + 16, MIN_PWM);
	delay(5000);
	cout << "0\n";
	pwmWrite(PIN_BASE + 16, 0);
	delay(5000);
	cout << "MIN\n";
	pwmWrite(PIN_BASE + 16, MIN_PWM);
	delay(5000);
	getchar();
}

void PWMMotorTest::Test()
{
	cout << "MAX\n";
	pwmWrite(PIN_BASE + 16, MAX_PWM);
	delay(2000);
	cout << "MIN\n";
	pwmWrite(PIN_BASE + 16, MIN_PWM);
	delay(5000);

	while (true) {
		for (size_t i = MIN_PWM; i < MAX_PWM; i++)
		{
			pwmWrite(PIN_BASE + 16, i);
			cout << i << "\n";
			cout.flush();
			delay(10);
		}

		for (size_t i = MAX_PWM; i > MIN_PWM; i--)
		{
			pwmWrite(PIN_BASE + 16, i);
			cout << i << "\n";
			cout.flush();
			delay(10);
		}
	}
}

void PWMMotorTest::Setup() {
	pwmWrite(PIN_BASE + 16, 0);
	getchar();
	pwmWrite(PIN_BASE + 16, MAX_PWM);
	getchar();
	pwmWrite(PIN_BASE + 16, MIN_PWM);
	getchar();
	pwmWrite(PIN_BASE + 16, 0);
	getchar();
}


PWMMotorTest::~PWMMotorTest()
{
}
