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
	pwmWrite(PIN_BASE + 16, 0);
}

void PWMMotorTest::SetSpeed(int num) {
	pwmWrite(PIN_BASE + 16, num);
	cout << num << "\n";
	cout.flush();
}

short int PWMMotorTest::StartMotors(short int ret) {
	cout << "MAX\n";
	pwmWrite(PIN_BASE + 16, MAX_PWM);
	delay(2000);
	cout << "MIN\n";
	pwmWrite(PIN_BASE + 16, MIN_PWM);
	delay(5000);
	return ret;
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
