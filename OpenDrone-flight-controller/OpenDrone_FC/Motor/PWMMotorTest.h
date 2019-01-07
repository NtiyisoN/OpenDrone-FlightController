/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: TODO
 *
 * 	@author Tim Klecka
 * 	@version 0.0.1 07.01.2019
 */
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

