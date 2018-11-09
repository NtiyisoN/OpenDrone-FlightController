#pragma once
class PWMMotorTest
{
public:
	int fd;
	PWMMotorTest();
	void SetSpeed(int num);
	void StartMotors();
	void Test();
	void Setup();
	~PWMMotorTest();
};

