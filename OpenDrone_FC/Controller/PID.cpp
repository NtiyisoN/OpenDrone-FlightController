/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: TODO
 *
 * 	@author Thomas Brych
 * 	@version 0.0.1 14.02.2019
 */
#include "PID.h"

PID *PID::instance = 0;

float pid_p_gain_roll = 2.0; //1.25           //Gain setting for the roll P-controller 0.65
float pid_i_gain_roll = 0.01; // 0.05;         //Gain setting for the roll I-controller 0.0006
float pid_d_gain_roll = 60; // 90;               //Gain setting for the roll D-controller 60
int pid_max_roll = 1000;						//Maximum output of the PID-controller (+/-)

float pid_p_gain_pitch = pid_p_gain_roll;		//Gain setting for the pitch P-controller.
float pid_i_gain_pitch = pid_i_gain_roll;		//Gain setting for the pitch I-controller.
float pid_d_gain_pitch = pid_d_gain_roll;		//Gain setting for the pitch D-controller.
int pid_max_pitch = pid_max_roll;				//Maximum output of the PID-controller (+/-)

float pid_p_gain_yaw = 4.0;						//Gain setting for the pitch P-controller.
float pid_i_gain_yaw = 0.02;					//Gain setting for the pitch I-controller.
float pid_d_gain_yaw = 0.00;					//Gain setting for the pitch D-controller.
int pid_max_yaw = 300;							//Maximum output of the PID-controller (+/-)

float pid_cur_val = 0;

PID *PID::getInstance(Orientation *o, PWMMotorTest *p)
{
	if (instance == 0)
	{
		instance = new PID(o, p);
	}
	return instance;
}

PID::PID(Orientation *o, PWMMotorTest *p)
{
	orientation = o;
	pwm = p;
}

void PID::calcValues()
{
	while (!run) {
		delay(50);
	}

	while (run) {
		calcPid();

		if (throttle < 1200) {
			esc_1 = throttle;   //Calculate the pulse for esc 1 (front-right - CCW)
			esc_2 = throttle;   //Calculate the pulse for esc 2 (rear-right - CW)
			esc_3 = throttle;   //Calculate the pulse for esc 3 (rear-left - CCW)
			esc_4 = throttle;   //Calculate the pulse for esc 4 (front-left - CW)
			pid_last_pitch_d_error = 0.0;
			pid_last_roll_d_error = 0.0;
			pid_last_yaw_d_error = 0.0;
		}
		else {
			esc_1 = throttle - pid_output_pitch + pid_output_roll - pid_output_yaw;   //Calculate the pulse for esc 1 (front-right - CCW)
			esc_2 = throttle + pid_output_pitch + pid_output_roll + pid_output_yaw;   //Calculate the pulse for esc 2 (rear-right - CW)
			esc_3 = throttle + pid_output_pitch - pid_output_roll - pid_output_yaw;   //Calculate the pulse for esc 3 (rear-left - CCW)
			esc_4 = throttle - pid_output_pitch - pid_output_roll + pid_output_yaw;   //Calculate the pulse for esc 4 (front-left - CW)
		}

		int speedMin = 1050;
		if (esc_1 < speedMin) esc_1 = speedMin;           //Keep the motors running.
		if (esc_2 < speedMin) esc_2 = speedMin;           //Keep the motors running.
		if (esc_3 < speedMin) esc_3 = speedMin;           //Keep the motors running.
		if (esc_4 < speedMin) esc_4 = speedMin;           //Keep the motors running.

		int speedMax = 1800;
		if (esc_1 > speedMax) esc_1 = speedMax;           //Limit the esc-1 pulse to 2500.
		if (esc_2 > speedMax) esc_2 = speedMax;           //Limit the esc-2 pulse to 2500.
		if (esc_3 > speedMax) esc_3 = speedMax;           //Limit the esc-3 pulse to 2500.
		if (esc_4 > speedMax) esc_4 = speedMax;           //Limit the esc-4 pulse to 2500.  
		
		pwm->SetSpeed(1, esc_1);	//Front left
		pwm->SetSpeed(0, esc_2);	//Rear left
		pwm->SetSpeed(3, esc_3);	//Rear right
		pwm->SetSpeed(2, esc_4);	//Front right
		delay(5);

		/*Default:
		pwm->SetSpeed(0, esc_3);
		pwm->SetSpeed(1, esc_2);
		pwm->SetSpeed(2, esc_1);
		pwm->SetSpeed(3, esc_4);*/
	}

	delay(100);
	if (!stop) {
		calcValues();
	}

	pwm->SetSpeed(16, 0);
}

