#pragma once
class PWMMotorTest
{
public:
	int fd;
	PWMMotorTest();
	void SetSpeed(int num);
	short int StartMotors(short int ret);
	void Test();
	void Setup();
	~PWMMotorTest();
};

