/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 */

#pragma once
class PWMMotorTest
{
public:
	PWMMotorTest();
	void SetSpeed(short pin, int speed);
	void ArmMotor();
	void Test();
	void Setup();
	~PWMMotorTest();

private:
	int fd;
};