void PID::calcPid() {
	double *ar = orientation->getPitchRoll();
	//std::cout << ar[0] << " " << ar[1] << " " << ar[2] << "\n";
	//Roll calculations
	pid_error_temp = ar[1] - pid_roll_setpoint;
	if (pid_error_temp != pid_error_temp) {
		calcPid();
	}
	pid_i_mem_roll += pid_i_gain_roll * pid_error_temp;
	if (pid_i_mem_roll > pid_max_roll)pid_i_mem_roll = pid_max_roll;
	else if (pid_i_mem_roll < pid_max_roll * -1)pid_i_mem_roll = pid_max_roll * -1;

	pid_output_roll = pid_p_gain_roll * pid_error_temp + pid_i_mem_roll + pid_d_gain_roll * ((pid_error_temp - pid_last_roll_d_error));
	if (pid_output_roll > pid_max_roll)pid_output_roll = pid_max_roll;
	else if (pid_output_roll < pid_max_roll * -1)pid_output_roll = pid_max_roll * -1;

	pid_last_roll_d_error = pid_error_temp;

	//Pitch calculations
	pid_cur_val = pid_error_temp = ar[0] - pid_pitch_setpoint;
	if (pid_error_temp != pid_error_temp) {
		calcPid();
	}
	pid_i_mem_pitch += pid_i_gain_pitch * pid_error_temp;
	if (pid_i_mem_pitch > pid_max_pitch)pid_i_mem_pitch = pid_max_pitch;
	else if (pid_i_mem_pitch < pid_max_pitch * -1)pid_i_mem_pitch = pid_max_pitch * -1;

	pid_output_pitch = pid_p_gain_pitch * pid_error_temp + pid_i_mem_pitch + pid_d_gain_pitch * ((pid_error_temp - pid_last_pitch_d_error));
	if (pid_output_pitch > pid_max_pitch)pid_output_pitch = pid_max_pitch;
	else if (pid_output_pitch < pid_max_pitch * -1)pid_output_pitch = pid_max_pitch * -1;

	pid_last_pitch_d_error = pid_error_temp;

	//Yaw calculations
	pid_error_temp = ar[2] - pid_yaw_setpoint;
	if (pid_error_temp != pid_error_temp) {
		calcPid();
	}
	pid_i_mem_yaw += pid_i_gain_yaw * pid_error_temp;
	if (pid_i_mem_yaw > pid_max_yaw)pid_i_mem_yaw = pid_max_yaw;
	else if (pid_i_mem_yaw < pid_max_yaw * -1)pid_i_mem_yaw = pid_max_yaw * -1;

	pid_output_yaw = pid_p_gain_yaw * pid_error_temp + pid_i_mem_yaw + pid_d_gain_yaw * ((pid_error_temp - pid_last_yaw_d_error));
	if (pid_output_yaw > pid_max_yaw)pid_output_yaw = pid_max_yaw;
	else if (pid_output_yaw < pid_max_yaw * -1)pid_output_yaw = pid_max_yaw * -1;

	pid_last_yaw_d_error = pid_error_temp;
}

void PID::setP(float curP) {
	if (curP >= 0) {
		//pid_p_gain_roll = curP;
		//pid_p_gain_pitch = curP;
	}
}

void PID::setI(float curI) {
	if (curI >= 0) {
		pid_i_gain_roll = curI;
		pid_i_gain_pitch = curI;
		std::cout << pid_i_gain_pitch << "\n";
	}
}

void PID::setD(float curD) {
	if (curD >= 0) {
		//pid_d_gain_roll = curD;
		//pid_d_gain_pitch = curD;
	}
}

void PID::setRun(bool curRun) {
	run = curRun;
}

void PID::setThrottle(float curThrottle) {
	if (curThrottle >= 1050 && curThrottle <= 1600) {
		throttle = curThrottle;
	}

	//pwm->SetSpeed(0, throttle);
	//pwm->SetSpeed(8, throttle);
}

void PID::setPitchSetpoint(int curPitchSetpoint) {
	if (curPitchSetpoint >= 1000 && curPitchSetpoint <= 2000) {
		if (curPitchSetpoint > 1480 && curPitchSetpoint < 1520) {
			pid_pitch_setpoint = 0;
		} else if (curPitchSetpoint < 1480) {
			int diff = (curPitchSetpoint - 1480)*-1;
			pid_pitch_setpoint = diff * 0.0625;
		} else if (curPitchSetpoint > 1520) {
			int diff = (curPitchSetpoint - 1520)*-1;
			pid_pitch_setpoint = diff * 0.0625;
		}
	}
}

void PID::setRollSetpoint(int curRollSetpoint) {
	int setPoint = curRollSetpoint;
	if (setPoint >= 1000 && setPoint <= 2000) {
		if (setPoint > 1480 && setPoint < 1520) {
			pid_roll_setpoint = 0;
		}
		else if (setPoint < 1480) {
			int diff = (setPoint - 1480)*-1;
			pid_roll_setpoint = diff * 0.0625;
		}
		else if (setPoint > 1520) {
			int diff = (setPoint - 1520)*-1;
			pid_roll_setpoint = diff * 0.0625;
		}
	}
}

void PID::setYawSetpoint(int curYawSetpoint) {
	//TODO: 
}

void PID::armMotor() {
	pwm->ExitMotor();
	pwm->ArmMotor();
}

void PID::interruptPid() {
	run = false;
	stop = true;
}

int *PID::getThrottles() {
	static int ar[4];
	ar[0] = esc_1;
	ar[1] = esc_2;
	ar[2] = esc_3;
	ar[3] = esc_4;
	return ar;
}

float *PID::getPIDVals() {
	static float ar[3];
	ar[0] = pid_output_pitch;
	ar[1] = pid_cur_val;
	ar[2] = pid_i_mem_pitch;
	return ar;
}

bool PID::isInit() {
	if (orientation != NULL) 
	{
		return true;
	}
	else 
	{
		return false;
	}
}

Orientation *PID::getOrientatin() {
	return orientation;
}

PID::~PID()
{
}
