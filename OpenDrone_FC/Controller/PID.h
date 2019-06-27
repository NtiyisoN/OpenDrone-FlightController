/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: This class is used to calculate the speed of the single motors of the drone
 *
 * 	@author Thomas Brych, Tim Klecka
 * 	@version 0.0.2 27.06.2019
 */
#include "Orientation.h"
#include "../Motor/PWMMotorTest.h"
#include "../Sensor/AbstractSensor/Barometer.h"
#include "../Sensor/AbstractSensor/Ultrasonic.h"
#include "wiringPi.h"
#include "iostream"
#include "thread"

#pragma once
class PID
{
public:
	~PID();
	static PID *getInstance(Orientation *o, PWMMotorTest *p, Barometer *b, Ultrasonic *u);
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
	PID(Orientation *o, PWMMotorTest *p, Barometer *b, Ultrasonic *u);
	PWMMotorTest *pwm = NULL;
	Orientation *orientation = NULL;
	Barometer *barometer = NULL;
	Ultrasonic *ultrasonic = NULL;

	float pid_error_temp;
	float pid_i_mem_roll = 0, pid_roll_setpoint = 0, pid_output_roll, pid_last_roll_d_error = 0;
	float pid_i_mem_pitch = 0, pid_pitch_setpoint = 0, pid_output_pitch, pid_last_pitch_d_error = 0;
	float pid_i_mem_yaw = 0, pid_yaw_setpoint = 0, pid_output_yaw, pid_last_yaw_d_error = 0;
	float pid_output_height, pid_last_height_error;

	int esc_1, esc_2, esc_3, esc_4;
	int throttle = 1050;
	bool heightControl = false;

	float pid_p_gain_roll = 2.1; //1.25             //Gain setting for the roll P-controller 0.65
	float pid_i_gain_roll = 0.011; // 0.05;          //Gain setting for the roll I-controller 0.0006
	float pid_d_gain_roll = 65; // 90;              //Gain setting for the roll D-controller 60
	int pid_max_roll = 1000;						//Maximum output of the PID-controller (+/-)

	float pid_p_gain_pitch = pid_p_gain_roll;		//Gain setting for the pitch P-controller.
	float pid_i_gain_pitch = pid_i_gain_roll;		//Gain setting for the pitch I-controller.
	float pid_d_gain_pitch = pid_d_gain_roll;		//Gain setting for the pitch D-controller.
	int pid_max_pitch = pid_max_roll;				//Maximum output of the PID-controller (+/-)

	float pid_p_gain_yaw = 2.0;						//Gain setting for the yaw P-controller. 4.0
	float pid_i_gain_yaw = 0.005;					//Gain setting for the yaw I-controller. 0.02
	float pid_d_gain_yaw = 0.00;					//Gain setting for the yaw D-controller.
	int pid_max_yaw = 200;							//Maximum output of the PID-controller (+/-)

	float pid_p_gain_height = 3;
	float pid_d_gain_height = 15;

	float pid_cur_val = 0;
	double wantedDistane = 200;

	float maxAngle = 45;
	float factorControl = maxAngle / 480;			//Maximum 45° (480 steps)

	bool run = false, stop = false;

	void calcPid();
};
