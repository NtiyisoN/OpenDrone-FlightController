#pragma once
class PWMMotorTest
{
public:
	int fd;
	PWMMotorTest();
	void SetSpeed(int num);
	void ArmMotor();
	void Test();
	void Setup();
	~PWMMotorTest();
};

