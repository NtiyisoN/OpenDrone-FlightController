/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: TODO
 *
 * 	@author Thomas Brych
 * 	@version 0.0.1 14.02.2019
 */
#include "Orientation.h"
#include "../Motor/PWMMotorTest.h"
#include "wiringPi.h"
#include "iostream"
#include "../Sensor/AbstractSensor/Barometer.h"
#include "../Sensor/HCSR04.h"
#include "thread"

#pragma once
class PID
{
public:
	~PID();
	static PID *getInstance(Orientation *o, PWMMotorTest *p, Barometer *b, HCSR04 *u);
	static PID *getInstanceCreated();
	static PID *instance;

	void setP(float curP);
	void setI(float curI);
	void setD(float curD);
	void setThrottle(float curThrottle);
	void setPitchSetpoint(int curPitchSetpoint);
	void setRollSetpoint(int curRollSetpoint);
	void setYawSetpoint(int curYawSetpoint);
	void setRun(bool curRun);
	void armMotor();
	bool isInit();
	void interruptPid();
	int* getThrottles();
	float *getPIDVals();
	void updateHeightControl();

	Orientation *getOrientatin();
	PWMMotorTest *getPwmMotorTest();

	void calcValues();

private:
	PID(Orientation *o, PWMMotorTest *p, Barometer *b, HCSR04 *u);
	PWMMotorTest *pwm = NULL;
	Orientation *orientation = NULL;
	Barometer *barometer = NULL;
	HCSR04 *ultrasonic = NULL;

	float pid_error_temp;
	float pid_i_mem_roll = 0, pid_roll_setpoint = 0, pid_output_roll, pid_last_roll_d_error = 0;
	float pid_i_mem_pitch = 0, pid_pitch_setpoint = 0, pid_output_pitch, pid_last_pitch_d_error = 0;
	float pid_i_mem_yaw = 0, pid_yaw_setpoint = 0, pid_output_yaw, pid_last_yaw_d_error = 0;
	float pid_output_height, pid_last_height_error;

	int esc_1, esc_2, esc_3, esc_4;
	int throttle = 1050;
	bool heightControl = false;

	void calcPid();
};
