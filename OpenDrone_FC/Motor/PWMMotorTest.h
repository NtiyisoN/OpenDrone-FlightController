/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: This class controls the motors of the drone
 *
 * 	@author Tim Klecka
 * 	@version 0.0.2 26.06.2019
 */
#pragma once
class PWMMotorTest
{
public:
	PWMMotorTest();
	void SetSpeed(short pin, int speed);
	void ExitMotor();
	void ArmMotor();
	void CalMotor();
	void Test();
	~PWMMotorTest();

private:
	int fd;
};

